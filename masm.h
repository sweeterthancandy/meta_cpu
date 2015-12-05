#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/exception/all.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/end.hpp>
#include <boost/mpl/next.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/map.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_index.hpp>


#define PRINT(X) do{ std::cout << #X " = " << (X) << "\n";}while(0)

namespace mpl = boost::mpl;

#include "register.h"
#include "int.h"
#include "label.h"
#include "contex.h"
#include "op_arth.h"
#include "op_cond_jmp.h"
#include "op_jmp.h"
#include "ops.h"


/*
 * this translate all jump instructions
 * to numeric offsets
 */

namespace aux{
        template<typename Iter, typename End>
        typename boost::enable_if<
                std::is_same<Iter,End>
        >::type print_asm_(int n){}

        template<typename Iter, typename End>
        typename boost::disable_if<
                std::is_same<Iter,End>
        >::type print_asm_(int n=0){
                using next = typename mpl::next<Iter>::type;
                using type = typename mpl::deref<Iter>::type;

                std::cout << boost::format("%-3i ") % n;
                type::print();

                print_asm_<next,End>(n+1);
        }

        template<
                class Iter,
                class End,
                class Offset,
                class Index>
        struct make_index{
        };

        template<typename Tuple, typename Label>
        struct push_index{
                BOOST_MPL_ASSERT(( is_label<Label> ));
                using offset = typename mpl::at_c<Tuple,0>::type;
                using old_index = typename mpl::at_c<Tuple,1>::type;
                BOOST_MPL_ASSERT_NOT(( mpl::has_key< old_index, Label> ));
                using pair = mpl::pair<Label,offset>;
                using index = typename mpl::insert<old_index, pair>::type;
                using type = mpl::vector<offset,index>;
        };

        template<typename Tuple>
        struct inc_offset{
                using type = mpl::vector<
                        typename mpl::next<
                                typename mpl::at_c<Tuple,0>::type
                        >::type,
                        typename mpl::at_c<Tuple,1>::type
                >;
        };


        using mpl::_1;
        using mpl::_2;

        template<typename Jmp_Op, class Index>
        struct resolve_label{
                using type = typename Jmp_Op::template resolve_label<Index>::type;
        };

        template<class Asm>
        struct assembler{
                using label_index_meta = typename mpl::fold<
                        Asm,
                        mpl::vector<
                                mpl::int_<0>,
                                mpl::map<>
                        >,
                        mpl::if_< 
                                is_label< _2>, 
                                push_index< _1, _2 >, 
                                inc_offset<_1> 
                        >
                >::type;
                using prog_size = typename mpl::at_c<label_index_meta,0>::type;
                using label_index = typename mpl::at_c<label_index_meta,1>::type;

                using type_meta = typename mpl::fold<
                        Asm,
                        mpl::vector<>,
                        mpl::if_<
                                is_label<_2>,
                                _1,
                                mpl::push_back<
                                        _1,
                                        mpl::if_<
                                                is_jump_op<_2>,
                                                resolve_label<_2,label_index>,
                                                _2
                                        >
                                >
                        >
                >::type;

                using type = typename mpl::push_back<type_meta, end >::type;

                using without_labels = typename mpl::fold<
                        Asm,
                        mpl::vector<>,
                        mpl::if_<
                                is_label<_2>,
                                _1,
                                mpl::push_back<_1,_2>
                        >
                >::type;

                static void debug(){
                        std::cout << boost::typeindex::type_id<prog_size>().pretty_name() << "\n";
                        std::cout << boost::typeindex::type_id<label_index>().pretty_name() << "\n";
                }
        };



}



template<typename Asm, typename Ctx>
struct execution_contex
{
        BOOST_MPL_ASSERT(( is_running<Ctx> ));

        using offset = typename mpl::at_c<Ctx,2>::type;
        using to_execute = typename mpl::at<Asm,offset>::type;

        using next_ctx = typename to_execute::template apply<Ctx>::type;


        using meta = typename mpl::if_<
                is_running<next_ctx>,
                execution_contex<Asm,next_ctx>,
                mpl::identity<next_ctx>
        >::type;
        using type = typename meta::type;
        
        template<typename C=next_ctx>
        static typename boost::enable_if< is_running<C> >::type debug(int n=0){
                std::cout << boost::format("%-3i %-15s %s\n")
                        % n
                        % to_execute::to_string()
                        % ctx_to_string<next_ctx>()
                        ;
               execution_contex<Asm,next_ctx>::debug(n+1); 

        }
        
        template<typename C=next_ctx>
        static typename boost::disable_if< is_running<C> >::type debug(int n=0){
                std::cout << boost::format("%-3i %-15s %s\n")
                        % n
                        % to_execute::to_string()
                        % ctx_to_string<next_ctx>()
                        ;
        }
};


template<typename Asm>
struct program{

        using assember_type = aux::assembler<Asm>;
        using assembed = typename assember_type::type;
        using without_labels = typename assember_type::without_labels;

        using exe = execution_contex<assembed, default_contex>;

        using contex = typename exe::type;
        using stack = typename mpl::at_c<contex,1>::type;


        static void print_asm(){
                aux::print_asm_<
                    typename mpl::begin<Asm>::type,
                    typename mpl::end<Asm>::type
                >();    
        }
        static void print_assembed(){
                aux::print_asm_<
                    typename mpl::begin<assembed>::type,
                    typename mpl::end<assembed>::type
                >();    
        }
        static void print_without_labels(){
                aux::print_asm_<
                    typename mpl::begin<without_labels>::type,
                    typename mpl::end<without_labels>::type
                >();    
        }

        static void debugger(){
                exe::debug();
        }
        

        static void print_stack(){
        } 

        static void debug(){
                assember_type::debug();
        }
};
