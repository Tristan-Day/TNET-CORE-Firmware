#pragma once

#include <Preferences.h>

using namespace std;

template <typename Type> class Persistent
{
  protected:
    const char* key;
    Type value;

    Preferences* collection;

  public:
    Persistent(Preferences* collection, const char* key);

    virtual void set(Type value);

    virtual Type get();
};
