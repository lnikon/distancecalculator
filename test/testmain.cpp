#define CATCH_CONFIG_MAIN
#include "hello.h"

#include <iostream>

#include <catch2/catch.hpp>

namespace ns_testmain
{

bool testmain()
{
    const std::string msg = "my_message";
    return ns_hello::hello(msg) == std::string{"Hello, " + msg + "\n"};    
}
} // ns_testmain

int main()
{
    std::cout << (ns_testmain::testmain() ? "PASS" : "FAIL") << "\n";
    return 0;
}

