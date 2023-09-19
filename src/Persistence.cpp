#include <Persistence.hpp>

template <typename T>
Persistent<T>::Persistent(Preferences* collection, const char* key)
{
    this->key = key;
    this->collection = collection;
}

template class Persistent<long>;

template <> void Persistent<long>::set(long value)
{
    collection->putLong(key, value);
}

template <> long Persistent<long>::get()
{
    return collection->getLong(key, 0);
}

template class Persistent<float>;

template <> void Persistent<float>::set(float value)
{
    collection->putFloat(key, value);
}

template <> float Persistent<float>::get()
{
    return collection->getFloat(key, 0);
}

template class Persistent<bool>;

template <> void Persistent<bool>::set(bool value)
{
    collection->putBool(key, value);
}

template <> bool Persistent<bool>::get()
{
    return collection->getBool(key, false);
}
