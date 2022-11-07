
#include <SoftwareSerial.h>
//                       RX    TX
HardwareSerial mySerial3(PA10,PA9); // compiles


void setup() {
mySerial3.begin(115200);
}

void loop() {
  mySerial3.println("Hello, world3?");
  delay(1000);  //delay 1 detik
}

