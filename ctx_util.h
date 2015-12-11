#pragma once

#include <boost/mpl/for_each.hpp>

struct ctx_index{
        enum type{
                registers = 0,
                stack, // 1
                counter, // 2
                running, // 3
                output_stack, // 4
                size  // 5

        };
};


namespace ctx_util{


        template<class Ctx>
        struct duck_test_contex
                : mpl::bool_<
                        mpl::size<Ctx>::value == ctx_index::size
                 >
        {};
        

        template<class Ctx>
        struct get_registers{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename mpl::at_c<Ctx,ctx_index::registers>::type;
        };
        template<class Ctx>
        struct get_stack{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename mpl::at_c<Ctx,ctx_index::stack>::type;
        };
        template<class Ctx>
        struct get_running{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename mpl::at_c<Ctx,ctx_index::running>::type;
        };
        template<class Ctx>
        struct get_counter{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename mpl::at_c<Ctx,ctx_index::counter>::type;
        };
        template<class Ctx>
        struct get_output_stack{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename mpl::at_c<Ctx,ctx_index::output_stack>::type;
        };

        template<class Registers, class Stack, class Counter, class Running, class Output_Stack>
        struct make_contex{
                using type = mpl::vector<Registers,Stack,Counter,Running,Output_Stack>;
        };

        template<class Ctx, class Registers>
        struct set_registers{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename make_contex<
                        Registers,
                        typename get_stack<Ctx>::type,
                        typename get_counter<Ctx>::type,
                        typename get_running<Ctx>::type,
                        typename get_output_stack<Ctx>::type
                >::type;
        };
        template<class Ctx, class Stack>
        struct set_stack{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        Stack,
                        typename get_counter<Ctx>::type,
                        typename get_running<Ctx>::type,
                        typename get_output_stack<Ctx>::type
                >::type;
        };
        template<class Ctx, class Counter>
        struct set_counter{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        typename get_stack<Ctx>::type,
                        Counter,
                        typename get_running<Ctx>::type,
                        typename get_output_stack<Ctx>::type
                >::type;
        };
        template<class Ctx, class Running>
        struct set_running{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        typename get_stack<Ctx>::type,
                        typename get_counter<Ctx>::type,
                        Running,
                        typename get_output_stack<Ctx>::type
                >::type;
        };
        template<class Ctx, class Output_Stack>
        struct set_output_stack{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        typename get_stack<Ctx>::type,
                        typename get_counter<Ctx>::type,
                        typename get_running<Ctx>::type,
                        Output_Stack
                >::type;
        };


        template<class Ctx>
        struct increment_counter{
                BOOST_MPL_ASSERT(( duck_test_contex<Ctx> ));
                using type = typename set_counter<
                        Ctx,
                        typename mpl::next<
                                typename get_counter<Ctx>::type
                        >::type
                >::type;
        };


}

namespace ctx_to_string_detail{
        struct identity_transform{
                template<typename T>
                T operator()(T&& val)const{
                        return val;
                }
        };
        template<typename Vec,typename Transform_Type = identity_transform>
        std::ostream& stream_aux(std::ostream& ostr, const std::string& prefix, const std::string& postfix, Transform_Type&& trans = identity_transform()){
                ostr << prefix;
                mpl::for_each<Vec>([&ostr,&trans,first=true](auto&& value)mutable{
                        if( ! first )
                                ostr << ",";
                        ostr << trans(value);
                        first = false;
                });
                ostr << postfix;
        }
}

template<typename Ctx>
std::string ctx_to_string(){

        using registers    = typename ctx_util::get_registers<Ctx>::type;
        using stack        = typename ctx_util::get_stack<Ctx>::type;
        using output_stack = typename ctx_util::get_output_stack<Ctx>::type;
        using counter      = typename ctx_util::get_counter<Ctx>::type;
        using running      = typename ctx_util::get_running<Ctx>::type;

        std::stringstream sstr;

        ctx_to_string_detail::stream_aux<registers>( sstr, "[", "]", [](auto&& value)->std::string{ return str(boost::format("%3s") % value);} );

        sstr << boost::format(", %3s, %3s")
                        % static_cast<int>(counter())
                        % static_cast<bool>(running());
        
        sstr << " ";
        ctx_to_string_detail::stream_aux<stack>( sstr, "{", "}");
        sstr << ", ";
        ctx_to_string_detail::stream_aux<output_stack>( sstr, "{", "}");

        return sstr.str();
}
