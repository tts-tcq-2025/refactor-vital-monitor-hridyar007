#include "alerts.h"
#include <iostream>
#include <thread>
#include <chrono>

static bool g_test_mode = false;

void setTestMode(bool enabled) { g_test_mode = enabled; }

static const char* alarmMessages[] = {
    "Temperature is too low!",
    "Temperature is critical!",
    "Pulse Rate is too low!",
    "Pulse Rate is too high!",
    "Oxygen Saturation out of range!",
    "Respiration Rate is too low!",
    "Respiration Rate is too high!"
};

// Side-effect function
void flashAlert(const std::string& msg) {
    std::cout << msg << "\n";
    if (g_test_mode) return;
    for (int i = 0; i < 6; ++i) {
        std::cout << "\r* " << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "\r *" << std::flush;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    std::cout << "\r  \n";
}

// Wrapper for alerts
int vitalsOk(float temperature, float pulseRate, float spo2, float respirationRate) {
    AlarmMask mask = evaluateVitals(temperature, pulseRate, spo2, respirationRate);
    if (mask == ALARM_NONE) return 1;

    for (int i = 0; i < 7; i++) {
        if (isAlarmSet(mask, 1 << i)) flashAlert(alarmMessages[i]);
    }
    return 0;
}
