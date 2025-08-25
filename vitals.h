#ifndef VITALS_H
#define VITALS_H

#include <cstdint>

using AlarmMask = uint8_t;

// Alarm bit masks
constexpr AlarmMask ALARM_NONE              = 0;
constexpr AlarmMask ALARM_TEMPERATURE_LOW   = 1 << 0;
constexpr AlarmMask ALARM_TEMPERATURE_HIGH  = 1 << 1;
constexpr AlarmMask ALARM_PULSE_LOW         = 1 << 2;
constexpr AlarmMask ALARM_PULSE_HIGH        = 1 << 3;
constexpr AlarmMask ALARM_SPO2_LOW          = 1 << 4;
constexpr AlarmMask ALARM_RESP_LOW          = 1 << 5;
constexpr AlarmMask ALARM_RESP_HIGH         = 1 << 6;

// Pure logic functions
AlarmMask evaluateVitals(float temperature, float pulseRate, float spo2, float respirationRate);

inline bool isAlarmSet(AlarmMask mask, AlarmMask alarmBit) {
    return (mask & alarmBit) != 0;
}
inline void addAlarm(AlarmMask& mask, AlarmMask alarmBit) {
    mask |= alarmBit;
}

#endif // VITALS_H
