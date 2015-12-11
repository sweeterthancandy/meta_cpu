#pragma once

#include <sstream>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/preprocessor.hpp>
#include <boost/type_index.hpp>

#include "register.h"
#include "int.h"


using default_contex = mpl::vector< 
        // 0: registers memory
        mpl::vector<
                int_<0>,
                int_<0>,
                int_<0>
        >,
        // 1: the stack
        mpl::list<>,
        // 2: program counter
        mpl::int_<0>,
        // 3: is running
        mpl::true_
        // 4: output stack
        ,mpl::list<>
>;


static_assert( mpl::size<default_contex>::value == 5 ,"");


template<typename Ctx>
struct is_running : 
        mpl::at_c<Ctx,3>::type
{};
