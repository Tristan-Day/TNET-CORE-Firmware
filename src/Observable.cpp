#include <Observable.hpp>

template <typename T> void Observable<T>::set(T value)
{
    this->value = value;

    for (function<void(T)> observer : observers)
    {
        observer(value);
    }
}

template <typename T> T Observable<T>::get()
{
    return value;
}

template <typename T> void Observable<T>::onUpdate(function<void(T)> callable)
{
    observers.push_back(callable);
}