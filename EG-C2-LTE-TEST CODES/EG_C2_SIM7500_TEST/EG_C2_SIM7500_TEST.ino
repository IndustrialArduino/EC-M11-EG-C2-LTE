/*
  Multiple Serial test

  Receives from the main serial port, sends to the others.
  Receives from serial port 1, sends to the main serial (Serial 0).

  This example works only with boards with more than one serial like Arduino Mega, Due, Zero, etc.

  The circuit:
  - any serial device attached to Serial port 1
  - Serial Monitor open on Serial port 0

  created 30 Dec 2008
  modified 20 May 2012
  by Tom Igoe & Jed Roach
  modified 27 Nov 2015
  by Arturo Guadalupi

  This example code is in the public domain.
*/

#define MODEM_RESET 32
#define MODEM_FLIGHT 22
#define MODEM_RX 26
#define MODEM_TX 25

long timer1;

void setup() {
  // initialize both serial ports:
  Serial.begin(115200);
  pinMode(MODEM_FLIGHT, OUTPUT); // FLIGHT MODE ENABLE
  pinMode(MODEM_RESET, OUTPUT); // MODEM RESET PIN
  digitalWrite(MODEM_FLIGHT, HIGH); // FLIGHT MODE

  MODEM_RESET_CYC();
  delay(2000);
  Serial1.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  Serial.println("SIM AT ATART >>>>>>>>>>>>>>");
  delay(2000);
  Serial1.println("AT");
  delay(2000);
  Serial1.println("AT+CPIN?");
  delay(2000);
  Serial1.println("AT+CNMP?");

  Serial.println("The device is powered up");
  pinMode(35, INPUT); // Digital Input 1
  pinMode(34, INPUT); // Digital Input 2
  pinMode(21, INPUT); // ADS1115 Interrupt
  pinMode(13, OUTPUT); // RS-485 flow control
  digitalWrite(13, HIGH);
  Serial.println("Initialized digital inputs");
}

void loop() {
  delay(3000);
  timer1 = millis();
  Serial1.println("AT");
  while (millis() < timer1 + 10000) {
    while (Serial1.available()) {
      int inByte = Serial1.read();
      Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial1.println("AT+CPIN?");
  while (millis() < timer1 + 10000) {
    while (Serial1.available()) {
      int inByte = Serial1.read();
      Serial.write(inByte);
    }
  }

  Serial.println("AT SCAN DONE");

  while (Serial.available()) {
    int inByte = Serial.read();
    Serial1.write(inByte);
  }

  while (Serial1.available()) {
    int inByte = Serial1.read();
    Serial.write(inByte);
  }

  // Digital Input Test
  Serial.print("I1: ");
  Serial.println(digitalRead(35));
  Serial.print("I2: ");
  Serial.println(digitalRead(34));
  delay(800);
  Serial.println("RS-485 TX TEST");
}

void MODEM_RESET_CYC() {
  digitalWrite(MODEM_RESET, HIGH);
  delay(1000);
  digitalWrite(MODEM_RESET, LOW);
  delay(1000);
  digitalWrite(MODEM_RESET, HIGH);
}
