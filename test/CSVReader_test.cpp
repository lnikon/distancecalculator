#define CATCH_CONFIG_MAIN

#include "CSVReader.hpp"

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("CSVReader: Parse Floats", "[csvreader]")
{
    auto csvReader = std::make_unique<readers::CSVReader<float>>();
    auto container = csvReader->read(
        Path("/home/nikon/projects/distancecalculator/test/data/csv/test_data.csv"));

    REQUIRE(container != nullptr);
    REQUIRE(container->rowCount() == 4);
    REQUIRE(container->columnCount() == 5);
}
