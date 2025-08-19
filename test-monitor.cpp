#include "monitor.h"
#include <cassert>
#include <sstream>
#include <iostream>

int main() {
    setTestMode(true);

    // --- Return value tests ---
    assert(vitalsOk(98.6f, 75.0f, 98.0f) == 1);   // all normal
    assert(vitalsOk(94.0f, 75.0f, 98.0f) == 0);   // temp low
    assert(vitalsOk(103.0f, 75.0f, 98.0f) == 0);  // temp high
    assert(vitalsOk(98.6f, 55.0f, 98.0f) == 0);   // pulse low
    assert(vitalsOk(98.6f, 105.0f, 98.0f) == 0);  // pulse high
    assert(vitalsOk(98.6f, 75.0f, 85.0f) == 0);   // SpO2 low
    assert(vitalsOk(103.0f, 50.0f, 85.0f) == 0);  // multiple alarms

    // --- Message output tests ---
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    vitalsOk(94.0f, 75.0f, 98.0f);
    assert(buffer.str().find("Temperature is too low!") != std::string::npos);
    buffer.str(""); buffer.clear();

    vitalsOk(103.0f, 75.0f, 98.0f);
    assert(buffer.str().find("Temperature is critical!") != std::string::npos);
    buffer.str(""); buffer.clear();

    vitalsOk(98.6f, 55.0f, 98.0f);
    assert(buffer.str().find("Pulse Rate is too low!") != std::string::npos);
    buffer.str(""); buffer.clear();

    vitalsOk(98.6f, 105.0f, 98.0f);
    assert(buffer.str().find("Pulse Rate is too high!") != std::string::npos);
    buffer.str(""); buffer.clear();

    vitalsOk(98.6f, 75.0f, 85.0f);
    assert(buffer.str().find("Oxygen Saturation out of range!") != std::string::npos);
    buffer.str(""); buffer.clear();

    std::cout.rdbuf(old); // restore

    std::cout << "✅ All tests passed!\n";
    return 0;
}
