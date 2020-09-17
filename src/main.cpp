#include "DistanceCalculator.hpp"

#include <iostream>
#include <optional>
#include <thread>
#include <vector>

/*
 * Initially distance calculation will be possible only with one engine
 */

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    DistanceCalculator<float> dc;
    dc.setDistanceCalculatorEngineType(DistanceCalculatorEngineType::Sequential);
    dc.calculate();
    return 0;
}
