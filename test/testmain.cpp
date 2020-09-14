#define CATCH_CONFIG_MAIN
#include "hello.h"

#include <catch2/catch.hpp>

TEST_CASE( "Check ns_hello::hello(msg) return value", "[hello]" ) {
    REQUIRE(ns_hello::hello("Name") == std::string{"Hello, Name\n"});
    REQUIRE(ns_hello::hello("Name") != std::string{"Hello, Name"});
    REQUIRE(ns_hello::hello("") == std::string{"Hello, \n"});
}
