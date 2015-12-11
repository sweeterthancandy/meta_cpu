#pragma once

#include "ctx_util.h"

namespace mpl = boost::mpl;

template<int Index>
struct reg{
        static std::string to_string(){
               return "%" + boost::lexical_cast<std::string>(Index); 
        }
        using index = mpl::int_<Index>;

        template<typename Ctx>
        struct eval{
                using type = typename mpl::at<
                        typename ctx_util::get_registers<Ctx>::type,
                        index
                >::type;
        };
};
template<typename T>
struct is_reg : public boost::mpl::false_{};
template<int Index>
struct is_reg< reg<Index> > : public boost::mpl::true_{};
