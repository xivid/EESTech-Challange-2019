
/* in esp26
 *  forward: digitalWrite(19, 1)
 *  backward: digitalWrite(21, 1)
 *  left: digitalWrite(22, 1)
 *  right: digitalWrite(23, 1)
 */
/*  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
 * 
 */
//this is for xms1100
#include <TLE94112.h>
#include <TLE94112_Motor.h>

// The TLE94112 has only got 3 PWM channels
// There is no one left for motor4
// This means, we cannot control its speed
Tle94112Motor motor1(tle94112, tle94112.TLE_NOPWM);
Tle94112Motor motor2(tle94112, tle94112.TLE_NOPWM);
//Tle94112Motor motor3(tle94112, tle94112.TLE_PWM2);
//Tle94112Motor motor4(tle94112, tle94112.TLE_PWM2);

#define GPIO1 7
#define GPIO2 9
#define GPIO3 2
#define GPIO4 5

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  pinMode(GPIO1, INPUT);
  pinMode(GPIO2, INPUT);
  pinMode(GPIO3, INPUT);
  pinMode(GPIO4, INPUT);
  
  tle94112.begin();
  
  motor1.connect(tle94112.TLE_HB1, motor1.HIGHSIDE);
  motor1.connect(tle94112.TLE_HB2, motor1.LOWSIDE);
  motor1.connect(tle94112.TLE_HB3, motor1.HIGHSIDE);
  motor1.connect(tle94112.TLE_HB4, motor1.LOWSIDE);
  
  motor2.connect(tle94112.TLE_HB6, motor2.HIGHSIDE);
  motor2.connect(tle94112.TLE_HB5, motor2.LOWSIDE);
  motor2.connect(tle94112.TLE_HB8, motor2.HIGHSIDE);
  motor2.connect(tle94112.TLE_HB7, motor2.LOWSIDE);

  //change the motors PWM frequency (just in case you will ever need it...)
  motor1.setPwmFreq(tle94112.TLE_FREQ80HZ);
  motor2.setPwmFreq(tle94112.TLE_FREQ80HZ);
 // motor3.setPwmFreq(tle94112.TLE_FREQ80HZ);
 // motor4.setPwmFreq(tle94112.TLE_FREQ80HZ);


  //when configuration is done, call begin to start operating the motors
  motor1.begin();
  motor2.begin();
//  motor3.begin();
//  motor4.begin();
}

void forward(){
//    motor1.stop();
//  motor2.stop();
  motor1.start(127);
  motor2.start(127);
}
void backward(){
    //backward
//      motor1.stop();
//  motor2.stop();
  motor1.start(-127);
  motor2.start(-127);
}
void left(){
//    motor1.stop();
//  motor2.stop();
  motor1.start(0);
  motor2.start(127);
}
void right(){
//    motor1.stop();
//  motor2.stop();
  motor1.start(127);
  motor2.start(0);
}
void fandl(){
  motor1.stop();
  motor1.start(60);
  motor1.setSpeed(60);
  motor2.start(127);
}
void fandr(){
  motor1.start(127);
  motor2.stop();
  motor2.start(60);
  motor2.setSpeed(60);

}
void bandl(){
  motor1.stop();
  motor2.stop();

  motor1.start(-60);
  motor1.setSpeed(-60);
  motor2.start(-127);
}
void bandr(){
  motor1.stop();
  motor2.stop();
  motor1.start(-127);
  motor2.start(-60);
  motor2.setSpeed(-60);
}
void stopmotor(){
  motor1.stop();
  motor2.stop();
}

void loop() {
  Serial.print("read 1: "); Serial.print(digitalRead(GPIO1));
  Serial.print(" read 2: "); Serial.print(digitalRead(GPIO2));
  Serial.print(" read 3: "); Serial.print(digitalRead(GPIO3));
  Serial.print(" read 4: "); Serial.println(digitalRead(GPIO4));
  // put your main code here, to run repeatedly:
  if(digitalRead(GPIO1) == 1) {
    forward();  // control back
    if (digitalRead(GPIO3) == 1) {
      left();  // control front
    } else if (digitalRead(GPIO4) == 1) {
      right();  // control front
    }
    return;
  }
  
  if(digitalRead(GPIO2) == 1){
    backward();
    if (digitalRead(GPIO3) == 1) {
      left();  // control front
    } else if (digitalRead(GPIO4) == 1) {
      right();  // control front
    }
    return;
  }
  
  if(digitalRead(GPIO3) == 1){
    left();
    return;
  }
  
  if(digitalRead(GPIO4) == 1){
    right();
    return;
  }
  
  stopmotor();
}
