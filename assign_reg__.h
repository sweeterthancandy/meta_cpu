#pragma once

#include <boost/mpl/copy.hpp>
#include <boost/mpl/iterator_range.hpp>

template<class Reg, class Value, class Index>
struct assign_reg{
        using null = mpl::vector0<>;
        using mid = typename mpl::advance<
                typename mpl::begin<Reg>::type,
                Index
        >::type;
        using first = typename mpl::copy<
                mpl::iterator_range<
                        typename mpl::begin<Reg>::type,
                        mid
                >,
                mpl::back_inserter<null>
        >::type;
        using second = typename mpl::push_back<first,Value>::type;
        using third = typename mpl::copy<
                mpl::iterator_range<
                        typename mpl::next<mid>::type,
                        typename mpl::end<Reg>::type
                >,
                mpl::back_inserter<second>
        >::type;

        using type = third;

        BOOST_MPL_ASSERT((mpl::equal_to< mpl::size<Reg>, mpl::size<type> > ));
};
