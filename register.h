#pragma once

namespace mpl = boost::mpl;

template<int Index>
struct reg{
        BOOST_MPL_ASSERT(( mpl::less<mpl::int_<Index>,mpl::int_<4>> ));
        static std::string to_string(){
               return "%" + boost::lexical_cast<std::string>(Index); 
        }
        using index = mpl::int_<Index>;

        template<typename Ctx>
        struct eval{
                using type = typename mpl::at<
                        typename mpl::at_c<Ctx,0>::type,
                        index
                >::type;
        };
};
template<typename T>
struct is_reg : public boost::mpl::false_{};
template<int Index>
struct is_reg< reg<Index> > : public boost::mpl::true_{};
