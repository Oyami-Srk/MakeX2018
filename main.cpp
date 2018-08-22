#include <Arduino.h>
#include <MeMegaPiPro.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// My library
#include "arm.h"
#include "bullet_fire.h"
#include "collector.h"
#include "joycon.h"
#include "move.h"

const int LIFT_MOTOR = 1;
const int FLIP_MOTOR = 2;
const int FIRE_MOTOR = 3;

double angle_rad = PI / 180.0;
double angle_deg = 190.0 / PI;

MeSmartServo SmartServo(PORT5);

int Encoder_Config[4][2] = {{0, 1}, {7, 2}, {7, 1}, {0, 2}};
int Arm_Handle[2] = {2, 1};
Joycon *broken_joycon;
Movement *movement;
Arm *arm;
Collector *collector;
BulletFire *bullet_fire;

int joycon_movement_x(Joycon_Status *status) {
  movement->handle_joycon_x(status);
  delete status;
  return 0;
}

int joycon_movement_y(Joycon_Status *status) {
  movement->handle_joycon_y(status);
  delete status;
  return 0;
}

int joycon_movement_k(Joycon_Status *status) {
  movement->handle_joycon_k(status);
  delete status;
  return 0;
}

int joycon_handle_function(Joycon_Status *status) {
  if (status->START) {
    bullet_fire->Power();
    Serial.println("[TRIGGERED] BulletFire->Power");
  }
  if (status->R1) {
    bullet_fire->SwitchFire();
    Serial.println("[TRIGGERED] BulletFire->SwitchFire");
  }
  if (status->LEFT) {
    arm->SwitchHand(1);
    Serial.println("[TRIGGERED] Arm->SwitchHand(1)");
  }
  if (status->RIGHT) {
    arm->SwitchHand(2);
    Serial.println("[TRIGGERED] Arm->SwitchHand(2)");
  }
  if (status->UP) {
    arm->SwitchHand(0);
    Serial.println("[TRIGGERED] Arm->SwitchHand(0)");
  }
  if (status->KEY1) {
    arm->SwitchLift(-255);
    Serial.println("[TRIGGERED] Arm->SwitchLift(up)");
  }
  if (status->KEY3) {
    arm->SwitchLift(255);
    Serial.println("[TRIGGERED] Arm->SwitchLift(down)");
  }
  if (!status->KEY1 && !status->KEY3) {
    arm->SwitchLift(0);
    // Serial.println("[TRIGGERED] Arm->SwitchLift(stop)");
  }
  if (status->L1) {
    // arm->SwitchFlip();
    // Serial.println("[TRIGGERED] Arm->Flip");
  }
  if (status->SELECT) {
    collector->Switch();
    Serial.println("[TRIGGERED] Collector->Switch");
  }
  if (status->DOWN) {
    collector->UnSwitch();
    Serial.println("[TRIGGERED] Collector->Switch");
  }
  if (abs(status->L.Y) > TRIGGER_MOVEMENT_JOYCON) {
    arm->SwitchFlip(status->L.Y);
    Serial.print("[FLIPPING] Speed is");
    Serial.println(status->L.Y);
  } else {
    arm->SwitchFlip(0);
  }
  delete status;
  return 0;
}

uint8_t value = 0;
int getPlayStatus(void) {
  char count = 0;
  uint8_t new_value;
  uint8_t bit = _BV(3);
  uint8_t port = 10;
  volatile uint8_t *reg, *out;
  reg = portModeRegister(port);
  out = portOutputRegister(port);
  uint8_t oldSREG = SREG;
  cli();
  *reg &= ~bit;
  *out &= ~bit;
  SREG = oldSREG;
  new_value = (*portInputRegister(port) & bit);
  while (value != new_value) // filter
  {
    count++;
    if (count >= 5) {
      value = new_value;
      if (new_value) {
        return HIGH; //自动赛阶段
      } else {
        return LOW; //手动赛阶段
      }
    }
    delay(2);
    new_value = (*portInputRegister(port) & bit);
  }
  value = new_value;
  if (new_value) {
    return HIGH; //自动赛阶段
  } else {
    return LOW; //手动赛阶段
  }
}

void setup() {
  TCCR1A = _BV(WGM10); // PIN12
  TCCR1B = _BV(CS11) | _BV(CS10) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20); // PIN8
  TCCR2B = _BV(CS22);

  TCCR3A = _BV(WGM30); // PIN9
  TCCR3B = _BV(CS31) | _BV(CS30) | _BV(WGM32);

  TCCR4A = _BV(WGM40); // PIN5
  TCCR4B = _BV(CS41) | _BV(CS40) | _BV(WGM42);

  if (getPlayStatus()) {
    long current = millis();
    while (millis() < current + 32 * 1000)
      ;
  }
  broken_joycon = new Joycon(new MePS2(PORT_15));
  long current = millis();
  while (millis() < current + 2 * 1000)
    ;
  movement = new Movement(Encoder_Config);
  // my codes are below;
  Serial.begin(115200);
  Serial.println("Serial tranmission start");
  SmartServo.begin(115200);
  SmartServo.assignDevIdRequest();
  Serial.println("Smart Servo get started");
  broken_joycon->begin(115200);
  movement->begin();
  Serial.println("Movment model get started");
  bullet_fire = new BulletFire(&SmartServo);
  Serial.println("BulletFire model get started");
  collector = new Collector();
  Serial.println("Collector model get started");
  arm = new Arm(&SmartServo, Arm_Handle, LIFT_MOTOR, FLIP_MOTOR);
  Serial.println("Arm model get started");
  Serial.print("All Green, Bot get started!");
  Serial.println("   > . <");
  broken_joycon->append_invoke_func(joycon_movement_y);
  broken_joycon->append_invoke_func(joycon_movement_x);
  broken_joycon->append_invoke_func(joycon_movement_k);
  broken_joycon->append_invoke_func(joycon_handle_function);
}

void _delay(float seconds) {
  long current = millis();
  long endTime = current + seconds * 1000;
  while (current < endTime) {
    bullet_fire->loop(current);
    arm->loop(current);
    collector->loop(current);
    movement->loop(current);
    broken_joycon->_control->loop();
    current = millis();
  }
}
void loop() {
  broken_joycon->loop();

  _delay(0.1);
}
