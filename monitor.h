#ifndef MONITOR_H
#define MONITOR_H

#include <cstdint>
#include <string>
#include <vector>

// Alarm bit masks
constexpr uint8_t ALARM_NONE              = 0;
constexpr uint8_t ALARM_TEMPERATURE_LOW   = 1 << 0;
constexpr uint8_t ALARM_TEMPERATURE_HIGH  = 1 << 1;
constexpr uint8_t ALARM_PULSE_LOW         = 1 << 2;
constexpr uint8_t ALARM_PULSE_HIGH        = 1 << 3;
constexpr uint8_t ALARM_SPO2_LOW          = 1 << 4;
constexpr uint8_t ALARM_RESP_LOW          = 1 << 5;
constexpr uint8_t ALARM_RESP_HIGH         = 1 << 6;

using AlarmMask = uint8_t;

// Structure describing a single vital
struct Vital {
    std::string name;
    float min;
    float max;
    float value;
    AlarmMask alarmLow;
    AlarmMask alarmHigh;
};

// API
AlarmMask evaluateVitals(const std::vector<Vital>& vitals);
int vitalsOk(const std::vector<Vital>& vitals);
void setTestMode(bool enabled);

static inline bool isAlarmSet(AlarmMask mask, AlarmMask alarmBit) {
    return (mask & alarmBit) != 0;
}
static inline void addAlarm(AlarmMask &mask, AlarmMask alarmBit) {
    mask |= alarmBit;
}

#endif // MONITOR_H
