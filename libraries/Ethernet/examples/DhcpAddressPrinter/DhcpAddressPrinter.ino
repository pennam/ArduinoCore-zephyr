/*
  DHCP-based IP printer

  This sketch uses the DHCP extensions to the Ethernet library
  to get an IP address via DHCP and print the address obtained.
  using an Arduino Wiznet Ethernet shield.

 */

#include "ZephyrEthernet.h"

void setup() {
  // start serial port:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }

  // in Zephyr system check if Ethernet is ready before proceeding to initialize
  while (Ethernet.linkStatus() != LinkON) {
    Serial.println("Waiting for link on");
    delay(100);
  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin() == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // no point in carrying on, so do nothing forevermore:
    while (true) {
      delay(1);
    }
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.println(Ethernet.localIP());
}

void loop() {
  if (Ethernet.linkStatus() != LinkON) {
    Serial.println(Ethernet.linkStatus());
    connectEth();
  }
}

void connectEth() {
  // in Zephyr system check if Ethernet is ready before proceeding to initialize
  Serial.print("Waiting for link on");
  while (Ethernet.linkStatus() != LinkON) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin() == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1);  // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  Serial.println("You're connected to the network");
  Serial.println();
}
