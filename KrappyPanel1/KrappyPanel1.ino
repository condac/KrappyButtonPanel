// Mode 1. Single arduino for all buttons, 2 rotary and 5 analog.
// Buttons are done with a button matrix, see scematics: !TODO!


//#define GYRO_STEERING
//#define ROTARY_ENCODER
#include <Joystick.h>

#include <Arduino.h>
#ifdef GYRO_STEERING
#include <TinyMPU6050.h>
#endif
#define REV_STEERING 1
#define UPSIDEDOWN 0


long shiftPulse = 50;

bool setupMode = false;
/*
 *  Constructing MPU-6050
 */


Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID+2,JOYSTICK_TYPE_MULTI_AXIS,
  32, 0,                  // Button Count, Hat Switch Count
  false, false, true,     // X and Y, but no Z Axis
  true, true, true,   // No Rx, Ry, or Rz
  false, false,          // No rudder or throttle
  false, false, false);  // No accelerator, brake, or steering

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
//  pinMode(0,INPUT_PULLUP);
//  pinMode(1,INPUT_PULLUP);
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  pinMode(8,INPUT_PULLUP);
  pinMode(9,INPUT_PULLUP);
  pinMode(10,INPUT_PULLUP);
  pinMode(11,INPUT_PULLUP);
  pinMode(12,INPUT_PULLUP);
  pinMode(13,INPUT_PULLUP);
  
  Serial.println("started..");
  delay(1000);
  Serial.print("1");
  //delay(1000); 
  //Serial.print("2");
  //delay(1000); 
  //Serial.print("3");
  //delay(1000); 
  //Serial.print("4");
  //delay(1000); 
  //Serial.print("5");
  // wait 5s so we can reprogram device incase of flooding on serial port
    // Initialize Joystick Library
  // Initialization



  Joystick.begin(false);
  //Joystick.setXAxisRange(-32767, 32767);
  
  Joystick.setXAxisRange(-512, 512);
  Joystick.setYAxisRange(-512, 512);
  Joystick.setZAxisRange(-512, 512);
  
  Joystick.setRxAxisRange(-512, 512);
  Joystick.setRyAxisRange(-512, 512);
  Joystick.setRzAxisRange(-512, 512);
}


long counter = 0;
long time1 = 0;
int button = 0; 
bool currentButtonState;
double prevang = 0.0;
int turns = 0;
int multiValue;

void loop() {
  mode1();
  
}



bool handleButton(int pin, int buttonNr, bool reverse, long pulse) {
  bool state;

    state = false;
    state = digitalRead(pin);
    if (reverse) {
      state = !state;
    }

    Joystick.setButton(buttonNr, state);
  
  return state;
}

void handleMultiWheelButton(int aPort, int button1, int button2, int startButton, int sections) {
  int value = analogRead(aPort);
  sections = sections;
  if(sections == 0) {
    sections = 1;
  }
  int divider = 1024/sections;
  //divider = divider + 1;
  value = value/divider;
  multiValue = value;
  handleButton(button1, value*2+startButton, false, shiftPulse);
  handleButton(button2, value*2+startButton+1, false, shiftPulse);
}

int getPosition(int aPort, int sections) {
  int divider = 1023/sections;
  divider = divider +1;
  int val = analogRead(aPort);
  int out = 0;
  int test = divider;
  //Serial.print("val");
  //Serial.print(val);
  
  //Serial.print("test");
  
  //Serial.println(test);
  while (val > test) {
    

    test = test + divider;
    out = out + 1;
  //    Serial.print("val");
  //Serial.print(val);
  //Serial.print("test");
  //Serial.println(test);
  }
  
  //Serial.print("getpos return:");
  
  //Serial.println(out);
  return out;
}

long rotarySticky = 0;
long stick = 15;
int ERSlast[2];
long ERSsticky[2];
int ERScycle[2];
long ERSstick = 30;
long ERSpause = 250;


bool falseOnce = false;


void CHJoystickButtonMatrix() {
  byte byte0 = digitalRead(2);
  byte byte1 = digitalRead(3);
  byte byte2 = digitalRead(4);
  byte byte3 = digitalRead(5);

  byte total = 0;
  bitWrite(total, 3, byte3);
  bitWrite(total, 2, byte2);
  bitWrite(total, 1, byte1);
  bitWrite(total, 0, byte0);

  for (int i=0;i<16;i++) {
    Joystick.setButton(i, 0);
  }
  if (total !=15) {
    Joystick.setButton(total, 1);
  }

}

bool hej;
long timehej;
void mode1() {
  //CHJoystickButtonMatrix();
  // read 2 gear shifters
  handleButton(2,0,false, shiftPulse); // modeswitch flip
  handleButton(3,1,true, shiftPulse);  // normal button

  handleButton(4,2,true, shiftPulse);// normal button
  handleButton(5,3,false, shiftPulse); // modeswitch flip
  handleButton(6,4,true, shiftPulse); // normal button 2 way spring
  handleButton(7,5,true, shiftPulse); // normal button 2 way spring
  handleButton(8,6,true, shiftPulse); // normal button 2 way spring
  handleButton(9,7,true, shiftPulse); // normal button 2 way spring
  handleButton(10,8,true, shiftPulse); // normal button 2 way spring
  handleButton(11,9,true, shiftPulse); // normal button 2 way spring
  handleButton(12,10,true, shiftPulse); // normal button 2 way spring
  handleButton(13,11,true, shiftPulse); // normal button 2 way spring

  // read button matrix
 
  
    //Joystick.setButton(100, hej);
   //if (timehej<millis()) {
   // timehej = millis()+1000;
  //hej = !hej;
  //}
  
  // Read 4 analog inputs
  int a0 = analogRead(A0);
  a0 = (a0-512);
  int a1 = analogRead(A1);
  a1 = (a1-512);
  int a2 = analogRead(A2);
  a2 = (a2-512);
  int a3 = analogRead(A3);
  a3 = (a3-512);
  int a4 = analogRead(A4);
  a4 = (a4-512);
  //a1 = -a1;
  //a0 = -a0;
  Joystick.setXAxis(0);
  Joystick.setYAxis(0);
  Joystick.setZAxis(a3);
  Joystick.setRxAxis(a2);
  Joystick.setRyAxis(a1);
  Joystick.setRzAxis(a0);
  // read multiwheel dial

  // send state
  Joystick.sendState();
  counter++;

  if (time1<millis() && setupMode) {
    Serial.println(counter);
    time1 = millis()+200;
    counter = 0;

  }
}
