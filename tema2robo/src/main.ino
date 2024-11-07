#include <Arduino.h>

#define R 6
#define G 5
#define B 4
#define BTN1 2
#define BTN 3

volatile bool blinking = false;  // Flag to start/stop blinking
volatile int blinkCount = 0;     // Counter for the number of blinks
volatile int countdownNumber = 3;
volatile int runda = 0;

String text;
String cuvant;
String cuvinte[] = {"parc", "rosie", "portocala", "zombie", "machiaje", "ciocolata", "cafenea"};
int nrcuv = sizeof(cuvinte) / sizeof(cuvinte[0]);

unsigned long startTime;        // Time when the word is displayed
unsigned long timerDuration;    // Timer duration based on difficulty
bool timerRunning = false;

// Difficulty Timer Durations (in milliseconds)
unsigned long easyTimerDuration = 10000;  // 10 seconds for Easy mode
unsigned long mediumTimerDuration = 7000;  // 7 seconds for Medium mode
unsigned long hardTimerDuration = 5000;    // 5 seconds for Hard mode

// Timer1 Compare Match A Interrupt Service Routine
ISR(TIMER1_COMPA_vect) {
  if (blinking) {
    // Toggle the LEDs every 1 second for blinking effect
    digitalWrite(R, digitalRead(R) ^ 1);
    digitalWrite(G, digitalRead(G) ^ 1);
    digitalWrite(B, digitalRead(B) ^ 1);

    // Print the countdown number every second
    if (countdownNumber > 0) {
      Serial.print(countdownNumber);
      Serial.println("!");
      countdownNumber--;
    }

    blinkCount++;

    // Stop blinking after 3 complete on-off cycles (6 toggles)
    if (blinkCount >= 6) {
      blinking = false;
      blinkCount = 0;
      countdownNumber = 3;
      digitalWrite(G, HIGH);  // Turn off LEDs
      digitalWrite(R, LOW);
      digitalWrite(B, LOW);
    }
  }
}

void CheckSwitch() {
  blinking = !blinking;  // Toggle the blinking state
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
        timerDuration = easyTimerDuration;  // Set timer for Easy mode
        break;
      case 1:
        Serial.println("Medium mode on!");
        timerDuration = mediumTimerDuration;  // Set timer for Medium mode
        break;
      case 2:
        Serial.println("Hard mode on!");
        timerDuration = hardTimerDuration;  // Set timer for Hard mode
        break;
    }
  }
  
  // If Button1 is pressed, reset the game and print "GAME OVER"
  if (digitalRead(BTN1) == LOW) {
    runda = 0;
    Serial.println("GAME OVER");
    nrcuv = sizeof(cuvinte) / sizeof(cuvinte[0]);  // Reset the words list
    blinking = false;  // Stop blinking when game ends
  }
}

void afiseazaCuvantAleator() {
  if (nrcuv > 0) {
    int indexAleator = random(nrcuv);
    cuvant = cuvinte[indexAleator];
    Serial.println(cuvant);
    cuvinte[indexAleator] = cuvinte[nrcuv - 1];  // Remove used word
    nrcuv--;
    startTime = millis();  // Start the timer
    timerRunning = true;
  }
}

void citire() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {  // Check for newline or carriage return
      text.trim();  // Remove any extra whitespace
      if (text == cuvant) {
        Serial.println("Corect! Generăm un alt cuvânt...");
        text = "";
        afiseazaCuvantAleator();  // Generate a new random word
        timerRunning = true;
      } else {
        text = "";  // Clear the input text for the next attempt
      }
    } else {
      text += c;  // Collect characters for the word
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

  // Timer1 settings for 1Hz interrupts (1 second period)
  noInterrupts();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15624;  // Set OCR1A for 1Hz interrupt with 1024 prescaler
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024
  TIMSK1 |= (1 << OCIE1A);  // Enable Timer1 interrupt
  interrupts();

  attachInterrupt(digitalPinToInterrupt(BTN), checkrunda, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN1), CheckSwitch, FALLING);
  randomSeed(analogRead(0));

  // Default mode is Easy
  timerDuration = easyTimerDuration;
}

void loop() {
  if (!blinking && nrcuv > 0) {
    citire();

    // Check if time has expired for the current word
    if (timerRunning && (millis() - startTime >= timerDuration)) {
      Serial.println("Timpul a expirat! Generăm un alt cuvânt...");
      text = "";  // Clear the input text
      afiseazaCuvantAleator();  // Generate a new random word
      startTime = millis();  // Reset the timer
    }
  } else if (nrcuv <= 0) {
    Serial.println("Jocul s-a terminat");
    blinking = false;
    nrcuv = sizeof(cuvinte) / sizeof(cuvinte[0]);  // Reset the game
  }
}
