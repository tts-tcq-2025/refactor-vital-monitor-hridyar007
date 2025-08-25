#include "monitor.h"
#include <iostream>

// Simple helper to run a single test case
void check(const char* name,
           float temperature, float pulse, float spo2, float resp,
           AlarmMask expected) {
    AlarmMask result = evaluateVitals(temperature, pulse, spo2, resp);
    int ok = vitalsOk(temperature, pulse, spo2, resp);

    bool mask_ok = (result == expected);
    bool return_ok = ((expected == ALARM_NONE && ok == 1) ||
                      (expected != ALARM_NONE && ok == 0));

    std::cout << "Test: " << name << " -> ";
    if (mask_ok && return_ok) {
        std::cout << "PASS\n";
    } else {
        std::cout << "FAIL (mask=" << int(result) << ", vitalsOk=" << ok << ")\n";
    }
}

int main() {
    setTestMode(true);

    // --- Normal case ---
    check("All normal", 98.6f, 75.0f, 98.0f, 16.0f, ALARM_NONE);

    // --- Single alarms ---
    check("Temperature low", 94.0f, 75.0f, 98.0f, 16.0f, ALARM_TEMPERATURE_LOW);
    check("Temperature high", 103.0f, 75.0f, 98.0f, 16.0f, ALARM_TEMPERATURE_HIGH);
    check("Pulse low", 98.6f, 55.0f, 98.0f, 16.0f, ALARM_PULSE_LOW);
    check("Pulse high", 98.6f, 105.0f, 98.0f, 16.0f, ALARM_PULSE_HIGH);
    check("SpO2 low", 98.6f, 75.0f, 85.0f, 16.0f, ALARM_SPO2_LOW);
    check("Resp low", 98.6f, 75.0f, 98.0f, 8.0f, ALARM_RESP_LOW);
    check("Resp high", 98.6f, 75.0f, 98.0f, 25.0f, ALARM_RESP_HIGH);

    // --- Multiple alarms ---
    check("Multiple alarms", 103.0f, 50.0f, 85.0f, 25.0f,
          static_cast<AlarmMask>(ALARM_TEMPERATURE_HIGH |
                                 ALARM_PULSE_LOW |
                                 ALARM_SPO2_LOW |
                                 ALARM_RESP_HIGH));

    // --- Edge cases: exactly at thresholds ---
    check("Temperature at low boundary", 95.0f, 75.0f, 98.0f, 16.0f, ALARM_NONE);
    check("Temperature at high boundary", 102.0f, 75.0f, 98.0f, 16.0f, ALARM_NONE);
    check("Pulse at low boundary", 98.6f, 60.0f, 98.0f, 16.0f, ALARM_NONE);
    check("Pulse at high boundary", 98.6f, 100.0f, 98.0f, 16.0f, ALARM_NONE);
    check("SpO2 at low boundary", 98.6f, 75.0f, 90.0f, 16.0f, ALARM_NONE);
    check("Resp at low boundary", 98.6f, 75.0f, 98.0f, 12.0f, ALARM_NONE);
    check("Resp at high boundary", 98.6f, 75.0f, 98.0f, 20.0f, ALARM_NONE);

    // --- Combined edges ---
    check("All at lower bounds", 95.0f, 60.0f, 90.0f, 12.0f, ALARM_NONE);
    check("All at upper bounds", 102.0f, 100.0f, 98.0f, 20.0f, ALARM_NONE);

    return 0;
}
