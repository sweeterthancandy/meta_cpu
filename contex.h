#pragma once

#include <sstream>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/preprocessor.hpp>
#include <boost/type_index.hpp>

#include "register.h"
#include "int.h"


template<size_t Num_Registers>
using make_default_contex = mpl::vector< 
        // 0: registers memory
        //mpl::vector<
                //int_<0>,
                //int_<0>,
                //int_<0>
        //>,

        typename mpl::fold<
                mpl::range_c<size_t,0,Num_Registers>,
                mpl::vector<>,
                mpl::push_back<
                        mpl::_1,
                        int_<0>
                >
        >::type,
                
        // 1: the stack
        mpl::list<>,
        // 2: program counter
        mpl::int_<0>,
        // 3: is running
        mpl::true_
        // 4: output stack
        ,mpl::list<>
>;




template<typename Ctx>
struct is_running : 
        mpl::at_c<Ctx,3>::type
{};
