#include <iostream>
#include "fan.h"
#include <thread>
#include <wiringPi.h>
#include <time.h>

using namespace std;

int threadTest()
{
    wiringPiSetup();
    pinMode(7, OUTPUT);
    for (int i= 0; i<10; i++){
        digitalWrite(7, LOW);
        this_thread::sleep_for(chrono::seconds(1));
        digitalWrite(7, HIGH);
        this_thread::sleep_for(chrono::seconds(1));
    }
    return 0;
}

int test() {
    thread t(threadTest);
    t.detach();
    cout << "thread Start" << endl;
    return 0;
}

int main() {
    fan::init(AUTO_CONTROL);
    while (1) {
        this_thread::sleep_for(chrono::seconds(100));
    }
}


