#ifndef ALERTS_H
#define ALERTS_H

#include "vitals.h"
#include <string>
#include <vector>

// Side-effect functions
void flashAlert(const std::string& message);
void setTestMode(bool enabled);

// Logic separated from I/O
std::vector<std::string> getAlarmMessages(AlarmMask mask);
void showAlarms(AlarmMask mask);

// Combined check + alert
int vitalsOk(float temperature, float pulseRate, float spo2, float respirationRate);

#endif // ALERTS_H
