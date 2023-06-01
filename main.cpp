#include "mbed.h"
#include "bbcar.h"
#include <cstdio>

 #define CENTER_BASE 1500
  #define unitsFC 360                          // Units in a full circle
  #define dutyScale 1000                       // Scale duty cycle to 1/000ths
  #define dcMin 29                             // Minimum duty cycle
  #define dcMax 971                            // Maximum duty cycle
  #define q2min unitsFC/4                      // For checking if in 1st uadrant
  #define q3max q2min * 3                      // For checking if in 4th uadrant


Ticker servo_ticker;
Ticker servo_feedback_ticker;

PwmIn servo0_f(D9), servo1_f(D10);
PwmOut servo0_c(D11), servo1_c(D12);
BBCar car(servo0_c, servo0_f, servo1_c, servo1_f, servo_ticker, servo_feedback_ticker);

BusInOut qti_pin(D4,D5,D6,D7);


int main() {

    parallax_qti qti1(qti_pin);
    int pattern;
    double initAngle0;
    double initAngle1;

    int start = 0;
    while(1) {
        pattern = (int)qti1;
        printf("%d\n",pattern);
        if (pattern == 0b1111) {
            start = 1;
            initAngle0 = car.servo0.angle;
            initAngle1 = car.servo1.angle; 
            break;
        }
        ThisThread::sleep_for(10ms);
    }
    car.goStraight(30);
    wait_us(2000000);
    while(1) {
      int test = (int)qti1;
      if (test != 0b0000) {  
         pattern = (int)qti1;
      }
      printf("%d\n",pattern);

      switch (pattern) {
         case 0b1000: car.turn(32, 0.1); break;
         case 0b1100: car.turn(32, 0.3); break;
         case 0b1110: car.turn(32, 0.3); break;
         case 0b0100: car.turn(32, 0.5); break;
         case 0b0110: car.goStraight(32); break;
         case 0b0010: car.turn(32, -0.5); break;
         case 0b0111: car.turn(32, -0.3); break;
         case 0b0011: car.turn(32, -0.3); break;
         case 0b0001: car.turn(32, -0.1); break;
         case 0b1111: car.stop(); break;
         default: car.goStraight(32);
      }
      ThisThread::sleep_for(5ms);
      if (pattern == 0b1111) break;
   }
   double distance1 = abs((float)(car.servo0.turns)*6.5*3.14);
   double distance2 = abs((float)(car.servo1.turns)*6.5*3.14);
   printf("distance = %lf\n", (distance1+distance2)/2);
}
