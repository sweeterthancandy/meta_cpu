#pragma once

#include "contex.h"


template<class L_Param, class R_Param>
struct mov{
        BOOST_MPL_ASSERT(( is_int_or_reg<L_Param> ));
        BOOST_MPL_ASSERT(( is_reg<R_Param> ));
        template<typename Ctx>
        struct apply{
                // value to move to register
                using value = typename L_Param::template eval<Ctx>::type;
                // index of register
                using index = typename R_Param::index;
                        
                using old_reg = typename ctx_util::get_registers<Ctx>::type;
                        
                using reg = typename assign_reg< old_reg,value,index>::type;

                using type = typename ctx_util::increment_counter<
                        typename ctx_util::set_registers< Ctx,reg>::type 
                >::type;
        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << boost::format("mov %s, %s")
                        % L_Param::to_string()
                        % R_Param::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};
template<class Param>
struct push{
        template<typename Ctx>
        struct apply{
                using value = typename Param::template eval<Ctx>::type;

                using type = typename ctx_util::increment_counter<
                        typename ctx_util::set_stack<
                                Ctx,
                                typename mpl::push_front<
                                        typename ctx_util::get_stack<Ctx>::type,
                                        value
                                >::type
                        >::type
                >::type;

        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << boost::format("push %s")
                        % Param::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};
#if 1
template<class Reg>
struct pop{
        BOOST_MPL_ASSERT(( is_reg<Reg> ));
        template<typename Ctx>
        struct apply{
                using value = typename Reg::template eval<Ctx>::type;

                using type = typename ctx_util::increment_counter<
                        typename ctx_util::set_stack<
                                Ctx,
                                typename mpl::pop_front<
                                        typename ctx_util::get_stack<Ctx>::type
                                >::type
                        >::type
                >::type;

        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << boost::format("pop %s")
                        % Reg::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};
#endif
template<class Param>
struct opush{
        template<typename Ctx>
        struct apply{
                using value = typename Param::template eval<Ctx>::type;

                using type = typename ctx_util::increment_counter<
                        typename ctx_util::set_output_stack<
                                Ctx,
                                typename mpl::push_front<
                                        typename ctx_util::get_output_stack<Ctx>::type,
                                        value
                                >::type
                        >::type
                >::type;

        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << boost::format("push %s")
                        % Param::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};

struct nop{
        template<typename Ctx>
        struct apply{
                using type = typename ctx_util::increment_counter<Ctx>::type;
        };
        static std::string to_string(){
                return "nop";
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};

struct end{
        template<typename Ctx>
        struct apply{
                using type = typename ctx_util::set_running<Ctx,mpl::false_>::type;
        };
        static std::string to_string(){
                return "end";
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};

