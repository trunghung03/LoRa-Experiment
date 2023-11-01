#include <SPI.h>
#include <LoRa.h>

#include <stdio.h>

//define the pins used by the transceiver module
#define ss PA4
#define rst PB0
#define dio0 PA3

// SPI stuff
SPIClass SPI_1(PA7, PA6, PA5);
// MOSI, MISO, SCLK

int totalPacket = 0;
int packetReceived = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);
  LoRa.setSPI(SPI_1);

  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0 && packetSize < 5) {
    // received a packet
    packetReceived++;

    char currentAmountArray[5] = {0, 0, 0, 0, 0};

    // read packet
    int i = 0;
    while (LoRa.available()) {

      char received = (char) LoRa.read();
      
      currentAmountArray[i++] = received;
      
      //Serial.print((char)LoRa.read());
    }
    currentAmountArray[i] = 0;

    // Serial.print("Received: ");
    // Serial.println(currentAmountArray[0]);
    // Serial.println(currentAmountArray[1]);

    sscanf(currentAmountArray, "%d", &totalPacket);

    Serial.print("total packet: ");
    Serial.print(totalPacket);
    Serial.print(" Packet received: ");
    Serial.print(packetReceived);

    double percent = ((double)packetReceived / totalPacket) * 100;
    Serial.print("Percentage: ");
    Serial.println(percent);
    
  }
}