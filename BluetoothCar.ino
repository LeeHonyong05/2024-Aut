const int trigPinLeft = 10;
const int echoPinLeft = 11;
const int trigPinRight = 8;
const int echoPinRight = 9;

int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 7;
int enA = 5;
int enB = 6;

const int IR1 = A3, IR2 = A2; // Infrared sensors
int data;

const int trigPin = 0;  // Ultrasonic sensor for distance
const int echoPin = 1;
float distanceLeft = 0;
float distanceRight = 0;
float lastMeasurementTime = 0;
unsigned long currentTime = 0;

void setup() {
  Serial.begin(9600);  // Start Serial communication

  // Set motor control pins as OUTPUT
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  // Set ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPinLeft, OUTPUT);
  pinMode(echoPinLeft, INPUT);
  pinMode(trigPinRight, OUTPUT);
  pinMode(echoPinRight, INPUT);
}

void loop() {
  currentTime = millis();  // Get current time for distance measurement

  // Read ultrasonic sensors if 50ms have passed
  if (currentTime - lastMeasurementTime > 50) {
    lastMeasurementTime = currentTime;

    // Measure left sensor
    digitalWrite(trigPinLeft, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinLeft, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinLeft, LOW);
    float durationLeft = pulseIn(echoPinLeft, HIGH);
    distanceLeft = (durationLeft * 0.0343) / 2;

    // Measure right sensor
    digitalWrite(trigPinRight, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPinRight, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinRight, LOW);
    float durationRight = pulseIn(echoPinRight, HIGH);
    distanceRight = (durationRight * 0.0343) / 2;

    // Print sensor readings for debugging
    Serial.print("Left: ");
    Serial.print(distanceLeft);
    Serial.print(" cm, Right: ");
    Serial.print(distanceRight);
    Serial.println(" cm");
  }

  // Obstacle detection logic
  if (distanceLeft <= 50 || distanceRight <= 50) {
    stopCar(); // Stop the car if an obstacle is detected
    delay(500); // Wait for a while
    backupCar(); // Backup the car
    delay(1000); // Backup for 1 second
    stopCar(); // Stop after backing up
    delay(500); // Wait before resuming normal operation
  }

  // Check if data is available from Bluetooth/Serial
  if (Serial.available() > 0) {
    data = Serial.read();  // Read incoming data

    // Handle different control commands from Bluetooth
    if (data == 'Z') { // IR control (based on IR sensor values)
      handleIRControl();
    } 
    else if (data == 'F') { // Move forward
      moveForward();
    }
    else if (data == 'B') { // Move backward
      moveBackward();
    }
    else if (data == 'L') { // Turn left
      turnLeft();
    }
    else if (data == 'R') { // Turn right
      turnRight();
    }
    else if (data == 'S') { // Stop
      stopCar();
    }
    else if (data == 'D') { // Spin/turn (example of custom behavior)
      spinAround();
    }
  }
}

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 150);  // Speed control
  analogWrite(enB, 150);  // Speed control
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 150);  // Speed control
  analogWrite(enB, 150);  // Speed control
}

void turnLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);  // Full speed
  analogWrite(enB, 255);  // Full speed
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);  // Full speed
  analogWrite(enB, 255);  // Full speed
}

void stopCar() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);    // Stop motors
  analogWrite(enB, 0);    // Stop motors
}

void backupCar() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 150);  // Speed control
  analogWrite(enB, 150);  // Speed control
}

void handleIRControl() {
  int Right = analogRead(IR2);
  int Left = analogRead(IR1);

  if ((Right > 900) && (Left > 300)) { // Move straight
    moveForward();
  } else if ((Right < 900) && (Left < 300)) { // Stop
    stopCar();
  } else if ((Right < 900) && (Left > 300)) { // Turn right
    turnRight();
  } else if ((Right > 900) && (Left < 300)) { // Turn left
    turnLeft();
  }
}

void spinAround() {
  // Spin for 2.5 seconds
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 150);
  analogWrite(enB, 255);
  delay(2500);

  // Reverse direction for 2.5 seconds
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 150);
  delay(2500);
}
