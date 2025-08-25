#ifndef ALERTS_H
#define ALERTS_H

#include "vitals.h"
#include <string>

void flashAlert(const std::string& message);
void setTestMode(bool enabled);
int vitalsOk(float temperature, float pulseRate, float spo2, float respirationRate);

#endif // ALERTS_H
