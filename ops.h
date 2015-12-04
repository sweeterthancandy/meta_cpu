#pragma once

template<class L_Param, class R_Param, class Result>
struct tribinaryop{
        template<typename Ctx>
        struct apply{
                using type = mpl::vector<
                        typename mpl::at_c<Ctx,0>::type, 
                        typename mpl::at_c<Ctx,1>::type, 
                        typename mpl::at_c<Ctx,2>::type, 
                        mpl::false_
                >;
        };
        static std::string to_string(){}
};



template<class L_Param, class R_Param>
struct mov{
        BOOST_MPL_ASSERT(( is_int_or_reg<L_Param> ));
        BOOST_MPL_ASSERT(( is_reg<R_Param> ));
        template<typename Ctx>
        struct apply{
                // value to move to register
                using value = typename L_Param::template eval<Ctx>::type;
                // index of register
                using index = typename R_Param::index;
                        
                using old_reg = typename mpl::at_c<Ctx,0>::type;
                        
                using reg = typename assign_reg< old_reg,value,index>::type;

                static void debug(){
                        PRINT(value());
                        PRINT(index());

                }

                // make new contex
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
                sstr << boost::format("mov %s, %s")
                        % L_Param::to_string()
                        % R_Param::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};
template<class L_Param, class R_Param>
struct binop{
        template<typename Ctx>
        struct apply{
                using type = mpl::vector<
                        typename mpl::at_c<Ctx,0>::type, 
                        typename mpl::at_c<Ctx,1>::type, 
                        typename mpl::at_c<Ctx,2>::type, 
                        mpl::false_
                >;
        };
        static std::string to_string(){}
};
template<class Param>
struct push{
        template<typename Ctx>
        struct apply{
                using value = typename Param::template eval<Ctx>::type;
                using type = mpl::vector<
                        typename mpl::at_c<Ctx,0>::type, 
                        typename mpl::push_front<
                                typename mpl::at_c<Ctx,1>::type, 
                                value
                        >::type,
                        typename mpl::next<typename mpl::at_c<Ctx,2>::type>::type,
                        mpl::true_
                >;
        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << boost::format("push %s")
                        % Param::to_string()
                        ;
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};


struct end{
        template<typename Ctx>
        struct apply{
                using type = mpl::vector<
                        typename mpl::at_c<Ctx,0>::type, 
                        typename mpl::at_c<Ctx,1>::type, 
                        typename mpl::at_c<Ctx,2>::type, 
                        mpl::false_
                >;
        };
        static std::string to_string(){
                return "end";
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};
template<class Param>
struct unaryop{
        template<typename Ctx>
        struct apply{
                using type = mpl::vector<
                        typename mpl::at_c<Ctx,0>::type, 
                        typename mpl::at_c<Ctx,1>::type, 
                        typename mpl::at_c<Ctx,2>::type, 
                        mpl::false_
                >;
        };
        static std::string to_string(){}
};
