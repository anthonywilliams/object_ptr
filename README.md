# `jss::object_ptr<T>`

This is an implementation of a class similar
to [`std::experimental::observer_ptr`](https://en.cppreference.com/w/cpp/experimental/observer_ptr)
from the [Library Fundamentals TS v2](http://wg21.link/n4562), but with various improvements
suggested in WG21 email discussions of the feature.

## Differences to `std::experimental::observer_ptr`

The most obvious change is the name: `observer_ptr` is a bad name, because it conjures up the idea
of the Observer pattern, but it doesn't really "observe" anything. I believe `object_ptr` is better:
it is a pointer to an object, so doesn't have any array-related functionality such as pointer
arithmetic, but it doesn't tell you anything about ownership.

The most important change to functionality is that it has **implicit** conversions from raw
pointers, `std::shared_ptr<T>` and `std::unique_ptr<T>`. This facilitates the use of
`jss::object_ptr<T>` as a drop-in replacement for `T*` in function parameters. There is nothing you
can do with a `jss::object_ptr<T>` that you can't do with a `T*`, and in fact there is considerably
less that you can do. The same applies with `std::shared_ptr<T>` and `std::unique_ptr<T>`: you are
reducing functionality, so this is safe, and reducing typing for safe operations is a good thing.

The other change is the removal of the `release()` member function. An `object_ptr` doesn't own
anything, so it can't release ownership. To clear it, call `reset()`; if you want the wrapped
pointer, call `get()` first.
