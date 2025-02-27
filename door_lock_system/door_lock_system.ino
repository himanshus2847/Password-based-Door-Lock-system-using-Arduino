#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Servo.h>

// LCD pin configuration
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {10, 11, A5, A4}; 
byte colPins[COLS] = {A3, A2, A1, A0}; 

// Initialize keypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// Servo and buzzer pins
const int servoPin = 12;
const int buzzerPin = 13;

// Create servo object
Servo doorLock;

// Password configuration
const String correctPassword = "1234"; // Change this to your desired password
String enteredPassword = "";
const int maxAttempts = 3;
int attempts = 0;
bool doorLocked = true;

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Enter Password:");
  
  // Initialize servo
  doorLock.attach(servoPin);
  doorLock.write(88); // Lock position
  
  // Initialize buzzer
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  char customKey = customKeypad.getKey();
  
  if (customKey) {
    if (customKey == '#') {
      checkPassword();
    }
    else if (customKey == '*') {
      resetSystem();
    }
    else {
      // Add pressed key to password
      if (enteredPassword.length() < 4) {
        enteredPassword += customKey;
        updateLCDPassword();
      }
    }
  }
  
  // Lock the system after 3 wrong attempts
  if (attempts >= maxAttempts) {
    lcd.clear();
    lcd.print("System Locked!");
    lcd.setCursor(0, 1);
    lcd.print("Reset needed!");
    soundBuzzer();
    delay(5000);
    resetSystem();
  }
}

void checkPassword() {
  if (enteredPassword == correctPassword) {
    lcd.clear();
    lcd.print("Access Granted!");
    doorLocked = false;
    doorLock.write(45); // rotate the motor counter-clockwise
    delay(500);
    doorLock.write(88);
    delay(1000);
    doorLock.write(135); // rotate the motor counter-clockwise
    delay(500);
    doorLock.write(88);
    delay(1000);
    attempts = 0;
  }
  else {
    lcd.clear();
    lcd.print("Wrong Password!");
    attempts++;
    soundBuzzer();
    delay(1000);
  }
  enteredPassword = "";
  updateLCDPassword();
}

void resetSystem() {
  enteredPassword = "";
  attempts = 0;
  doorLocked = true;
  doorLock.write(88); // Lock position
  updateLCDPassword();
}

void updateLCDPassword() {
  lcd.clear();
  lcd.print("Enter Password:");
  lcd.setCursor(0, 1);
  for (int i = 0; i < enteredPassword.length(); i++) {
    lcd.print("*");
  }
}

void soundBuzzer() {
  digitalWrite(buzzerPin, HIGH);
  delay(500);
  digitalWrite(buzzerPin, LOW);
}