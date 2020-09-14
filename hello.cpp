#include "hello.h"

#include <iostream>

namespace ns_hello
{

std::string hello(const std::string& msg)
{
   return std::string{"Hello, "} + msg + std::string{"\n"};
}

} // ns_hello
