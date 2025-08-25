#include "monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

static bool g_test_mode = false;

void setTestMode(bool enabled) {
    g_test_mode = enabled;
}

static void flashAlert(const std::string& message) {
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

// Pure function: no I/O
AlarmMask evaluateVitals(const std::vector<Vital>& vitals) {
    AlarmMask mask = ALARM_NONE;
    for (const auto& v : vitals) {
        if (v.value < v.min) addAlarm(mask, v.alarmLow);
        if (v.value > v.max) addAlarm(mask, v.alarmHigh);
    }
    return mask;
}

// Reporting + decision
int vitalsOk(const std::vector<Vital>& vitals) {
    AlarmMask mask = evaluateVitals(vitals);
    if (mask == ALARM_NONE) return 1;

    for (const auto& v : vitals) {
        if (isAlarmSet(mask, v.alarmLow)) {
            flashAlert(v.name + " is too low!");
        }
        if (isAlarmSet(mask, v.alarmHigh)) {
            flashAlert(v.name + " is too high!");
        }
    }
    return 0;
}
