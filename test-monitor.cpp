#include "monitor.h"
#include <iostream>
#include <cassert>

void check(const char* name,
           std::vector<Vital> vitals,
           AlarmMask expected) {
    AlarmMask result = evaluateVitals(vitals);
    int ok = vitalsOk(vitals);
    bool mask_ok = (result == expected);
    bool return_ok = ((expected == ALARM_NONE && ok == 1) ||
                      (expected != ALARM_NONE && ok == 0));

    std::cout << "Test: " << name << " -> ";
    if (mask_ok && return_ok) {
        std::cout << "PASS\n";
    } else {
        std::cout << "FAIL (mask=" << int(result) << ", vitalsOk=" << ok << ")\n";
    }
}

int main() {
    setTestMode(true);

    // Define baseline vitals
    Vital temp  = {"Temperature", 95.0f, 102.0f, 98.6f, ALARM_TEMPERATURE_LOW, ALARM_TEMPERATURE_HIGH};
    Vital pulse = {"Pulse Rate",  60.0f, 100.0f, 75.0f, ALARM_PULSE_LOW,       ALARM_PULSE_HIGH};
    Vital spo2  = {"SpO2",        90.0f, 100.0f, 98.0f, ALARM_SPO2_LOW,        ALARM_NONE};
    Vital resp  = {"Respiration", 12.0f, 20.0f, 16.0f, ALARM_RESP_LOW,         ALARM_RESP_HIGH};

    check("All normal", {temp, pulse, spo2, resp}, ALARM_NONE);

    // Individual alarms
    check("Temp low",   {Vital{temp.name, temp.min, temp.max, 94.0f, temp.alarmLow, temp.alarmHigh}, pulse, spo2, resp}, ALARM_TEMPERATURE_LOW);
    check("Temp high",  {Vital{temp.name, temp.min, temp.max, 103.0f,temp.alarmLow, temp.alarmHigh}, pulse, spo2, resp}, ALARM_TEMPERATURE_HIGH);
    check("Pulse low",  {temp, Vital{pulse.name,pulse.min,pulse.max,55.0f,pulse.alarmLow,pulse.alarmHigh}, spo2, resp}, ALARM_PULSE_LOW);
    check("Pulse high", {temp, Vital{pulse.name,pulse.min,pulse.max,105.0f,pulse.alarmLow,pulse.alarmHigh}, spo2, resp}, ALARM_PULSE_HIGH);
    check("SpO2 low",   {temp, pulse, Vital{spo2.name,spo2.min,spo2.max,85.0f,spo2.alarmLow,spo2.alarmHigh}, resp}, ALARM_SPO2_LOW);
    check("Resp low",   {temp, pulse, spo2, Vital{resp.name,resp.min,resp.max,8.0f,resp.alarmLow,resp.alarmHigh}}, ALARM_RESP_LOW);
    check("Resp high",  {temp, pulse, spo2, Vital{resp.name,resp.min,resp.max,25.0f,resp.alarmLow,resp.alarmHigh}}, ALARM_RESP_HIGH);

    // Multiple alarms
    check("Multiple failures",
          {Vital{temp.name,temp.min,temp.max,103.0f,temp.alarmLow,temp.alarmHigh},
           Vital{pulse.name,pulse.min,pulse.max,50.0f,pulse.alarmLow,pulse.alarmHigh},
           Vital{spo2.name,spo2.min,spo2.max,85.0f,spo2.alarmLow,spo2.alarmHigh},
           Vital{resp.name,resp.min,resp.max,25.0f,resp.alarmLow,resp.alarmHigh}},
          static_cast<AlarmMask>(ALARM_TEMPERATURE_HIGH |
                                 ALARM_PULSE_LOW |
                                 ALARM_SPO2_LOW |
                                 ALARM_RESP_HIGH));

    // Edge cases: exactly at threshold → should be OK
    check("At lower bound", {Vital{temp.name,temp.min,temp.max,95.0f,temp.alarmLow,temp.alarmHigh},
                             Vital{pulse.name,pulse.min,pulse.max,60.0f,pulse.alarmLow,pulse.alarmHigh},
                             Vital{spo2.name,spo2.min,spo2.max,90.0f,spo2.alarmLow,spo2.alarmHigh},
                             Vital{resp.name,resp.min,resp.max,12.0f,resp.alarmLow,resp.alarmHigh}},
                             ALARM_NONE);
    check("At upper bound", {Vital{temp.name,temp.min,temp.max,102.0f,temp.alarmLow,temp.alarmHigh},
                             Vital{pulse.name,pulse.min,pulse.max,100.0f,pulse.alarmLow,pulse.alarmHigh},
                             spo2,
                             Vital{resp.name,resp.min,resp.max,20.0f,resp.alarmLow,resp.alarmHigh}},
                             ALARM_NONE);

    return 0;
}
