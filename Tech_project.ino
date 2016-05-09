  /**
TODO compleatly rewrite the readkey function getting unxepected behaviour
**/
#include <Keypad.h>
#include <SPI.h>
#include <RFID.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define SS_PIN 10
#define RST_PIN 9
#define relayPin 8
RFID rfid(SS_PIN, RST_PIN);

LiquidCrystal_I2C lcd(0x27,16,2);
// Setup variables:
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;
int I2C_Address = 4;
int byteRead = 0;
int position = 0;

char recivedInfo[5];
char* password = "1337";

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char Keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {17, 16, 15, 14}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 5, 4, 3, 2 }; //connect to the column pinouts of the keypad

int pos = 0;
//this is where we will define who can open the doors/gate
//this is the keypad code
char secretCode[6] = {'1', '2', '3', '4', '5', '6'};
//this is where the RFID card numbers will be stored
int card_One[5] = {210, 147, 224, 99, 194}; //card name is sunfounder


char inputCode[6] = {'0', '0', '0', '0', '0', '0'};
char user[9];

//initialize an instance of class NewKeypad
Keypad keypad = Keypad( makeKeymap(Keys), rowPins, colPins, ROWS, COLS); 

void setup()
{ 
  Serial.begin(9600);
  Wire.begin(I2C_Address);
  lcd.begin(16, 2);
  SPI.begin(); 
  rfid.init(); 
  lcd.setCursor(0,0);
  lcd.print(" Access Control ");
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);  
  lcd.backlight(); 
}
void loop()
{  
/*  char customKey = customKeypad.getKey();
  switch(customKey){
    case '1' :
      readRfid();
      lcd.setCursor(0,0);
      lcd.print("   Scan Card    ");
    case '2' :
      readKey();
      lcd.setCursor(0,0);
      lcd.print("   Enter code   ");
    break;
 }*/
  char key = keypad.getKey();
  Serial.println(key);  
  if (key == '*' || key == '#'){
    position = 0;
    closeDoor();
  }
  if (key == password[position]){
    position ++;
  }
  if (position == 4){
    openDoor();
  }
  delay(100);
  readRfid();
}
void readRfid()
{ 
  if (rfid.isCard()) 
  {
    if (rfid.readCardSerial()) 
    {
      if(rfid.serNum[0] == 18 && rfid.serNum[1] == 35 && rfid.serNum[2] == 180 && rfid.serNum[3] == 169 && rfid.serNum[4] == 44)
      {
        lcd.setCursor(0,1);
        lcd.print("Hello NICK      ");
        openDoor();
        user[9] = 'Card01';
        Serial.print(rfid.serNum[4]);
      }
      else if(rfid.serNum[0] == 8 && rfid.serNum[1] == 110 && rfid.serNum[2] == 226 && rfid.serNum[3] == 118 && rfid.serNum[4] == 242)
      {
        lcd.setCursor(0,1);
        lcd.print("Hello SUNFOUNDER");
        openDoor();
        user[9] = 'Card01';
        Serial.print(rfid.serNum[4]);
      }
      else if(rfid.serNum[0] == 2 && rfid.serNum[1] == 0 && rfid.serNum[2] == 192 && rfid.serNum[3] == 169 && rfid.serNum[4] == 107)
      {
        lcd.setCursor(0,1);
        lcd.print("Hello RILEY     ");
        openDoor();
        user[9] = 'Card01';
        Serial.print(rfid.serNum[4]);
      }
      else if(rfid.serNum[0] == 66 && rfid.serNum[1] == 192 && rfid.serNum[2] == 188 && rfid.serNum[3] == 169 && rfid.serNum[4] == 151)
      {
        lcd.setCursor(0,1);
        lcd.print("Hello SUNFOUNDER");
        openDoor();
        user[9] = 'Card01';
        Serial.print(rfid.serNum[4]);
      }
      else 
      {
        /* If we have the same ID, just write a dot. */
        lcd.setCursor(0,1);
        lcd.print("                 ");
        lcd.setCursor(0,1);
        lcd.print("Hello unkown guy!");
        closeDoor();
        Serial.println(rfid.serNum[0]);
        Serial.println(rfid.serNum[1]);
        Serial.println(rfid.serNum[2]);
        Serial.println(rfid.serNum[3]);
        Serial.println(rfid.serNum[4]);
        delay(1000);
      }
    }
  }
  lcd.setCursor(0,1);
  lcd.print("                 ");
  closeDoor();
  rfid.halt();
}
/*void readKey()
{
  int correct = 0;
  int i;
  char customKey = customKeypad.getKey();
  while(1){
  if (customKey)
  {
    switch(customKey)
    {
      case '*':
        pos = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Input Your Code:");
      break;
      case '#':    
        for(i = 0; i < 6; i++)
        {
          if(inputCode[i] == secretCode[i])
          {
            correct ++;
          }
        }
        if(correct == 6)
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Input correctly!");
          lcd.setCursor(0, 1);
          lcd.print(" Please Come In ");
          openDoor();
          user[9] = 'Keypad01';
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          loop();
          lcd.print(" Access Control ");
          closeDoor();
          correct = 0;
        }
        else
        {
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("  Input Error!  ");
          lcd.setCursor(0, 1);
          lcd.print("  Please Again  ");
          closeDoor();
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Access Control ");
        }
      break;
      default:
        inputCode[pos] = customKey;
        lcd.setCursor(pos,1);
        lcd.print(inputCode[pos]);
        pos ++;
    }
    }
  }
}*/
void openDoor(){
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Serial.println("open");
  position = 0;  
  delay(2000);
  digitalWrite(relayPin, LOW);
  //pinMode(relayPin, INPUT);
}
void closeDoor(){
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  //pinMode(relayPin, INPUT);
  Serial.println("close");
}
void requestEvent()
{
  Wire.write(user);
}
void receiveEvent(int test)
{
  if (Wire.available()){
    while (Wire.available()){
      recivedInfo[byteRead] = Wire.read();
      byteRead++;
    }
    if(recivedInfo[0] == 'F' && recivedInfo[1] == 'I' && recivedInfo[2] == 'R' && recivedInfo[3] == 'E'){
      while(1){
        pinMode(relayPin, OUTPUT);
        digitalWrite(relayPin, HIGH);
        delay(100);
        Wire.beginTransmission(0); // transmit to device #0
        Wire.write("FIRE");        // sends five bytes
        Wire.endTransmission();    // stop transmitting        
      }
    }
    if(recivedInfo[0] == 'O' && recivedInfo[1] == 'P' && recivedInfo[2] == 'E' && recivedInfo[3] == 'N'){
      openDoor();      
    }
    if(recivedInfo[0] == 'S' && recivedInfo[1] == 'T' && recivedInfo[2] == 'O' && recivedInfo[3] == 'P'){
      while(1){
        pinMode(relayPin, OUTPUT);
        digitalWrite(relayPin, HIGH);
        delay(100);
        Wire.beginTransmission(0); // transmit to device #0
        Wire.write("STOP");        // sends five bytes
        Wire.endTransmission();    // stop transmitting        
      }
    }
  }
  byteRead = 0;
}
