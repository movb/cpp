/* Persistent single linked list.
 * Based on: http://bartoszmilewski.com/2013/11/13/functional-data-structures-in-c-lists/
 * Original source: https://github.com/BartoszMilewski/Okasaki/blob/master/SimpleList/ListGC.h
 * /

#include <memory>
#include <cassert>

template <class T>
class List
{
    struct Item;
public:
    List() {}
    List(T v, List tail) : _head(std::make_shared<Item>(v, tail._head)) {}
    bool isEmpty() const { return !_head; }

    T front() const
    {
        assert(!isEmpty());
        return _head->_val;
    }
    List pop_front() const
    {
        assert(!isEmpty());
        return List(_head->_next);
    }

    List push_front(T v) const {
        return List(v, *this);
    }
private:
    explicit List(std::shared_ptr<const Item> const &items) : _head(items) {}

    struct Item
    {
        Item(T v, std::shared_ptr<const Item> const &tail) : _val(v), _next(tail) {}
        T _val;
        std::shared_ptr<const Item> _next;
    };

    std::shared_ptr<const Item> _head;
};

template<class U, class T, class F>
List<U> fmap(F f, List<T> lst)
{
    static_assert(std::is_convertible<F, std::function<U(T)>>::value,
            "fmap requires a function type U(T)");
    if(lst.isEmpty())
        return List<U>();
    else
        return List<U>(f(lst.front()), fmap<U>(f, lst.pop_front()));
}

template <class T, class P>
List<T> filter(P p, List<T> lst)
{
    static_assert(std::is_convertible<P, std::function<bool(T)>>::value,
            "filter requires a function type bool(T)");
    if(lst.isEmpty())
        return List<T>();
    if(p(lst.front()))
        return List<T>(lst.front(),filter(p,lst.pop_front()));
    else
        return filter(p,lst.pop_front());
}

template <class T, class U, class F>
U foldr(F f, U acc, List<T> lst)
{
    static_assert(std::is_convertible<F, std::function<U(U,T)>>::value,
            "foldr requires a function type U(U,T)");
    if(lst.isEmpty())
        return acc;
    else
        return f(lst.front(), foldr(f, acc, lst.pop_front()));
}

template <class T, class U, class F>
U foldl(F f, U acc, List<T> lst)
{
    static_assert(std::is_convertible<F, std::function<U(U,T)>>::value,
            "foldl requires a function type U(U,T)");
    if(lst.isEmpty())
        return acc;
    else
        return foldl(f, f(acc, lst.front(), lst.pop_front()));
}

template<class T, class F>
void forEach(List<T> lst, F f)
{
    static_assert(std::is_convertible<F, std::function<void(T)>>::value,
            "forEach requires a function type void(T)");
    if(!lst.isEmpty()) {
        f(lst.front());
        forEach(lst.pop_front(), f);
    }
}

template<class T>
List<T> concat(List<T> const & a, List<T> const &b)
{
    if (a.isEmpty())
        return b;
    return List<T>(a.front(), concat(a.pop_front(), b));
}

template<class T>
List<T> reverse(List<T> const & lst)
{
    return foldl([](List<T> const & acc, T v)
            {
                return List<T>(v, acc);
            }, List<T>(), lst);
}
