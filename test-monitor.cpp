#include "monitor.h"
#include <iostream>


void check(const char* name, float temp, float pulse, float spo2, AlarmMask expected) {
    AlarmMask result = evaluateVitals(temp, pulse, spo2);
    int ok = vitalsOk(temp, pulse, spo2);
    bool mask_ok = (result == expected);
    bool return_ok = ((expected == ALARM_NONE && ok == 1) || (expected != ALARM_NONE && ok == 0));

    std::cout << "Test: " << name << " -> ";
    if (mask_ok && return_ok) {
        std::cout << "PASS\n";
    } else {
        std::cout << "FAIL (mask=" << int(result) << ", vitalsOk=" << ok << ")\n";
    }
}

int main() {
    
    setTestMode(true);

    check("All normal", 98.6f, 75.0f, 98.0f, ALARM_NONE);
    check("Temp low", 94.0f, 75.0f, 98.0f, ALARM_TEMPERATURE_LOW);
    check("Temp high", 103.0f, 75.0f, 98.0f, ALARM_TEMPERATURE_HIGH);
    check("Pulse low", 98.6f, 55.0f, 98.0f, ALARM_PULSE_LOW);
    check("Pulse high", 98.6f, 105.0f, 98.0f, ALARM_PULSE_HIGH);
    check("SpO2 low", 98.6f, 75.0f, 85.0f, ALARM_SPO2_LOW);
    check("Multiple failures", 103.0f, 50.0f, 85.0f,
          static_cast<AlarmMask>(ALARM_TEMPERATURE_HIGH | ALARM_PULSE_LOW | ALARM_SPO2_LOW));

    return 0;
}
