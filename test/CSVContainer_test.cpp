#define CATCH_CONFIG_MAIN

#include "CSVContainer.hpp"

#include <memory>
#include <random>
#include <cstdlib>

#include <catch2/catch.hpp>

TEST_CASE("CSVContainer: Initial state", "[CSVContainer]")
{
	structures::CSVContainer<float> container;
	REQUIRE(container.rowCount() == 0);
	REQUIRE(container.columnCount() == 0);
	// REQUIRE(container.begin() == container.end());
	// REQUIRE(container.cbegin() == container.cend());
}


TEST_CASE("CSVContainer: Construct from raw 1D array", "[CSVContainer]")
{
	/* Prepare raw arrays DONT FORGET TO DELETE LATER */
	std::srand(std::time(nullptr));
	const std::size_t rowDim = 16;
	const std::size_t colDim = 32;
	const std::size_t sizeFlat = rowDim * colDim;
	float* fPtr = new float[sizeFlat];
	for (int i = 0; i < sizeFlat; i++) {
		*(fPtr + i) = std::rand();
	}

	structures::CSVContainer<float> container(fPtr, rowDim, colDim);
	REQUIRE(container.rowCount() == rowDim);
	REQUIRE(container.columnCount() == colDim);
	for (int i = 0; i < rowDim; i++) {
		for (int j = 0; j < colDim; j++) {
			REQUIRE(container.at(i, j) == *(fPtr + i * colDim + j));
		}
	}

	delete[] fPtr;
}

