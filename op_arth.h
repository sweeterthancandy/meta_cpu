#pragma once

#include <boost/mpl/string.hpp>
#include <boost/preprocessor.hpp>

#include "primatives.h"
#include "assign_reg__.h"


template<class L_Param, class R_Param, class Result, class Operator, class Name>
struct basic_binary_op{
        BOOST_MPL_ASSERT(( is_int_or_reg< L_Param > ));
        BOOST_MPL_ASSERT(( is_int_or_reg< R_Param > ));
        BOOST_MPL_ASSERT(( is_reg<Result> ));

        using operator_t = Operator;
        using name_t = Name;

        template<typename Ctx>
        struct apply{
                using lp = typename L_Param::template eval<Ctx>::type;
                using rp = typename R_Param::template eval<Ctx>::type;

                using result = typename mpl::apply<operator_t,lp,rp>::type;

                using reg = typename assign_reg<
                        typename mpl::at_c<Ctx,0>::type, 
                        result,
                        typename Result::index
                >::type;

                using type = mpl::vector<
                        reg,
                        typename mpl::at_c<Ctx,1>::type, 
                        typename mpl::next<
                                typename mpl::at_c<Ctx,2>::type
                        >::type,
                        typename mpl::at_c<Ctx,3>::type
                >;
        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << boost::format("%s %s, %s, %s") 
                        % mpl::c_str<name_t>::value
                        % L_Param::to_string()
                        % R_Param::to_string()
                        % Result::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};


#define OP_ARTH_seq\
        ((sub)(mpl::minus)(('s')('u')('b')))\
        ((add)(mpl::plus)(('a')('d')('d')))\
        ((mod)(mpl::modulus)(('m')('o')('d')))\
        ((mul)(mpl::times)(('m')('u')('l')))\

#define OP_ARTH_aux(r,data,elem)\
        template<class L_Param, class R_Param, class Result>\
        struct BOOST_PP_SEQ_ELEM(0,elem) : basic_binary_op<\
                L_Param, R_Param, Result,\
                 mpl::lambda<BOOST_PP_SEQ_ELEM(1,elem)<mpl::_,mpl::_> >, \
                        mpl::string<BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_ELEM(2,elem))>\
        >{};\

BOOST_PP_SEQ_FOR_EACH(OP_ARTH_aux,~,OP_ARTH_seq)

#undef OP_ARTH_seq
#undef OP_ARTH_aux

