#pragma once

#include "label.h"
#include "int.h"

template<typename T>
struct is_int_or_reg : 
        boost::mpl::or_<
                is_int_<T>,
                is_reg<T> 
        >
{};
