#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>
 #include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 12, 10, 11, 13);

Adafruit_MPU6050 mpu;

int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 7;
int enA = 5;
int enB = 6;

const int IR1 = A3, IR2 = A2;
const int sPin1 = A0, sPin2 = A1;
int Dstate1 = 0;
int lastDstate1 = 0;
int Dstate2 = 0;
int lastDstate2 = 0;
double D1 = 0;
double D2 = 0;
double tD = 0;  

unsigned long prevTime = 0;
float A=0;
int bump =0;
int spin =0;
int stop =0;
void setup() {

    if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  pinMode(sPin1, INPUT);
  pinMode(sPin2, INPUT);

  lcd.begin(16, 2);
  lcd.clear();

  digitalWrite(enA, LOW);
  digitalWrite(enB, LOW);

}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  unsigned long currTime = millis();
  if (currTime - prevTime >= 150) {
    prevTime = currTime;
    lcd.clear();
      A=(atan(a.acceleration.z/a.acceleration.x) /3.1415926 * 180 ) +3;
    lcd.setCursor(0, 0);
      lcd.print("Time: ");
      lcd.print(currTime / 1000);
      lcd.print("s ");
    lcd.setCursor(0, 1);
      lcd.print("A: "); 
      lcd.print(A,1);
    lcd.setCursor(8, 1);
      lcd.print("D: ");
      lcd.print(tD);
      lcd.print(" m");
    if ( A <-20 && bump<1 && spin<1 ) { 
          bump=bump+1; 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
        lcd.clear();
      A=(atan(a.acceleration.x/a.acceleration.z) /3.1415926 * 180 ) +3;
    lcd.setCursor(0, 0);
      lcd.print("Time: ");
      lcd.print(currTime / 1000);
      lcd.print("s ");
    lcd.setCursor(0, 1);
      lcd.print("A: "); 
      lcd.print(A,1);
    lcd.setCursor(8, 1);
      lcd.print("D: ");
      lcd.print(tD);
      lcd.print(" m");
        delay(1200);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    }
    if (A>-7 && bump>0 && spin<1){
          spin=spin+1;
          A=(atan(a.acceleration.x/a.acceleration.z) /3.1415926 * 180 ) +3;
    lcd.setCursor(0, 0);
      lcd.print("Time: ");
      lcd.print(currTime / 1000);
      lcd.print("s ");
    lcd.setCursor(0, 1);
      lcd.print("A: "); 
      lcd.print(A,1);
    lcd.setCursor(8, 1);
      lcd.print("D: ");
      lcd.print(tD);
      lcd.print(" m");
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
    analogWrite(enA, 200);
    analogWrite(enB, 200);
      delay(50);

    analogWrite(enA, 0);
    analogWrite(enB, 0);
      delay(4000);
      
    digitalWrite(in2, LOW);
    digitalWrite(in1, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
    analogWrite(enA, 255);
    analogWrite(enB, 255);
      delay(2110 );
      bump = 1;
    analogWrite(enA, 0);
    analogWrite(enB, 0);
      delay(200);
    }
  }
  if(A>14){
    bump++;
  }
        if (A<3 && bump>1 && spin>0){
  Dstate1 = digitalRead(sPin1);
  if (Dstate1 != lastDstate1 && Dstate1 == 1) {
    D1= D1 + 1.125;
  }
  lastDstate1 = Dstate1;

  Dstate2 = digitalRead(sPin2);
  if (Dstate2 != lastDstate2 && Dstate2 == 1) {
    D2= D2 + 1.125;
  }
  lastDstate2 = Dstate2;
    }
          tD = (D1+D2)/200;
if( fabs(tD - 0.9) < 0.02 && stop <1){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);
    digitalWrite(in3, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  stop ++;
        delay(2000); 
    analogWrite(enA, 0);
    analogWrite(enB, 0);
}

  int Right = analogRead(IR2);
  int Left = analogRead(IR1);
  int Rthre = 900;
  int Lthre = 300;
  if ((Right > Rthre) && (Left > Lthre) && spin<1) { // Move straight
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
    analogWrite(enA, 150);
    analogWrite(enB, 150);
        delay(15);
        analogWrite(enA, 0);
    analogWrite(enB, 0);
  }else if ((Right > Rthre) && (Left > Lthre) && spin>0) { // Move straight
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
    analogWrite(enA, 85);
    analogWrite(enB, 85);
        delay(10);
        analogWrite(enA, 0);
    analogWrite(enB, 0);
    }else if ((Right > Rthre) && (Left > Lthre) && bump>1) { // Move straight
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
    analogWrite(enA, 115);
    analogWrite(enB, 115);
        delay(10);
        analogWrite(enA, 0);
    analogWrite(enB, 0);
  } else if ((Right <Rthre) && (Left < Lthre)) { // Stop
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);
    digitalWrite(in3, LOW);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
        delay(15);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  } else if ((Right < Rthre) && (Left >Lthre)) { // Turn right
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in4, HIGH);
    digitalWrite(in3, LOW);
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    delay(70);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  } else if ((Right > Rthre) && (Left <Lthre)) { // Turn left
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in4, LOW);
    digitalWrite(in3, HIGH);
    analogWrite(enA, 255);
    analogWrite(enB, 255);
    delay(70);
    analogWrite(enA, 0);
    analogWrite(enB, 0);
  }
  }