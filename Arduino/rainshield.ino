#include <Servo.h>

Servo myservo;

// Pin configuration
#define BUZ 2
#define SERVO_PIN 3
#define RAIN 4
#define LED1 5
#define LED2 6

// Window positions
#define WINDOW_OPEN 90
#define WINDOW_CLOSED 0

// Rain sensor is active LOW
#define RAIN_DETECTED LOW

bool windowClosed = false;

void setup()
{
  Serial.begin(9600);

  pinMode(BUZ, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(RAIN, INPUT);

  // Attach servo
  myservo.attach(SERVO_PIN);

  // IMPORTANT:
  // Every power-up/reset starts with window OPEN
  myservo.write(WINDOW_OPEN);

  // Initial dry/open indication
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(BUZ, LOW);

  windowClosed = false;

  Serial.println("================================");
  Serial.println("SMART BUS WINDOW SYSTEM");
  Serial.println("SYSTEM: ONLINE");
  Serial.println("WINDOW: OPEN - 90 DEG");
  Serial.println("================================");

  delay(1000);
}

void loop()
{
  int rain = digitalRead(RAIN);

  // =========================================
  // RAIN DETECTED
  // =========================================

  if (rain == RAIN_DETECTED && windowClosed == false)
  {
    Serial.println("Rain detected...");
    
    // Confirm rain condition
    delay(1000);

    if (digitalRead(RAIN) == RAIN_DETECTED)
    {
      Serial.println("RAIN CONFIRMED");
      Serial.println("WINDOW CLOSING");

      // Rain indication
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);

      // Short buzzer
      digitalWrite(BUZ, HIGH);
      delay(300);
      digitalWrite(BUZ, LOW);

      // Close window
      myservo.write(WINDOW_CLOSED);

      delay(1000);

      windowClosed = true;

      Serial.println("WINDOW CLOSED - 0 DEG");
    }
  }

  // =========================================
  // RAIN STOPPED
  // =========================================

  if (rain != RAIN_DETECTED && windowClosed == true)
  {
    Serial.println("Rain stopped...");

    // Confirm dry condition
    delay(2000);

    if (digitalRead(RAIN) != RAIN_DETECTED)
    {
      Serial.println("DRY CONDITION CONFIRMED");
      Serial.println("WINDOW OPENING");

      // Dry indication
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, LOW);
      digitalWrite(BUZ, LOW);

      // Open window
      myservo.write(WINDOW_OPEN);

      delay(1000);

      windowClosed = false;

      Serial.println("WINDOW OPEN - 90 DEG");
    }
  }

  delay(100);
}
