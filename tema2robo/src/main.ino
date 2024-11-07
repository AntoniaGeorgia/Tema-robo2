#include <Arduino.h>

#define R 6
#define G 5
#define B 4
#define BTN1 2
#define BTN 3

volatile bool blinking = false;  
volatile int blinkCount = 0;     
volatile int countdownNumber = 3;
volatile int runda = 0;

String text;
String cuvant;
String cuvinte[] = {"parc", "rosie", "portocala", "zombie", "machiaje", "ciocolata", "cafenea"};
int nrcuv = sizeof(cuvinte) / sizeof(cuvinte[0]);

unsigned long startTime;        
unsigned long timerDuration;    
bool timerRunning = false;


unsigned long easyTimerDuration = 10000;  
unsigned long mediumTimerDuration = 7000;  
unsigned long hardTimerDuration = 5000;    


ISR(TIMER1_COMPA_vect) {
  if (blinking) {
    
    digitalWrite(R, digitalRead(R) ^ 1);
    digitalWrite(G, digitalRead(G) ^ 1);
    digitalWrite(B, digitalRead(B) ^ 1);

    
    if (countdownNumber > 0) {
      Serial.print(countdownNumber);
      Serial.println("!");
      countdownNumber--;
    }

    blinkCount++;

    
    if (blinkCount >= 6) {
      blinking = false;
      blinkCount = 0;
      countdownNumber = 3;
      digitalWrite(G, HIGH); 
      digitalWrite(R, LOW);
      digitalWrite(B, LOW);
    }
  }
}

void CheckSwitch() {
  blinking = !blinking;  
  if (!blinking) {
    blinkCount = 0;
    countdownNumber = 3;
    digitalWrite(R, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(B, HIGH);
  }
}

void checkrunda() {
  if (!blinking) {
    runda++;
    switch (runda % 3) {
      case 0:
        Serial.println("Easy mode on!");
        timerDuration = easyTimerDuration;  
        break;
      case 1:
        Serial.println("Medium mode on!");
        timerDuration = mediumTimerDuration;  
        break;
      case 2:
        Serial.println("Hard mode on!");
        timerDuration = hardTimerDuration;  
        break;
    }
  }
  
  
  if (digitalRead(BTN1) == LOW) {
    runda = 0;
    Serial.println("GAME OVER");
    nrcuv = sizeof(cuvinte) / sizeof(cuvinte[0]);  
    blinking = false;  
  }
}

void afiseazaCuvantAleator() {
  if (nrcuv > 0) {
    int indexAleator = random(nrcuv);
    cuvant = cuvinte[indexAleator];
    Serial.println(cuvant);
    cuvinte[indexAleator] = cuvinte[nrcuv - 1];  
    nrcuv--;
    startTime = millis();  
    timerRunning = true;
  }
}

void citire() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {  
      text.trim(); 
      if (text == cuvant) {
        Serial.println("Corect! Generăm un alt cuvânt...");
        text = "";
        afiseazaCuvantAleator();  
        timerRunning = true;
      } else {
        text = "";  
      }
    } else {
      text += c;  
    }
  }
}

void setup() {
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(BTN1, INPUT_PULLUP);
  pinMode(BTN, INPUT_PULLUP);
  digitalWrite(R, HIGH);
  digitalWrite(G, HIGH);
  digitalWrite(B, HIGH);

  Serial.begin(9600);

 
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  
  TCCR1B |= (1 << WGM12); 
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A);  
  interrupts();

  attachInterrupt(digitalPinToInterrupt(BTN), checkrunda, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN1), CheckSwitch, FALLING);
  randomSeed(analogRead(0));

  
  timerDuration = easyTimerDuration;
}

void loop() {
  if (!blinking && nrcuv > 0) {
    citire();

    
    if (timerRunning && (millis() - startTime >= timerDuration)) {
      Serial.println("Timpul a expirat! Generăm un alt cuvânt...");
      text = "";  
      afiseazaCuvantAleator();  
      startTime = millis();  
    }
  } else if (nrcuv <= 0) {
    Serial.println("Jocul s-a terminat");
    blinking = false;
    nrcuv = sizeof(cuvinte) / sizeof(cuvinte[0]);  
  }
}
