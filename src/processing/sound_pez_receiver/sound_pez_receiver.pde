import processing.sound.*;
import processing.serial.*;


// constants
String SOUNDS_DIR = "/Volumes/Documents/Projects/sound-pez-2/assets/sounds/";
String SWELLS_DIR = SOUNDS_DIR + "major-scale-swells/";

// variables
Serial port;
String portData;
String messageId = "";
AudioDevice device;
SoundFile[] swells;


// 
void setup() {
  initPort();
  initAudio();
}

void draw() {
  if (port.available() > 0) {
    portData = port.readStringUntil(';');
  }
  if (portData != null) {
    portData = portData.replaceFirst(";", ""); 
    String data[] = split(portData, ",");
    String receivedId = data[0];    
    if(receivedId.equals(messageId) == false) {
      messageId = data[0];
      println("New gesture! (" + messageId + ")");
      String gestureDirection = data[1];
      println("gesture direction: " +gestureDirection);
      int gestureSpeed = int(data[2]);
      println("gesture speed: " + gestureSpeed);
      playRandomSwell(getSoundRate(gestureSpeed));
    }
  }
}


// methods definitions
void initPort() {
  // change to match port
  String portName = Serial.list()[2];
  // printArray(Serial.list());
  // println(portName);
  port = new Serial(this, portName, 9600);
}
  
void initAudio() {
  device = new AudioDevice(this, 48000, 32);
  loadSwells();
}

void loadSwells() {
  swells = new SoundFile[8];
  for (int i = 0; i < swells.length; i++) {
    String filename = SWELLS_DIR + "0" + (i + 1) + ".wav";
    swells[i] = new SoundFile(this, filename);
  }
}

void playRandomSwell(float rate) {
  int index = int(random(8));
  swells[index].play(rate, 1.0);
}

float getSoundRate(int gestureSpeed) {
  float rate;
  if (gestureSpeed > 30) {
    rate = 2.0;
  } else if (gestureSpeed > 15) {
    rate = 1.0;
  } else {
    rate = 0.5;
  }
  return rate;
}