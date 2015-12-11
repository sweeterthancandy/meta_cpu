#pragma once

#include <boost/mpl/for_each.hpp>

struct ctx_index{
        enum type{
                registers = 0,
                stack,
                counter,
                running,
                output_stack
        };
};


namespace ctx_util{

        template<class Ctx>
        struct get_registers{
                using type = typename mpl::at_c<Ctx,ctx_index::registers>::type;
        };
        template<class Ctx>
        struct get_stack{
                using type = typename mpl::at_c<Ctx,ctx_index::stack>::type;
        };
        template<class Ctx>
        struct get_running{
                using type = typename mpl::at_c<Ctx,ctx_index::running>::type;
        };
        template<class Ctx>
        struct get_counter{
                using type = typename mpl::at_c<Ctx,ctx_index::counter>::type;
        };

        template<class Registers, class Stack, class Counter, class Running>
        struct make_contex{
                using type = mpl::vector<Registers,Stack,Counter,Running>;
        };

        template<class Ctx, class Registers>
        struct set_registers{
                using type = typename make_contex<
                        Registers,
                        typename get_stack<Ctx>::type,
                        typename get_counter<Ctx>::type,
                        typename get_running<Ctx>::type
                >::type;
        };
        template<class Ctx, class Stack>
        struct set_stack{
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        Stack,
                        typename get_counter<Ctx>::type,
                        typename get_running<Ctx>::type
                >::type;
        };
        template<class Ctx, class Counter>
        struct set_counter{
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        typename get_stack<Ctx>::type,
                        Counter,
                        typename get_running<Ctx>::type
                >::type;
        };
        template<class Ctx, class Running>
        struct set_running{
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        typename get_stack<Ctx>::type,
                        typename get_counter<Ctx>::type,
                        Running
                >::type;
        };


        template<class Ctx>
        struct increment_counter{
                using type = typename make_contex<
                        typename get_registers<Ctx>::type,
                        typename get_stack<Ctx>::type,
                        typename mpl::next<
                                typename get_counter<Ctx>::type
                        >::type,
                        typename get_running<Ctx>::type
                >::type;
        };
}


template<typename Ctx>
std::string ctx_to_string(){

        using registers = typename ctx_util::get_registers<Ctx>::type;
        using stack     = typename ctx_util::get_stack<Ctx>::type;
        using counter   = typename ctx_util::get_counter<Ctx>::type;
        using running   = typename ctx_util::get_running<Ctx>::type;

        std::stringstream sstr;
        
        sstr << "[";
        mpl::for_each<registers>([&sstr,first=true](auto&& value)mutable{
                if( ! first )
                        sstr << ",";
                sstr << boost::format("%3s") % value;
                first = false;
        });
        sstr << "]";

        sstr << boost::format(", %3s, %3s")
                        % static_cast<int>(counter())
                        % static_cast<bool>(running());
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
