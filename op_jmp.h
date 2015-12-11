#pragma once

#include <boost/mpl/front.hpp>
#include <boost/mpl/pop_front.hpp>

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
struct have_labels_to_resolve< jmp<Param> > : mpl::true_{};



template<class Param>
struct call{
        template<typename Ctx>
        struct apply{
                using type = typename ctx_util::set_counter<
                        typename ctx_util::set_stack<
                                Ctx,
                                typename mpl::push_front<
                                        typename ctx_util::get_stack<Ctx>::type,
                                        typename mpl::next<
                                                typename ctx_util::get_counter<Ctx>::type
                                        >::type
                                >::type
                        >::type,
                        Param
                >::type;
        };
        template<typename Label = Param>
        static typename boost::enable_if< is_label<Label>,std::string >::type to_string(){
                std::stringstream sstr;
                sstr << boost::format("call %s")
                        % label_string<Param>::get()
                        ;
                return sstr.str();
        }
        template<typename Label = Param>
        static typename boost::disable_if< is_label<Label>,std::string >::type to_string(){
                std::stringstream sstr;
                sstr << boost::format("call %s")
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
                        call< typename mpl::at<Index, Param>::type >,
                        call<Param> 
                >::type;
        };
};

struct ret{
        template<typename Ctx>
        struct apply{
                using type = typename ctx_util::set_counter<
                        typename ctx_util::set_stack<
                                Ctx,
                                typename mpl::pop_front<
                                        typename ctx_util::get_stack<Ctx>::type
                                >::type
                        >::type ,
                        typename mpl::front<
                                typename ctx_util::get_stack<Ctx>::type
                        >::type
                >::type;
        };
        static std::string to_string(){
                return "ret";
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};


template<class Param>
struct have_labels_to_resolve< call<Param> > : mpl::true_{};
