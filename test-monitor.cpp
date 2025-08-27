#include "monitor.h"
#include <cassert>
#include <iostream>

void testTemperatureLow() {
    enableTestMode(true);
    AlarmMask mask = checkVitals(90.0f, 70.0f, 95.0f, 15.0f);
    assert(isAlarmSet(mask, ALARM_TEMPERATURE_LOW));
    auto msgs = getAlarmMessages(mask);
    assert(msgs[0] == "Temperature is too low!");
}

void testPulseHigh() {
    enableTestMode(true);
    AlarmMask mask = checkVitals(98.0f, 150.0f, 95.0f, 15.0f);
    assert(isAlarmSet(mask, ALARM_PULSE_HIGH));
}

void testNormalVitals() {
    enableTestMode(true);
    AlarmMask mask = checkVitals(98.0f, 70.0f, 95.0f, 15.0f);
    assert(mask == ALARM_NONE);
}

int main() {
    testTemperatureLow();
    testPulseHigh();
    testNormalVitals();
    std::cout << "All tests passed!\n";
    return 0;
}
