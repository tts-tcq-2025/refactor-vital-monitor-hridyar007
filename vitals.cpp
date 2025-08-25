#include "vitals.h"

struct VitalData {
    float value;
    float low;
    float high;
    AlarmMask lowAlarm;
    AlarmMask highAlarm;
};

// Pure helper
static AlarmMask checkVital(const VitalData& v) {
    AlarmMask mask = ALARM_NONE;
    mask |= (v.value < v.low) ? v.lowAlarm : ALARM_NONE;
    mask |= (v.value > v.high) ? v.highAlarm : ALARM_NONE;
    return mask;
}

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
