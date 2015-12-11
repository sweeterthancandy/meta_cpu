#pragma once

template<typename T>
struct is_label : public boost::mpl::false_{};


#define DEF_LABEL( NAME )                                                      \
        struct NAME {                                                          \
                static std::string to_string() {                               \
                        return BOOST_PP_STRINGIZE( NAME );                     \
                }                                                              \
                static void print() { std::cout << to_string() << "\n"; }      \
        }; \
        template<>\
        struct is_label<NAME> : mpl::true_ {};


DEF_LABEL(my_label)

