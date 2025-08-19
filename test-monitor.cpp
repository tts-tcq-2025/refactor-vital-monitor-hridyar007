#include "monitor.h"
#include <cassert>
#include <sstream>
#include <iostream>

int main() {
    setTestMode(true);

    // Capture output
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    vitalsOk(94.0f, 75.0f, 98.0f); // temp low
    std::string output = buffer.str();
    assert(output.find("Temperature is too low!") != std::string::npos);

    buffer.str(""); buffer.clear();

    vitalsOk(103.0f, 75.0f, 98.0f); // temp high
    output = buffer.str();
    assert(output.find("Temperature is critical!") != std::string::npos);

    buffer.str(""); buffer.clear();

    vitalsOk(98.6f, 55.0f, 98.0f); // pulse low
    output = buffer.str();
    assert(output.find("Pulse Rate is too low!") != std::string::npos);

    buffer.str(""); buffer.clear();

    vitalsOk(98.6f, 105.0f, 98.0f); // pulse high
    output = buffer.str();
    assert(output.find("Pulse Rate is too high!") != std::string::npos);

    buffer.str(""); buffer.clear();

    vitalsOk(98.6f, 75.0f, 85.0f); // SpO2 low
    output = buffer.str();
    assert(output.find("Oxygen Saturation out of range!") != std::string::npos);

    std::cout.rdbuf(old); // restore
    return 0;
}
