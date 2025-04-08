#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 if needed

byte card_ID[4];
byte Name1[4] = {0x03, 0xDA, 0xF4, 0x20};     // Adwait Bhalerao's card UID
byte Name2[4] = {0x04, 0x2F, 0x07, 0x22};     // Shreyas Newe's card UID

int NumbCard[2] = {0, 0};
int j = 0;

String Name;
long Number;
int n = 0;

// Declare the function before setup()
bool checkID(byte a[], byte b[]) {
  for (byte i = 0; i < 4; i++) {
    if (a[i] != b[i]) return false;
  }
  return true;
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Mark your");
  lcd.setCursor(0, 1);
  lcd.print("attendance");

  Serial.println("CLEARSHEET");
  Serial.println("LABEL,Date,Time,Name,Number");
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  for (byte i = 0; i < 4; i++) {
    card_ID[i] = mfrc522.uid.uidByte[i];
  }

  if (checkID(card_ID, Name1)) {
    Name = "Adwait Bhalerao";
    Number = 1262241696;
    j = 0;
  } else if (checkID(card_ID, Name2)) {
    Name = "Shreyas Newe";
    Number = 1262241387;
    j = 1;
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Unknown Card");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mark your");
    lcd.setCursor(0, 1);
    lcd.print("attendance");
    return;
  }

  if (NumbCard[j] == 1) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(Name.substring(0, 16));
    lcd.setCursor(0, 1);
    lcd.print("Already Logged");
  } else {
    NumbCard[j] = 1;
    n++;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Welcome,");
    lcd.setCursor(0, 1);
    lcd.print(Name.substring(0, 16)); // Trim name to fit LCD
    Serial.print("DATA,DATE,TIME," + Name + ",");
    Serial.println(Number);
    Serial.println("SAVEWORKBOOKAS,Names/WorkNames");
  }

  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Mark your");
  lcd.setCursor(0, 1);
  lcd.print("attendance");
}
