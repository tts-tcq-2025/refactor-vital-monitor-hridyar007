#ifndef MONITOR_H
#define MONITOR_H

bool isInRange(float value, float min, float max);
bool temperatureOk(float temperature);
bool pulseRateOk(int pulseRate);
bool spo2Ok(int spo2);
bool vitalsOk(float temperature, int pulseRate, int spo2);

#endif
