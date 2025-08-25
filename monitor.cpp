#include "monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

static bool g_test_mode = false;

void setTestMode(bool enabled) {
    g_test_mode = enabled;
}

// Threshold table (avoids duplication)
static const VitalThreshold thresholds[] = {
    {95.0f, 102.0f, ALARM_TEMPERATURE_LOW,  ALARM_TEMPERATURE_HIGH},
    {60.0f, 100.0f, ALARM_PULSE_LOW,        ALARM_PULSE_HIGH},
    {90.0f, 1e9f,   ALARM_SPO2_LOW,         ALARM_NONE}, // no upper alarm
    {12.0f, 20.0f,  ALARM_RESP_LOW,         ALARM_RESP_HIGH}
};

// Message mapping
static const char* alarmMessages[] = {
    "Temperature is too low!",
    "Temperature is critical!",
    "Pulse Rate is too low!",
    "Pulse Rate is too high!",
    "Oxygen Saturation out of range!",
    "Respiration Rate is too low!",
    "Respiration Rate is too high!"
};

// Pure function: evaluate a single vital against thresholds
static AlarmMask checkVital(float value, const VitalThreshold& t) {
    if (value < t.low) return t.lowAlarm;
    if (value > t.high) return t.highAlarm;
    return ALARM_NONE;
}

// Pure function: evaluate all vitals
AlarmMask evaluateVitals(float temperature, float pulseRate, float spo2, float respirationRate) {
    float values[] = {temperature, pulseRate, spo2, respirationRate};
    AlarmMask mask = ALARM_NONE;
    for (int i = 0; i < 4; i++) {
        addAlarm(mask, checkVital(values[i], thresholds[i]));
    }
    return mask;
}

// I/O: flashing alert (kept separate from logic)
static void flashAlert(const char* message) {
    std::cout << message << "\n";
    if (g_test_mode) return;

    for (int i = 0; i < 6; ++i) {
        std::cout << "\r* " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\r *" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "\r  \n";
}

// I/O wrapper: trigger alarms
int vitalsOk(float temperature, float pulseRate, float spo2, float respirationRate) {
    AlarmMask mask = evaluateVitals(temperature, pulseRate, spo2, respirationRate);
    if (mask == ALARM_NONE) return 1;

    for (int bit = 0; bit < 7; bit++) {
        AlarmMask alarmBit = (1 << bit);
        if (isAlarmSet(mask, alarmBit)) {
            flashAlert(alarmMessages[bit]);
        }
    }
    return 0;
}
