**🚀Complete Guide: Line Following Robot using PID Control**

A **Line Following Robot (LFR)** is an autonomous robot that follows a predefined path (usually a black line on a white surface). This guide will cover **everything** from **hardware setup** to **full coding and calibration**.

---

## **🛠 Step 1: Components Needed**

| Component                   | Quantity |
| --------------------------- | -------- |
| Arduino Uno                 | 1        |
| L298N Motor Driver          | 1        |
| IR Sensor Array (5 Sensors) | 1        |
| DC Motors with Wheels       | 2        |
| Robot Chassis               | 1        |
| Li-ion Battery (7.4V-12V)   | 1        |
| Connecting Wires            | -        |

---

## **🔌 Step 2: Circuit Diagram**

### **Connections**

| Component        | Arduino Pin |
| ---------------- | ----------- |
| Motor Driver ENA | D5          |
| Motor Driver ENB | D10         |
| Motor Driver IN1 | D6          |
| Motor Driver IN2 | D7          |
| Motor Driver IN3 | D8          |
| Motor Driver IN4 | D9          |
| IR Sensor S1     | A0          |
| IR Sensor S2     | A1          |
| IR Sensor S3     | A2          |
| IR Sensor S4     | A3          |
| IR Sensor S5     | A4          |

---

## **⚡ Step 3: Understanding PID Control**

The **PID algorithm** helps the robot adjust motor speeds based on sensor input.

- **P (Proportional):** Adjusts speed based on error.
- **I (Integral):** Accumulates past errors (not always needed).
- **D (Derivative):** Predicts future errors for smoother correction.

---

## **📜 Step 4: Complete Code**

### **Line Following Robot Code with PID & Sensor Calibration**

```cpp
// **Motor Driver Pins (L298N)**
#define ENA 5   // Left motor speed (PWM)
#define ENB 10  // Right motor speed (PWM)
#define IN1 6   // Left motor forward
#define IN2 7   // Left motor backward
#define IN3 8   // Right motor forward
#define IN4 9   // Right motor backward

// **IR Sensor Pins**
#define S1 A0  
#define S2 A1  
#define S3 A2  
#define S4 A3  
#define S5 A4  

// **PID Constants (Adjust during testing)**
float Kp = 25;  // Proportional Gain
float Ki = 0;   // Integral Gain (often 0 for LFR)
float Kd = 12;  // Derivative Gain

int baseSpeed = 100;  // Base motor speed
int threshold[5];  // Store calibrated sensor thresholds
int position = 0;
float lastError = 0, I = 0;

void setup() {
  // **Motor Pins Setup**
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // **Sensor Pins Setup**
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  
  Serial.begin(9600);

  // **Calibrate Sensors at Startup**
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

  // **Convert Analog to Digital using Thresholds**
  for (int i = 0; i < 5; i++) {
    sensorValues[i] = (sensorValues[i] > threshold[i]) ? 1 : 0;
  }

  // **Calculate Position using Weighted Sum**
  position = (sensorValues[0] * -2) + (sensorValues[1] * -1) +
             (sensorValues[2] * 0) + (sensorValues[3] * 1) +
             (sensorValues[4] * 2);

  // **PID Calculation**
  float P = position;
  I += position;
  float D = position - lastError;
  float PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);

  lastError = position;

  // **Adjust Motor Speeds**
  int leftSpeed = baseSpeed + PIDvalue;
  int rightSpeed = baseSpeed - PIDvalue;

  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  moveRobot(leftSpeed, rightSpeed);
}

// **Move Robot Based on PID Output**
void moveRobot(int leftSpeed, int rightSpeed) {
  analogWrite(ENA, leftSpeed);
  analogWrite(ENB, rightSpeed);

  digitalWrite(IN1, leftSpeed > 0);
  digitalWrite(IN2, leftSpeed <= 0);
  digitalWrite(IN3, rightSpeed > 0);
  digitalWrite(IN4, rightSpeed <= 0);
}

// **Sensor Calibration Function**
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
```

---

## **📌 Troubleshooting**

| Problem                        | Solution                                   |
| ------------------------------ | ------------------------------------------ |
| Robot does not move            | Check motor wiring and power supply.       |
| Robot moves in wrong direction | Swap motor wires (IN1 & IN2, IN3 & IN4).   |
| Robot overshoots the track     | Reduce `Kp`, increase `Kd`.                |
| Robot moves too slow           | Increase `baseSpeed` in code.              |
| Sensors not detecting properly | Adjust sensor height and check thresholds. |

---

## **🔚 Final Thoughts**

✅ **Hardware assembled and wired correctly**\
✅ **Sensors calibrated for accurate readings**\
✅ **PID algorithm tuned for smooth motion**\
✅ **Line following test successful**

If your LFR is **not working as expected**, share your **sensor readings, PID behavior, and track design**, and I’ll help you troubleshoot further!

