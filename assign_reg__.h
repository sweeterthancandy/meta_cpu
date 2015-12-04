#pragma once

template<class Reg, class Value, class Index>
struct assign_reg{
        using type = typename mpl::insert<
                Reg,
                typename mpl::advance<
                        typename mpl::begin<Reg>::type,
                        Index
                >::type,
                Value
        >::type;
};
