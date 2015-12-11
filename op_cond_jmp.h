#pragma once

#include <boost/mpl/string.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/preprocessor.hpp>

#include "op_traits.h"
#include "contex.h"

template<class L_Param, class R_Param, class Operator, class Name>
struct basic_cond_jump{
        BOOST_MPL_ASSERT(( is_int_or_reg<L_Param> ));

        using operator_t = Operator;
        using name_t = Name;

        template<typename Ctx>
        struct apply{
                using lp = typename L_Param::template eval<Ctx>::type;
                using offset = R_Param;

                using type = typename ctx_util::set_counter<
                        Ctx,
                        typename mpl::if_<
                                typename mpl::apply<operator_t, lp>::type,
                                offset,
                                typename mpl::next< 
                                        typename ctx_util::get_counter<Ctx>::type
                                >::type
                        >::type
                >::type;
        };
        template<typename Label = R_Param>
        static typename boost::enable_if< is_label<Label>,std::string >::type to_string(){
                std::stringstream sstr;
                sstr << boost::format("%s %s, %s")
                        % mpl::c_str<name_t>::value
                        % L_Param::to_string()
                        % label_string<R_Param>::get()
                        ;
                return sstr.str();
        }
        template<typename Label = R_Param>
        static typename boost::disable_if< is_label<Label>,std::string >::type to_string(){
                std::stringstream sstr;
                sstr << boost::format("%s %s, %s")
                        % mpl::c_str<name_t>::value
                        % L_Param::to_string()
                        % R_Param()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }

        template<typename Index>
        struct resolve_label{
                using type = typename mpl::if_<
                        is_label<R_Param>,
                        basic_cond_jump<L_Param, typename mpl::at<Index, R_Param>::type , operator_t, name_t>,
                        basic_cond_jump<L_Param, R_Param, operator_t, name_t>
                >::type;
        };
};


#define OP_COND_JMP_seq\
        ((jgt)(mpl::greater)(('j')('g')('t')))\
        ((jeq)(mpl::equal_to)(('j')('e')('q')))\

#define OP_COND_JMP_aux(r,data,elem)                                    \
        template<typename L_Param, typename R_Param>\
        struct BOOST_PP_SEQ_ELEM(0,elem) \
                : basic_cond_jump<\
                        L_Param, R_Param,\
                         mpl::lambda< BOOST_PP_SEQ_ELEM(1,elem)<mpl::_,mpl::int_<0> > >,\
                         mpl::string<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_ELEM(2,elem))>\
        >{};\
        template<class L_Param, class R_Param>\
        struct have_labels_to_resolve< BOOST_PP_SEQ_ELEM(0,elem)< L_Param,  R_Param> > : mpl::true_{};\

BOOST_PP_SEQ_FOR_EACH(OP_COND_JMP_aux,~, OP_COND_JMP_seq)

#undef OP_COND_JMP_seq
#undef OP_COND_JMP_aux


        
