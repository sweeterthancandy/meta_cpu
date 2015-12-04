#pragma once

#include <sstream>
#include <boost/mpl/for_each.hpp>

using default_contex = mpl::vector< 
        // registers memory
        mpl::vector<
                int_<0>,
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
        using reg0 = typename mpl::at_c<reg,0>::type;
        using reg1 = typename mpl::at_c<reg,1>::type;
        using reg2 = typename mpl::at_c<reg,2>::type;
        using reg3 = typename mpl::at_c<reg,3>::type;
        using stack = typename mpl::at_c<Ctx,1>::type;
        using offset = typename mpl::at_c<Ctx,2>::type;
        using running = typename mpl::at_c<Ctx,3>::type;

        std::stringstream sstr;

        sstr << boost::format("[%3s,%3s,%3s,%3s], %3s, %3s")
                        % reg0() % reg1() % reg2() % reg3()
                        % offset()
                        % running();
        sstr << "  {";
        bool first=true;
        mpl::for_each<stack>([&sstr,&first](auto&& value)mutable{
                //std::cout << boost::typeindex::type_id<decltype(value)>().pretty_name() << "\n";
                if( ! first )
                        sstr << ",";
                sstr << value;
                first = false;
        });
        sstr << "}";

        return sstr.str();
}
