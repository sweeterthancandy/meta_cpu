#pragma once

#include <sstream>
#include <boost/mpl/for_each.hpp>
#include <boost/preprocessor.hpp>
#include "register.h"

using default_contex = mpl::vector< 
        // registers memory
        mpl::vector<
                int_<0>,
                int_<0>,
                int_<0>
        >,
        // the stack
        mpl::list<>,
        // program register
        mpl::int_<0>,
        // is running
        mpl::true_
>;
template<typename Ctx>
struct is_running : 
        mpl::at_c<Ctx,3>::type
{};
template<typename Ctx>
std::string ctx_to_string(){
        using reg = typename mpl::at_c<Ctx,0>::type;
        using stack = typename mpl::at_c<Ctx,1>::type;
        using offset = typename mpl::at_c<Ctx,2>::type;
        using running = typename mpl::at_c<Ctx,3>::type;


        std::stringstream sstr;
        
        sstr << "[";
        mpl::for_each<reg>([&sstr,first=true](auto&& value)mutable{
                if( ! first )
                        sstr << ",";
                sstr << boost::format("%3s") % value;
                first = false;
        });
        sstr << "]";

        sstr << boost::format(", %3s, %3s")
                        % offset()
                        % running();
        sstr << "  {";
        mpl::for_each<stack>([&sstr,first=true](auto&& value)mutable{
                if( ! first )
                        sstr << ",";
                sstr << value;
                first = false;
        });
        sstr << "}";

        return sstr.str();
}
