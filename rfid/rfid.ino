#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;          // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = 10;         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
    Serial.begin(9600);   // Initialize serial communications with the PC
    while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();      // Init SPI bus
    mfrc522.PCD_Init();   // Init MFRC522
    mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
      return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
      return;
  }

  if(mfrc522.uid.uidByte[0] == 0x03 && mfrc522.uid.uidByte[1] == 0x28 && mfrc522.uid.uidByte[2] == 0x46 && mfrc522.uid.uidByte[3] == 0x18){
    Serial.println("Pass");
  }else{
    Serial.println("fail");
  }
}
  
