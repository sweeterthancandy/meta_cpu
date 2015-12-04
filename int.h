#pragma once

template<int Value>
struct int_{
        static std::string to_string(){
               return boost::lexical_cast<std::string>(Value); 
        }
        operator int()const{
                return Value;
        }
        template<typename Ctx>
        struct eval{
                using type = mpl::int_<Value>;
        };
};
template<typename T>
struct is_int_ : public boost::mpl::false_{};
template<int Value>
struct is_int_< int_<Value> > : public boost::mpl::true_{};
