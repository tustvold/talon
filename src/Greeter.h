#pragma once
#include <iostream>
#include <fruit/fruit.h>

class Writer {
public:
    virtual void write(const std::string& s) = 0;
};

class Greeter {
public:
    virtual void greet() = 0;

    virtual ~Greeter() {
        std::cout << "Greeter Destroyed" << std::endl;
    }
};


fruit::Component<Greeter> getGreeterComponent();