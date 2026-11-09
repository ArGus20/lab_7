#include <iostream>
#include <string>
#include <stdexcept>

void testBadExceptions(int type) {
    if (type == 1) throw 404;
    if (type == 2) throw std::string("String exception error");
    throw std::runtime_error("Standard runtime exception");
}

int main() {
    for (int i = 1; i <= 3; ++i) {
        try {
            testBadExceptions(i);
        } catch (const std::exception& e) {
            std::cout << "std::exception: " << e.what() << '\n';
        } catch (...) {
            std::cout << "catch(...): primitive/unknown type caught\n";
        }
    }
    return 0;
}