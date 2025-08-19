#ifndef MONITOR_H
#define MONITOR_H

#include <cstdint>

// Alarm bit masks
constexpr uint8_t ALARM_NONE              = 0;
constexpr uint8_t ALARM_TEMPERATURE_LOW   = 1 << 0; // bit 0
constexpr uint8_t ALARM_TEMPERATURE_HIGH  = 1 << 1; // bit 1
constexpr uint8_t ALARM_PULSE_LOW         = 1 << 2; // bit 2
constexpr uint8_t ALARM_PULSE_HIGH        = 1 << 3; // bit 3
constexpr uint8_t ALARM_SPO2_LOW          = 1 << 4; // bit 4

using AlarmMask = uint8_t;

// Evaluate patient vitals and return alarm mask
AlarmMask evaluateVitals(float temperature, float pulseRate, float spo2);

// Returns 1 if vitals are OK, 0 if alarms are triggered.
// Prints alert messages when alarms occur.
int vitalsOk(float temperature, float pulseRate, float spo2);

// Enable/disable test mode (skips delays in flashing alerts).
void setTestMode(bool enabled);

// Helpers
static inline bool isAlarmSet(AlarmMask mask, AlarmMask alarmBit) {
    return (mask & alarmBit) != 0;
}

static inline void addAlarm(AlarmMask &mask, AlarmMask alarmBit) {
    mask |= alarmBit;
}

#endif // MONITOR_H
