// Motor Driver Pins
#define ENA 5   // Left motor speed (PWM)
#define ENB 10  // Right motor speed (PWM)
#define IN1 6   // Left motor forward
#define IN2 7   // Left motor backward
#define IN3 8   // Right motor forward
#define IN4 9   // Right motor backward

// IR Sensor Pins
#define S1 A0  
#define S2 A1  
#define S3 A2  
#define S4 A3  
#define S5 A4  

// PID Constants 
float Kp = 25;  // Proportional Gain (Best ranage is 20-30)
float Ki = 0;   // Integral Gain (often 0 for LFR)
float Kd = 12;  // Derivative Gain (10 teke 18 range e rakha valo)

int baseSpeed = 100;  // Base motor speed
int threshold[5];  // Store calibrated sensor thresholds
int position = 0;
float lastError = 0, I = 0;

void setup() {
  // Motor Pins Setup
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  //Sensor Pins Setup
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  
  Serial.begin(9600);

  // Calibrate Sensors at Startup
  calibrateSensors();
}

void loop() {
  int sensorValues[5] = {
    analogRead(S1),
    analogRead(S2),
    analogRead(S3),
    analogRead(S4),
    analogRead(S5)
  };

  // Convert Analog to Digital using Thresholds
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = (sensorValues[i] > threshold[i]) ? 1 : 0;
  }

  // Calculate Position using Weighted Sum**
  position = (sensorValues[0] * -2) + (sensorValues[1] * -1) + (sensorValues[2] * 0) + (sensorValues[3] * 1) + (sensorValues[4] * 2);

  // PID Calculation
  float P = position;
  I += position;
  float D = position - lastError;
  float PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);

  lastError = position;

  //Adjust Motor Speeds
  int leftSpeed = baseSpeed + PIDvalue;
  int rightSpeed = baseSpeed - PIDvalue;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  moveRobot(leftSpeed, rightSpeed);
}

// Move Robot Based on PID Output
void moveRobot(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);

  digitalWrite(IN1, leftSpeed > 0);
  digitalWrite(IN2, leftSpeed <= 0);
  digitalWrite(IN3, rightSpeed > 0);
  digitalWrite(IN4, rightSpeed <= 0);
}

// Sensor Calibration Function
void calibrateSensors() {
  Serial.println("Calibrating Sensors...");
  int minVals[5] = {1023, 1023, 1023, 1023, 1023};
  int maxVals[5] = {0, 0, 0, 0, 0};

  for (int i = 0; i < 200; i++) {  
    int sensorVals[5] = {
      analogRead(S1),
      analogRead(S2),
      analogRead(S3),
      analogRead(S4),
      analogRead(S5)
    };

    for (int j = 0; j < 5; j++) {
      if (sensorVals[j] < minVals[j]) minVals[j] = sensorVals[j];
      if (sensorVals[j] > maxVals[j]) maxVals[j] = sensorVals[j];
    }
    delay(5);
  }

  for (int j = 0; j < 5; j++) {
    threshold[j] = (minVals[j] + maxVals[j]) / 2;
    Serial.print("Threshold for S");
    Serial.print(j + 1);
    Serial.print(": ");
    Serial.println(threshold[j]);
  }
}