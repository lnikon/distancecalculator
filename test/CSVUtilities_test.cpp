#define CATCH_CONFIG_MAIN

#include "TestConfig.hpp"
#include "Utility.hpp"

#include <catch2/catch.hpp>

#include <istream>
#include <memory>
#include <iostream>

TEST_CASE("CSVUtilities: Row Count", "[common]")
{
    std::ifstream inCsv(DISC_TEST_ROOT + "/data/csv/test_data.csv");
    if (!inCsv.is_open())
    {
        REQUIRE_FALSE(true);
    }

    const std::size_t rowCount = countLines(inCsv);
	REQUIRE(rowCount == 128);
}

TEST_CASE("CSVUtilities: Column Count", "[common]")
{
    std::ifstream inCsv(DISC_TEST_ROOT + "/data/csv/test_data.csv");
    if (!inCsv.is_open())
    {
        REQUIRE_FALSE(true);
    }

    const std::size_t columnCount = countColumns(inCsv);
	REQUIRE(columnCount == 512);
}
