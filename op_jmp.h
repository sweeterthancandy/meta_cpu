#pragma once

#include "op_traits.h"
#include "contex.h"

template<class Param>
struct jmp{
        template<typename Ctx>
        struct apply{
                using type = typename ctx_util::set_counter<Ctx,Param>::type;
        };
        template<typename Label = Param>
        static typename boost::enable_if< is_label<Label>,std::string >::type to_string(){
                std::stringstream sstr;
                sstr << boost::format("jmp %s")
                        % label_string<Param>::get()
                        ;
                return sstr.str();
        }
        template<typename Label = Param>
        static typename boost::disable_if< is_label<Label>,std::string >::type to_string(){
                std::stringstream sstr;
                sstr << boost::format("jmp %s")
                        % Param()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string<>() << "\n";
        }
        template<typename Index>
        struct resolve_label{
                using type = typename mpl::if_<
                        is_label<Param>,
                        jmp< typename mpl::at<Index, Param>::type >,
                        jmp<Param> 
                >::type;
        };
};
template<class Param>
struct is_jump_op< jmp<Param> > : mpl::true_{};
