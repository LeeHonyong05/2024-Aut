const int trigPinLeft = 10;
const int echoPinLeft = 11;
const int trigPinRight = 8;
const int echoPinRight = 9;

const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 7;
const int enA = 5;
const int enB = 6;

const int threshold = 30;

unsigned long lastMeasurementTime = 0;
const int measurementInterval = 50;

void setup() {
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  unsigned long currentTime = millis();

  if (currentTime - lastMeasurementTime > measurementInterval) {
    lastMeasurementTime = currentTime;

    // Measure left sensor
    digitalWrite(trigPinLeft, LOW);
        digitalWrite(trigPinRight, LOW);
    delayMicroseconds(1);
    digitalWrite(trigPinLeft, HIGH);
        digitalWrite(trigPinRight, HIGH);
    delayMicroseconds(5);
    digitalWrite(trigPinLeft, LOW);
    float durationLeft = pulseIn(echoPinLeft, HIGH);
    float distanceLeft = (durationLeft * 0.0343) / 2;
        digitalWrite(trigPinRight, LOW);
    float durationRight = pulseIn(echoPinRight, HIGH);
    float distanceRight = (durationRight * 0.0343) / 2;

    // Print sensor readings for debugging
    Serial.print("Left: ");
    Serial.print(distanceLeft);
    Serial.print(" cm, Right: ");
    Serial.print(distanceRight);
    Serial.println(" cm");

    // Decision making based on distance readings
    if (distanceLeft < threshold && distanceRight < threshold) {
      // Both sensors are too close, move backwards
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in4, HIGH);
      digitalWrite(in3, LOW);
      analogWrite(enA, 170);
      analogWrite(enB, 170);
    } else if (distanceLeft > threshold && distanceRight < threshold) {
      // Only right sensor is too close, turn left
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in4, HIGH);
      digitalWrite(in3, LOW);
      analogWrite(enA, 170);
      analogWrite(enB, 170);
    } else if (distanceLeft < threshold && distanceRight > threshold) {
      // Only left sensor is too close, turn right
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in4, LOW);
      digitalWrite(in3, HIGH);
      analogWrite(enA, 170);
      analogWrite(enB, 170);
    } else if (distanceLeft > threshold && distanceRight > threshold) {
      // Both sensors are clear, stop
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in4, LOW);
      digitalWrite(in3, LOW);
      analogWrite(enA, 0);
      analogWrite(enB, 0);

      // Add a small delay to ensure the motors are stopped
      delay(100);  // 100ms delay to allow motors to completely stop
    }
  }
}
