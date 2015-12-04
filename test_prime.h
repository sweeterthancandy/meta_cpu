#pragma once

#include "label.h"


using start_outer_ = label<0>;
using start_inner_ = label<1>;
using end_inner_  = label<2>;
using end_outer_  = label<3>;
using not_prime_  = label<4>;

#define DEF_LABEL_STRING(LABEL)\
        template<>\
        struct label_string<LABEL>\
        {\
                static std::string get(){\
                        return BOOST_PP_STRINGIZE(LABEL);\
                }\
        };

DEF_LABEL_STRING(start_outer_)
DEF_LABEL_STRING(start_inner_)
DEF_LABEL_STRING(end_inner_ )
DEF_LABEL_STRING(end_outer_ )
DEF_LABEL_STRING(not_prime_ )

#undef DEF_LABEL_STRING

using prog_ = mpl::vector<
        mov< int_<3>, reg<0> >, 
        start_outer_,
        sub< reg<0>, int_<CONFIG_MAX_PRIME>, reg<2> >,
        jgt< reg<2>, end_outer_> , 
        mov< int_<3>, reg<1> >,
        start_inner_,
        mul< reg<1>, reg<1>, reg<2> >,
        sub< reg<2>, reg<0>, reg<2> >,
        jgt< reg<2>, end_inner_ >,
        mod< reg<0>, reg<1>, reg<2> >,
        jeq< reg<2>, not_prime_ >,
        add< reg<1>, int_<2>, reg<1> >,
        jmp<start_inner_>,
        end_inner_,
        push< reg<0> >,
        not_prime_,
        add< reg<0>, int_<2>, reg<0> >,
        jmp<start_outer_>,
        end_outer_
>;
using prog  = program<prog_>;
