#pragma once

template<typename Label>
struct label_string;

template<int Index>
struct label{
        template<typename Context>
        struct apply{
                using ctx = Context;
        };
        static std::string to_string(){
                std::stringstream sstr;
                sstr << label_string<label<Index> >::get() << ":";
                return sstr.str();
        }
        static void print(){
                std::cout << to_string() << "\n";
        }
};

template<typename T>
struct is_label : public boost::mpl::false_{};

template<int Index>
struct is_label< label<Index> > : public boost::mpl::true_{};
