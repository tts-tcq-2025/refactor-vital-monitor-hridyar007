#include "monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

static bool g_test_mode = false;

// Thresholds
const float TEMP_LOW   = 95.0f;
const float TEMP_HIGH  = 102.0f;
const float PULSE_LOW  = 60.0f;
const float PULSE_HIGH = 100.0f;
const float SPO2_LOW   = 90.0f;
const float RESP_LOW   = 12.0f;   // breaths/min
const float RESP_HIGH  = 20.0f;   // breaths/min

void setTestMode(bool enabled) {
    g_test_mode = enabled;
}

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

AlarmMask evaluateVitals(float temperature, float pulseRate, float spo2, float respirationRate) {
    AlarmMask mask = ALARM_NONE;

    if (temperature < TEMP_LOW) {
        addAlarm(mask, ALARM_TEMPERATURE_LOW);
    } else if (temperature > TEMP_HIGH) {
        addAlarm(mask, ALARM_TEMPERATURE_HIGH);
    }

    if (pulseRate < PULSE_LOW) {
        addAlarm(mask, ALARM_PULSE_LOW);
    } else if (pulseRate > PULSE_HIGH) {
        addAlarm(mask, ALARM_PULSE_HIGH);
    }

    if (spo2 < SPO2_LOW) {
        addAlarm(mask, ALARM_SPO2_LOW);
    }

    if (respirationRate < RESP_LOW) {
        addAlarm(mask, ALARM_RESP_LOW);
    } else if (respirationRate > RESP_HIGH) {
        addAlarm(mask, ALARM_RESP_HIGH);
    }

    return mask;
}

int vitalsOk(float temperature, float pulseRate, float spo2, float respirationRate) {
    AlarmMask mask = evaluateVitals(temperature, pulseRate, spo2, respirationRate);
    if (mask == ALARM_NONE) {
        return 1;
    }

    if (isAlarmSet(mask, ALARM_TEMPERATURE_LOW)) {
        flashAlert("Temperature is too low!");
    }
    if (isAlarmSet(mask, ALARM_TEMPERATURE_HIGH)) {
        flashAlert("Temperature is critical!");
    }
    if (isAlarmSet(mask, ALARM_PULSE_LOW)) {
        flashAlert("Pulse Rate is too low!");
    }
    if (isAlarmSet(mask, ALARM_PULSE_HIGH)) {
        flashAlert("Pulse Rate is too high!");
    }
    if (isAlarmSet(mask, ALARM_SPO2_LOW)) {
        flashAlert("Oxygen Saturation out of range!");
    }
    if (isAlarmSet(mask, ALARM_RESP_LOW)) {
        flashAlert("Respiration Rate is too low!");
    }
    if (isAlarmSet(mask, ALARM_RESP_HIGH)) {
        flashAlert("Respiration Rate is too high!");
    }

    return 0;
}
