// from https://learn.sparkfun.com/tutorials/connecting-arduino-to-processing

void setup() {
  // initialize serial communications at a 9600 baud rate
  Serial.begin(9600);
}

void loop() {
  // send 'Hello, world!' over the serial port
  Serial.println("Hello, world!");
  // wait so we don't drive ourselves crazy
  delay(250);
}
