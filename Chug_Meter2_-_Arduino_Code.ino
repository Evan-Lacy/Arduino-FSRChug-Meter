#include<LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7);
const int pressurePin = A0;
const int buttonPin = 2;
int buttonState = 0;
int fsrVal;
int emptyGlass;
int milliseconds = 0;
int seconds = 0;
int minutes = 0;
int timeLimit = 1;
int topMinutes = -1;
int topSeconds = -1;
int topMilliseconds = -1;

void setup() {
  Serial.begin(9600);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.begin(16,2);
  pinMode(buttonPin, INPUT);

}

void loop() {
  buttonState = digitalRead(buttonPin);
  clearScreen();
  lcd.setCursor(0,0);
  if (topMilliseconds == -1) {
    lcd.print("Top Chug: NULL");
  } else {
    lcd.print("Top Chug:");
    lcd.print(topMinutes);
    lcd.print(":");
    lcd.print(topSeconds);
    lcd.print(":");
    lcd.print(topMilliseconds);
  }
  lcd.setCursor(0,1);
  lcd.print("Wait for Empty");
  
  if (buttonState == HIGH){
      fsrVal = analogRead(pressurePin);
      if(fsrVal > 0){
        emptyGlass = fsrVal;
        Serial.println("Empty Glass found");
        
        clearScreen();
        lcd.setCursor(0,0);
        lcd.print("Empty Val: ");
        lcd.print(emptyGlass);
        lcd.setCursor(0,1);
        lcd.print("Pickup glass!");
        
        fillGlassLoop();
      }
  }
}
void fillGlassLoop(){
  while(true){
    fsrVal = analogRead(pressurePin);
    if(fsrVal == 1023){
      clearScreen();
      
      Serial.println("Chug started");
      break;
    }
  }
  chugLoop();
}
void chugLoop(){
  lcd.setCursor(0,1);
  lcd.print("Chug! Chug!");
  while(true){
    fsrVal = analogRead(pressurePin);
    if(fsrVal == emptyGlass){
      Serial.println("Chug ended");
      break;
    }
    timerTick();
    delay(1);
  }
  postChug();
}

void timerTick(){
    if(milliseconds == 99){
      seconds++;
      milliseconds = 0;
    }
    if (seconds == 59){
      minutes++;
      seconds = 0;
    }

    Serial.print(minutes);
    Serial.print(":");
    Serial.print(seconds);
    Serial.print(":");
    Serial.println(milliseconds);

    lcd.setCursor(0,0);
    lcd.print("Attempt: ");
    lcd.print(minutes);
    lcd.print(":");
    lcd.print(seconds);
    lcd.print(":");
    lcd.print(milliseconds);
    
    milliseconds++;
    /*if (minutes >= timeLimit){
      timeStart = false;
    }
     milliseconds++;
    if (emptyGlass){
      timeStart = false;
    }*/
}

void postChug() {
  clearScreen();
  lcd.setCursor(0,0);
  if (topMilliseconds == -1) {
    lcd.print("Top Chug: NULL");
  } else {
    lcd.print("Top Chug:");
    lcd.print(topMinutes);
    lcd.print(":");
    lcd.print(topSeconds);
    lcd.print(":");
    lcd.print(topMilliseconds);
  }
  lcd.setCursor(0,1);
  lcd.print("Attempt: ");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds);
  lcd.print(":");
  lcd.print(milliseconds);
  delay(5000);

  if (topMinutes == -1 && topSeconds == -1 && topMilliseconds == -1) {
    topMinutes = minutes;
    topSeconds = seconds;
    topMilliseconds = milliseconds;
  } else if (minutes > topMinutes) {
    
  } else if (seconds > topSeconds) {
    
  } else if (milliseconds > topMilliseconds) {
    
  } else {
    topMinutes = minutes;
    topSeconds = seconds;
    topMilliseconds = milliseconds;
  }

  minutes = 0;
  seconds = 0;
  milliseconds = 0;
}

 void clearScreen(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
 }

