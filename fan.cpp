//
// Created by SincereXIA on 2019/8/24.
//

#include "fan.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <wiringPi.h>
#include <iostream>
#include <thread>
#include <string>
using namespace std;

int fan::PIN = 24;
int fan::PMW_RANGE = 1024;
FAN_MODULE fan::MODULE = POWER_MODULE;
int fan::PIN_MODE = INPUT;
int fan::_SPEED_NOW_ = 100;


/*
 * 功能：执行系统命令，并获取命令的返回数据
 * 参数：cmd 要执行的系统命令，result 为接收命令返回的数据
 * 返回值：成功返回0，失败返回-1
 * 注意：执行的命令和返回的数据最大为CMD_RESULT_LEN
 */
static int execute_cmd_result(const char *cmd, char *result)
{
    if (cmd == nullptr || result == nullptr || strlen(cmd) <= 0)
        return -1;
    char buf_ps[CMD_RESULT_LEN] = {0};
    char ps[CMD_RESULT_LEN] = {0};
    FILE *ptr;
    strcpy(ps, cmd);
    if((ptr=popen(ps, "r"))!=nullptr)
    {
        while(fgets(buf_ps, CMD_RESULT_LEN, ptr)!=nullptr)
        {
            strcat(result, buf_ps);
            if(strlen(result)>CMD_RESULT_LEN)
                break;
        }
        pclose(ptr);
    }
    else
    {
        printf("error: [sys.c]popen %s error\n", ps);
        return -1;
    }
    return 0;
}


double fan::GetTemp() {
    char buff[CMD_RESULT_LEN] = {0};
    char * ptr;
    if (execute_cmd_result("cat /sys/class/thermal/thermal_zone0/temp", buff) < 0)
    {
        printf("错误：执行获取cpu温度命令失败！\n");
        return -1;
    }
    double ret = strtod(buff, &ptr)/1000.0;
    return ret;
}

int fan::init(FAN_MODULE module, int controlPin) {
    PIN = controlPin;
    wiringPiSetup();
    pinMode(PIN, OUTPUT);
    digitalWrite(PIN, LOW);
    MODULE = POWER_MODULE;
    PIN_MODE = INPUT;
    _SPEED_NOW_ = 100;
    SetFanModule(module);
    return 0;
}

int fan::SetFanModule(FAN_MODULE module) {
    switch (module) {
        case AUTO_CONTROL:
            if (MODULE != module){
                MODULE = module;
                thread t(RunAutoControl);
                t.detach();
            }
            break;
        case QUITE:
            break;
        case POWER_MODULE:
            break;
        case STOP:
            break;
    }
    return 0;
}

int fan::Log(const char *log) {
    std::cout << log << endl;
    return 0;
}

void fan::RunAutoControl() {
    char logInfo[100];
    while(MODULE == AUTO_CONTROL) {
        double tmp = GetTemp();
        if (tmp < 45.0) {
            SetFanSpeed(0);
            sprintf(logInfo, "Tmp is %lf, set Speed to 0%%", tmp);
            Log(logInfo);
        } else if (tmp < 50.0) {
            SetFanSpeed(20);
            sprintf(logInfo, "Tmp is %lf, set Speed to 20%%", tmp);
            Log(logInfo);
        } else if (tmp < 54.0) {
            SetFanSpeed(50);
            sprintf(logInfo, "Tmp is %lf, set Speed to 50%%", tmp);
            Log(logInfo);
        } else {
            SetFanSpeed(100);
            sprintf(logInfo, "Tmp is %lf, set Speed to 100%%", tmp);
            Log(logInfo);
        }

        this_thread::sleep_for(chrono::seconds(30));
    }
}

int fan::SetFanSpeed(int speedPercent) {
    if (speedPercent == _SPEED_NOW_) {
        return 0;
    }
    if (speedPercent == 0){
        SetPinMode(OUTPUT);
        digitalWrite(PIN, HIGH);
    }else if (speedPercent == 100) {
        SetPinMode(OUTPUT);
        digitalWrite(PIN, LOW);
    }else{
        SetPinMode(PWM_OUTPUT);
        int pmwVal = PMW_RANGE * (100 - speedPercent) / 100;
        cout <<"pmwVaal: "<<pmwVal << endl;
        pwmWrite(PIN, pmwVal);
    }
    _SPEED_NOW_ = speedPercent;
    return 0;
}

int fan::Stop() {
    pinMode(PIN, INPUT);
    pullUpDnControl(PIN, PUD_UP);
    return 0;
}

int fan::SetPinMode(int mode) {
    switch (mode) {
        case INPUT:
            if (PIN_MODE != INPUT) {
                pinMode(PIN, INPUT);
            }
            PIN_MODE = INPUT;
            break;
        case OUTPUT:
            if (PIN_MODE != OUTPUT) {
                pinMode(PIN, OUTPUT);
            }
            PIN_MODE = OUTPUT;
            break;
        case PWM_OUTPUT:
            if (PIN_MODE != PWM_OUTPUT) {
                pinMode(PIN, PWM_OUTPUT);
                pwmSetRange(PMW_RANGE);
            }
            PIN_MODE = PWM_OUTPUT;
            break;
        default:
            Log("Set PIN MODE ERROR");
            break;
    }
    return 0;
}
