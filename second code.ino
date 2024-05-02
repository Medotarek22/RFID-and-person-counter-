#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10 // RX slave select
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

byte card_ID[4]; // Card UID size 4 bytes

byte knownCards[][4] = {
  {0x33, 0x21, 0x77, 0x2A}, // Card 1
  {0x33, 0x7E, 0xB2, 0x2A}, // Card 2
  {0x23, 0x22, 0x06, 0x2F}, // Card 3
  {0xD3, 0xA3, 0xB1, 0x2A}, // Card 4
  {0x83, 0x1B, 0x8E, 0x2A}, // Card 5
  {0x43, 0x45, 0xCB, 0x2A},  // Card 6
};

String names[] = {"MOHAMED", "elsharkawy", "MAHMOUD", "ABDALLAH", "HABIB", "MAZEN"};
long numbers[] = {3030522, 3030321, 3000221, 3030731, 3040209, 3030610};

int NumbCards[6] = {0}; // Array to track if each card has been detected
int statuses[6] = {0};  // Array to track if each card has entered or left

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  Serial.println("CLEARSHEET");
  Serial.println("LABEL,Date,Name,Number,Time IN,Time OUT");

  lcd.setCursor(2, 0);
  lcd.print("Good Morning");
  delay(1000);
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    card_ID[i] = mfrc522.uid.uidByte[i];
  }

  for (int i = 0; i < 6; i++) {
    if (compareArrays(card_ID, knownCards[i], 4)) {
      if (NumbCards[i] == 0) {
        NumbCards[i] = 1;
        int n = i + 1; // Card number
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Welcome ");
        lcd.setCursor(0, 1);
        lcd.print(names[i]);
        Serial.print("DATA,DATE,");
        Serial.print(names[i]);
        Serial.print(",");
        Serial.print(numbers[i]);
        Serial.print(",");
        Serial.print("TIME");
        Serial.print(",");
        Serial.println("");
        delay(500);
      } else if (statuses[i] == 0) {
        statuses[i] = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(names[i]);
        lcd.setCursor(0, 1);
        lcd.print("BYE BYE");
        Serial.print("DATA,DATE,");
        Serial.print(names[i]);
        Serial.print(",");
        Serial.print(numbers[i]);
        Serial.print(",,TIME");
      } else {
        statuses[i] = 1;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(names[i]);
        lcd.setCursor(0, 1);
        lcd.print("ALREADY LEFT");
      }
    }
  }
  delay(1000);

  // Return to "Good Morning" message
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Good Morning");
  delay(2000);
}

bool compareArrays(byte arr1[], byte arr2[], int size) {
  for (int i = 0; i < size; i++) {
    if (arr1[i] != arr2[i]) {
      return false;
    }
  }
  return true;
}