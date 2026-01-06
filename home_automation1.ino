// Pin definitions
int pirPin = 2;
int ldrPin = A0;
int tempPin = A1;

int lightLED = 8;
int fanLED = 9;
int buzzer = 10;

// Timing variables
unsigned long lastMotionTime = 0;
unsigned long offDelay = 10000; // 10 seconds

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(lightLED, OUTPUT);
  pinMode(fanLED, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  Serial.println("Smart Room Automation System Started");
}

void loop() {

  int motion = digitalRead(pirPin);
  int ldrValue = analogRead(ldrPin);
  int tempValue = analogRead(tempPin);

  // TMP36 conversion
  float voltage = tempValue * (5.0 / 1023.0);
  float temperature = (voltage - 0.5) * 100;

  // MOTION DETECTED
  if (motion == HIGH) {
    lastMotionTime = millis();

    Serial.println("\nRoom Status: OCCUPIED");

    // Short beep for motion
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);

    // Light control
    if (ldrValue < 500) {
      digitalWrite(lightLED, HIGH);
      Serial.println("Light Status: ON (Room is Dark)");
    } else {
      digitalWrite(lightLED, LOW);
      Serial.println("Light Status: OFF (Room is Bright)");
    }

    // Fan control
    if (temperature > 30) {
      digitalWrite(fanLED, HIGH);
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C → FAN ON");

      // Alert beep for high temperature
      digitalWrite(buzzer, HIGH);
      delay(200);
      digitalWrite(buzzer, LOW);
    } else {
      digitalWrite(fanLED, LOW);
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.println(" C → FAN OFF");
    }
  }

  // NO MOTION
  else {
    if (millis() - lastMotionTime > offDelay) {
      digitalWrite(lightLED, LOW);
      digitalWrite(fanLED, LOW);

      Serial.println("\nRoom Status: EMPTY");
      Serial.println("All devices turned OFF after delay");
    }
  }

  delay(500);
}
