#include "monitor.h"

static bool g_test_mode = false;

void setTestMode(bool enabled) {
    g_test_mode = enabled;
}

AlarmMask evaluateVitals(float, float, float) {
    return ALARM_NONE; // minimal stub
}

int vitalsOk(float, float, float) {
    return 1; // always OK for now
}
