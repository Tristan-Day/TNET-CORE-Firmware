#pragma once

#include <Arduino.h>

using namespace std;

template <typename Type> class Observable
{
  protected:
    Type value;
    vector<function<void(Type)>> observers;

public:
    Observable();
    
    Type get();

    void set(Type value);

    void onUpdate(function<void(Type)> callable);
};