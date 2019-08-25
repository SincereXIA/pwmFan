//
// Created by SincereXIA on 2019/8/24.
//

#ifndef FAN_FAN_H
#define FAN_FAN_H
#define CMD_RESULT_LEN 1024

#include <wiringPi.h>
typedef enum tpFanMODULE {
    AUTO_CONTROL,
    QUITE,
    POWER_MODULE,
    STOP,
}FAN_MODULE;

class fan {
    static int PIN;
    static int PMW_RANGE;
    static FAN_MODULE MODULE;
    static int PIN_MODE;
    static int _SPEED_NOW_;
    void static RunAutoControl();
    static int SetPinMode(int mode);
public:
    static int init(FAN_MODULE module = AUTO_CONTROL, int controlPin = 24);
    static int SetFanSpeed(int speedPercent);
    static int SetFanModule(FAN_MODULE module);
    static double GetTemp();
    static int Stop();
    static int Log(const char * log);
};


#endif //FAN_FAN_H
