#include <Keypad.h>
#include <SPI.h>
#include <RFID.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#define SS_PIN 10
#define RST_PIN 9
#define relayPin 2
RFID rfid(SS_PIN, RST_PIN);

LiquidCrystal lcd(3, 8, 14, 15, 16, 17);

// Setup variables:
int serNum0;
int serNum1;
int serNum2;
int serNum3;
int serNum4;
int I2C_Address = 4;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'5','6','7','8'},
  {'9','A','B','C'},
  {'D','*','0','#'}
};
byte rowPins[ROWS] = {7, 6, 5, 4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = { 3, 2, 1, 0}; //connect to the column pinouts of the keypad

int pos = 0;
//this is where we will define who can open the doors/gate
//this is the keypad code
char secretCode[6] = {'1', '2', '3', '4', '5', '6'};
//this is where the RFID card numbers will be stored
int card_One[5] = {21, 111, 159, 175, 74}; //card name is sunfounder


char inputCode[6] = {'0', '0', '0', '0', '0', '0'};

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

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
}

void loop()
{ 
  if (rfid.isCard()) 
  {
    if (rfid.readCardSerial()) 
    {
      if(rfid.serNum[0] == card_One[1] && rfid.serNum[1] == card_One[2] && rfid.serNum[2] == card_One[3] && rfid.serNum[3] == card_One[4] && rfid.serNum[4] == card_One[5])
      {
        lcd.setCursor(0,1);
        lcd.print("Hello SUNFOUNDER");
        openDoor();
      }
      else 
      {
        /* If we have the same ID, just write a dot. */
        lcd.setCursor(0,1);
        lcd.print("                 ");
        lcd.setCursor(0,1);
        lcd.print("Hello unkown guy!");
        closeDoor();
        delay(1000);
      }
    }
  }
  lcd.setCursor(0,1);
  lcd.print("                 ");
  closeDoor();
  rfid.halt();
  readKey();
}
void readKey()
{
  int correct = 0;
  int i;
  char customKey = customKeypad.getKey();
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
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Access Control ");
          closeDoor();
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
void openDoor(){
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  delay(20000);
  digitalWrite(relayPin, LOW);
  pinMode(relayPin, INPUT);  
}
void closeDoor(){
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(relayPin, INPUT);
}
