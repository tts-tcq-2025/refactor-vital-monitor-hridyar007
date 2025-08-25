#include "monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

static bool g_test_mode = false;
void setTestMode(bool enabled) { g_test_mode = enabled; }

// Table of thresholds (shared for all vitals)
struct VitalData {
    float value;
    float low;
    float high;
    AlarmMask lowAlarm;
    AlarmMask highAlarm;
};

static const char* alarmMessages[] = {
    "Temperature is too low!",
    "Temperature is critical!",
    "Pulse Rate is too low!",
    "Pulse Rate is too high!",
    "Oxygen Saturation out of range!",
    "Respiration Rate is too low!",
    "Respiration Rate is too high!"
};

// Flashing alert (side effect, separate from evaluation)
static void flashAlert(const char* msg) {
    std::cout << msg << "\n";
    if (g_test_mode) return;
    for (int i = 0; i < 6; i++) {
        std::cout << "\r* " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\r *" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "\r  \n";
}

// Pure function: evaluate a single vital (complexity = 1)
static AlarmMask checkVital(const VitalData& v) {
    AlarmMask mask = ALARM_NONE;
    mask |= (v.value < v.low) ? v.lowAlarm : ALARM_NONE;
    mask |= (v.value > v.high) ? v.highAlarm : ALARM_NONE;
    return mask;
}

// Pure function: evaluate all vitals (complexity = 1)
AlarmMask evaluateVitals(float temperature, float pulseRate, float spo2, float respirationRate) {
    VitalData vitals[] = {
        {temperature, 95.0f, 102.0f, ALARM_TEMPERATURE_LOW, ALARM_TEMPERATURE_HIGH},
        {pulseRate, 60.0f, 100.0f, ALARM_PULSE_LOW, ALARM_PULSE_HIGH},
        {spo2, 90.0f, 1e9f, ALARM_SPO2_LOW, ALARM_NONE},
        {respirationRate, 12.0f, 20.0f, ALARM_RESP_LOW, ALARM_RESP_HIGH}
    };
    AlarmMask mask = ALARM_NONE;
    for (const auto& v : vitals) mask |= checkVital(v);
    return mask;
}

// Side-effect function: trigger alerts (complexity = 1)
int vitalsOk(float temperature, float pulseRate, float spo2, float respirationRate) {
    AlarmMask mask = evaluateVitals(temperature, pulseRate, spo2, respirationRate);
    if (mask == ALARM_NONE) return 1;

    for (int i = 0; i < 7; i++) {
        if (isAlarmSet(mask, 1 << i)) flashAlert(alarmMessages[i]);
    }
    return 0;
}
