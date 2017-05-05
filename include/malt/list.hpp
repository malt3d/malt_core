#pragma once
#include <type_traits>

namespace malt
{
namespace mp
{
    template <typename...> struct list {};

    template <class...> struct append;
    template <class T, class... Ts>
    struct append <list<Ts...>, T>
    {
        using type = list<Ts..., T>;
    };
    template <class... Ts> using append_t = typename append<Ts...>::type;

    template <class...> struct prepend;
    template <class T, class... Ts>
    struct prepend <list<Ts...>, T>
    {
        using type = list<T, Ts...>;
    };
    template <class... Ts> using prepend_t = typename prepend<Ts...>::type;

    template <class...> struct front;
    template <class T, class... Ts> struct front<list<T, Ts...>> { using type = T; };
    template <class... Ts> using front_t = typename front<Ts...>::type;

    template <class...> struct tail;
    template <class T, class... Ts> struct tail<list<T, Ts...>> { using type = list<Ts...>; };
    template <class... Ts> using tail_t = typename tail<Ts...>::type;

    template <class...> struct reverse;
    template <>
    struct reverse <list<>>
    {
        using type = list<>;
    };

    template <class Front, class... Ts>
    struct reverse<list<Front, Ts...>>
    {
        using type = append_t<typename reverse<list<Ts...>>::type, Front>;
    };
    template<class... Ts> using reverse_t = typename reverse<Ts...>::type;

    template <class...> struct map;

    template <class T>
    struct map <T, list<>>
    {
        using type = list<>;
    };

    template <class T, class Front, class... Ts>
    struct map <T, list<Front, Ts...>>
    {
    private:
        using Tail = list<Ts...>;
    public:
        using type = prepend_t< typename map<T, Tail>::type, typename T:: template invoke<Front> >;
    };

    template <class... Ts> using map_t = typename map<Ts...>::type;

    template <class...> struct index_of;
    template <class T>
    struct index_of<T, list<>>
    {
        static constexpr auto value = -10000;
    };
    template <class T, class Front, class... Ts>
    struct index_of<T, list<Front, Ts...>>
    {
        static constexpr auto value = std::is_same<T, Front>() ? 0 : 1 + index_of<T, list<Ts...>>::value;
    };

    template <class... Ts>
    constexpr int index_of_t()
    {
        return index_of<Ts...>::value;
    }

    template <int, class...> struct type_at;

    template <class Front, class... Ts>
    struct type_at<0, list<Front, Ts...>>
    {
        using type = Front;
    };

    template <int i, class... Ts>
    struct type_at<i, list<Ts...>>
    {
        using type = typename type_at <i - 1, tail_t<list<Ts...>>>::type;
    };

    template<int i, class... Ts>
    using type_at_t = typename type_at<i, Ts...>::type;

    template <template <typename...> class, typename...> struct convert;

    template <template <typename...> class T, typename... Ts>
    struct convert <T, list<Ts...>>
    {
        using type = T<Ts...>;
    };

    template <template <typename...> class T, typename... Ts>
    using convert_t = typename convert<T, Ts...>::type;

    template <template <class> class U>
    struct mapper {
        template <class T>
        using invoke = U<T>;
    };

    template <class F>
    constexpr void for_each(list<>, const F&) {}

    template <class... Ts, class F, class List = list<Ts...>>
    constexpr void for_each(List, const F& f)
    {
        f(static_cast<front_t<List>*>(nullptr));
        for_each(tail_t<List>(), f);
    }

    template <bool, class...> struct filter;

    template <class Head, class Filter>
    struct filter<false, Filter, list<Head>>
    {
        using type = list<>;
    };

    template <class Head, class Filter>
    struct filter<true, Filter, list<Head>>
    {
        using type = list<Head>;
    };

    template <class Filter, class _, class... Ts>
    struct filter<false, Filter, list<_, Ts...>>
    {
        using type = typename filter<Filter:: template value<front_t<list<Ts...>>>(), Filter, list<Ts...>>::type;
    };

    template <class Filter, class Head, class... Ts>
    struct filter<true, Filter, list<Head, Ts...>>
    {
        using type = prepend_t<typename filter<Filter:: template value<front_t<list<Ts...>>>(), Filter, list<Ts...>>::type, Head>;
    };

    template <class Filter, class T>
    using filter_t = typename filter<Filter:: template value<front_t<T>>(), Filter, T>::type;

    template <class...> struct concat;
    template <class... Ts, class... Us>
    struct concat<list<Ts...>, list<Us...>>
    {
        using type = list<Ts..., Us...>;
    };
    template <class... Ts>
    using concat_t = typename concat<Ts...>::type;

    template<class...> struct length;
    template<class... Ts>
    struct length<list<Ts...>>
    {
        static constexpr auto value = sizeof...(Ts);
    };

    template <class... Ts>
    constexpr int length_t()
    {
        return length<list<Ts...>>::value;
    }

    template <class List>
    struct intersect_filter
    {
        template <class elem>
        static constexpr auto value()
        {
            return index_of_t<elem, List>() >= 0;
        }
    };

    template <class List>
    struct diff_filter
    {
        template <class elem>
        static constexpr auto value()
        {
            return !intersect_filter<List>::template value<elem>();
        }
    };
}
}