// Line sensor pins
const int LEFT_SENSOR = 13;
const int MIDDLE_SENSOR = 12;
const int RIGHT_SENSOR = 11;

// Ultrasonic sensor pins
const int TRIG_PIN = 10;
const int ECHO_PIN = 8;

// Motor pins
const int ENA = 9;
const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;
const int ENB = 3;

// Motor speed
const int MOTOR_SPEED = 255;
const int SEARCH_SPEED = 150;

// Obstacle detection threshold (in cm)
const int OBSTACLE_DISTANCE = 14;

void setup() {
  pinMode(MIDDLE_SENSOR, INPUT);
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  delay(20000);
}

void loop() {
  bool rightSensor = digitalRead(MIDDLE_SENSOR);
  bool leftSensor = digitalRead(LEFT_SENSOR);
  bool middleSensor = digitalRead(RIGHT_SENSOR);

  // 0 for white surface, 1 for black surface
  // Follow the line based on sensor inputs
  if (middleSensor == 0 && rightSensor == 1 && leftSensor == 1) {
    moveForward();
  }
  else if (leftSensor == 0 && rightSensor == 1 && middleSensor == 1) {
    // Line is to the left → Turn left
    turnLeftSlightly();
  }
  else if (leftSensor == 1 && rightSensor == 0 && middleSensor == 1) {
    // Line is to the right → Turn right
    turnRightSlightly();
  }
  else {
    // Lost the line → Search
    searchForLine();
  }
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
}

void turnLeftSlightly() {
  digitalWrite(IN1, LOW);   // left wheel slower
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  // right wheel forward
  digitalWrite(IN4, LOW);
  analogWrite(ENA, SEARCH_SPEED); // slow left
  analogWrite(ENB, MOTOR_SPEED);  // normal right
}

void turnRightSlightly() {
  digitalWrite(IN1, HIGH);  // left wheel forward
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // right wheel slower
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, MOTOR_SPEED);  // normal left
  analogWrite(ENB, SEARCH_SPEED); // slow right
}

void turnLeftInPlace() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, SEARCH_SPEED);
  analogWrite(ENB, SEARCH_SPEED);
}

void turnRightInPlace() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, SEARCH_SPEED);
  analogWrite(ENB, SEARCH_SPEED);
}

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}

// Alternate left and right until white line is detected
void searchForLine() {
  stopMotors();
  delay(100);

  bool found = false;

  while (!found) {
    // Turn left
    turnLeftInPlace();
    for (int i = 0; i < 500; i += 50) {
      delay(50);
      if (digitalRead(MIDDLE_SENSOR) == 0 && digitalRead(RIGHT_SENSOR) == 1 && digitalRead(LEFT_SENSOR) == 1) {  // Found white line
        found = true;
        break;
      }
    }

    if (found) break;

    stopMotors();
    delay(100);

    // Turn right
    turnRightInPlace();
    for (int i = 0; i < 1000; i += 50) {
      delay(50);
      if (digitalRead(MIDDLE_SENSOR) == 0 && digitalRead(RIGHT_SENSOR) == 1 && digitalRead(LEFT_SENSOR) == 1) {  // Found white line
        found = true;
        break;
      }
    }

    stopMotors();
    delay(100);
  }
}
