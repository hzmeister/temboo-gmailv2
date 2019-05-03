#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <Temboo.h>

#define TEMBOO_ACCOUNT "TembooAccName"  // Your Temboo account name 
#define TEMBOO_APP_KEY_NAME "myFirstApp"  // Your Temboo app key name
#define TEMBOO_APP_KEY "slkasmclkamsclalkscmalkc"  // Your Temboo app key

int alarmPin = 8; //uses pin8 as trigger for this example

byte ethernetMACAddress[] = { 0x90, 0xA2, 0xDA, 0x00, 0x55, 0x65 }; //must be unique on LAN
EthernetClient client;

void setup() {
  pinMode(alarmPin, INPUT_PULLUP);
  pinMode(4, OUTPUT); //disable w5100 sdcard
  digitalWrite(4, HIGH); //disable w5100 sdcard
  Serial.begin(9600);

  // For debugging, wait until the serial console is connected.
  delay(4000);
  while (!Serial);
  Serial.print("DHCP:");
  if (Ethernet.begin(ethernetMACAddress) == 0) {
    Serial.println("FAIL");
    while (true);
  }
  Serial.println("OK");
}
void loop() {
  if (digitalRead(alarmPin) == HIGH) {

    TembooChoreo SendMessageChoreo(client);

    // Invoke the Temboo client
    SendMessageChoreo.begin();

    // Set Temboo account credentials
    SendMessageChoreo.setAccountName(TEMBOO_ACCOUNT);
    SendMessageChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    SendMessageChoreo.setAppKey(TEMBOO_APP_KEY);

    // Set Choreo inputs
    SendMessageChoreo.addInput("RefreshToken", "mclakmclka-mclkamckalmclkasmc-lkasdmclkasmclk");
    SendMessageChoreo.addInput("ClientSecret", "doisadscoaj_dcoiajiosajc");
    SendMessageChoreo.addInput("ClientID", "2809128348-kscaslcasdncanckadsnckja.apps.googleusercontent.com");
    SendMessageChoreo.addInput("From", "sender@gmail.com");
    SendMessageChoreo.addInput("To", "recip1@gmail.com,recip2@gmail.com,recip3@gmail.com"); //can have multiple recipients
    SendMessageChoreo.addInput("Subject", "SUBJECT"); //must incldude subject
    SendMessageChoreo.addInput("MessageBody", "MESSAGE"); //must include message

    // Identify the Choreo to run
    SendMessageChoreo.setChoreo("/Library/Google/Gmailv2/Messages/SendMessage");

    // Run the Choreo; when results are available, print them to serial
    SendMessageChoreo.run();

    while (SendMessageChoreo.available()) {
      char c = SendMessageChoreo.read();
      Serial.print(c);
    }
    SendMessageChoreo.close();
  }
}
