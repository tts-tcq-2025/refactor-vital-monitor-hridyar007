#ifndef MONITOR_H
#define MONITOR_H

#include "vitals.h"
#include "alerts.h"

// Public API
using AlarmMask = ::AlarmMask;

// Wrapper to evaluate vitals (logic + optional alerts)
inline AlarmMask checkVitals(float temperature, float pulseRate, float spo2, float respirationRate) {
    return evaluateVitals(temperature, pulseRate, spo2, respirationRate);
}

// Wrapper to check vitals and trigger alerts
inline int checkVitalsWithAlerts(float temperature, float pulseRate, float spo2, float respirationRate) {
    return vitalsOk(temperature, pulseRate, spo2, respirationRate);
}

// Expose test mode control
inline void enableTestMode(bool enabled) {
    setTestMode(enabled);
}

#endif // MONITOR_H
