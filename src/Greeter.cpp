#include "Greeter.h"

class StdoutWriter : public Writer {
public:
    INJECT(StdoutWriter()) = default;

    void write(const std::string &s) override {
        std::cout << s;
    }
};

class GreeterImpl : public Greeter {
private:
    Writer* writer;

public:
    INJECT(GreeterImpl(Writer * writer)) : writer(writer) {

    }

    void greet() override {
        writer->write("Hello World\n");
    }
};

fruit::Component<Greeter> getGreeterComponent() {
    return fruit::createComponent()
        .bind<Writer, StdoutWriter>()
        .bind<Greeter, GreeterImpl>();
}