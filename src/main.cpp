#include "hello.h"

#include <iostream>
#include <optional>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv)
{
    std::cout << ns_hello::hello("Initial setup");
    
    return 0;
}
