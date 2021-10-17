// Include the AccelStepper library:
#include <AccelStepper.h>
// Library for LCD
#include <LiquidCrystal_I2C.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 3
#define motorInterfaceType 1
#define mlPerStep 0.000909091

// Define Button Input Interface
#define fastBackward 12
#define backward 11
#define toggle 10
#define forward 9
#define fastForward 8

// Used for button delay listening
unsigned long time = millis();

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

// I2C address 0x27, 16 column and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

int currentSpeed = 0;

void updateLCD(){
  lcd.clear();
  lcd.print("steps: ");
  lcd.print(currentSpeed);
  lcd.setCursor(0,1);
  lcd.print("ml/min: ");
  lcd.print(currentSpeed * mlPerStep * 60);
}

// Change stepper speed
void changeSpeed(int _steps){
  if(_steps == 0){
    stepper.setSpeed(_steps);
  }else{
    stepper.setSpeed(currentSpeed + _steps);  
  }
  currentSpeed = stepper.speed();
  updateLCD();
}

// Listen for pressed buttons
void checkInput(){
  if(millis()-time > 150){
    if(digitalRead(fastBackward)){
      changeSpeed(-100);
    }
    if(digitalRead(backward)){
      changeSpeed(-1);
    }
    if(digitalRead(toggle)){
      changeSpeed(0);
    }
    if(digitalRead(forward)){
      changeSpeed(1);
    }
    if(digitalRead(fastForward)){
      changeSpeed(100);
    }
    time = millis();
  }
}

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  updateLCD();
  
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(10000);
  // Set the speed in steps per second:
  stepper.setSpeed(currentSpeed);
}

void loop() {
  // Listen for input
  checkInput(); 
  // Step the motor with a constant speed as set by setSpeed():
  stepper.runSpeed();
}
