#include <Wire.h>
#include <ZX_Sensor.h>

// Constants
const int BAUD_RATE = 9600;
const int ZX_ADDR = 0x10;    // ZX Sensor I2C address
const int INTERRUPT_NUM = 0; // Pin 2 on the UNO
const String CHARS[52] = {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
                          "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",
                          "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
                          "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"};

// Global Variables
ZX_Sensor zxSensor = ZX_Sensor(ZX_ADDR);
volatile GestureType gesture;
volatile bool isInterrupted;
uint8_t gestureSpeed;

void setup() {
  uint8_t ver;

  // Initialize gesture to no gesture
  gesture = NO_GESTURE;

  // Initialize Serial port
  Serial.begin(BAUD_RATE);

  // Initialize ZX Sensor (configure I2C and read model ID)
  if (zxSensor.init(GESTURE_INTERRUPTS)) {
//    Serial.print("ZX Sensor initialization complete.;");
  } else {
//    Serial.print("Something went wrong during ZX Sensor init!;");
  }

  // Read the model version number and ensure the library will work
  ver = zxSensor.getModelVersion();
  if (ver == ZX_ERROR) {
//    Serial.print("Error reading model version number;");
  } else {
//    Serial.print("Model version: ");
//    Serial.print(ver);
//    Serial.print(";");
  }
  if (ver != ZX_MODEL_VER) {
//    Serial.print("Model version needs to be ");
//    Serial.print(ZX_MODEL_VER);
//    Serial.print(" to work with this library. Stopping.;");
    while (1);
  }

  // Read the register map version and ensure the library will work
  ver = zxSensor.getRegMapVersion();
  if (ver == ZX_ERROR) {
//    Serial.print("Error reading register map version number.;");
  } else {
//    Serial.print("Register Map Version: ");
//    Serial.print(ver);
//    Serial.print(";");
  }
  if (ver != ZX_REG_MAP_VER) {
//    Serial.print("Register map version needs to be ");
//    Serial.print(ZX_REG_MAP_VER);
//    Serial.print(" to work with this library. Stopping.;");
    while (1);
  }

  // Initialize interrupt service routine
  isInterrupted = false;
  zxSensor.clearInterrupt();
  attachInterrupt(INTERRUPT_NUM, interruptRoutine, RISING);
//  Serial.print("Gesture sensor ready!;");
}

void loop() {
  // If we have an interrupt, read and print the gesture
  if (isInterrupted) {

    // Clear the interrupt flag
    isInterrupted = false;

    // You MUST read the STATUS register to clear interrupt!
    zxSensor.clearInterrupt();

    // Read last gesture
    gesture = zxSensor.readGesture();

    // Get speed. Note: higher speed numbers mean slower
    gestureSpeed = zxSensor.readGestureSpeed();

    switch (gesture) {
      case NO_GESTURE:
        sendGesture("NONE", 0);
        break;
      case RIGHT_SWIPE:
        sendGesture("RIGHT", gestureSpeed);
        break;
      case LEFT_SWIPE:
        sendGesture("LEFT", gestureSpeed);
        break;
      case UP_SWIPE:
        sendGesture("UP", gestureSpeed);
        break;
      default:
        break;
    }
  }
}

void sendGesture(String gestureDirection, int gestureSpeed) {
  String id = createID();
  Serial.print(id);
  Serial.print(",");
  Serial.print(gestureDirection);
  Serial.print(",");
  Serial.print(gestureSpeed, DEC);
  Serial.print(";");
}

String createID() {
  String id = "";
  int ID_LENGTH = 4;
  int index;
  for (int i = 0; i < ID_LENGTH; i++) {
    index = random(52);
    id += CHARS[index];
  }
  return id;
}

void interruptRoutine() {
  isInterrupted = true;
}
