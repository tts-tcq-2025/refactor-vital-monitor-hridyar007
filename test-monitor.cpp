// test-monitor.cpp
#include "monitor.h"
#include <cassert>

int main() {
    setTestMode(true);
    AlarmMask mask = evaluateVitals(98.6f, 75.0f, 98.0f);
    assert(mask == ALARM_NONE);
    return 0;
}
