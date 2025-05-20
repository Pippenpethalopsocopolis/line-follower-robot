// Line sensor pin (Middle sensor)
const int MIDDLE_SENSOR = 12; Write your line tracker sensor pins here

// Ultrasonic sensor pins
const int TRIG_PIN = 10; // Write your ultrasonic sensor pins here
const int ECHO_PIN = 8;

// Motor driver pins
const int ENA = 9; //Write your motor driver pins here
const int IN1 = 7;
const int IN2 = 6;
const int IN3 = 5;
const int IN4 = 4;
const int ENB = 3;

// Motor speed
const int MOTOR_SPEED = 255;
const int SEARCH_SPEED = 120;

// Obstacle detection threshold (in cm)
const int OBSTACLE_DISTANCE = 10;

void setup() {
  pinMode(MIDDLE_SENSOR, INPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  bool onLine = digitalRead(MIDDLE_SENSOR);  // 1 = black, 0 = white
  int distance = getDistance();

  if (distance < OBSTACLE_DISTANCE) {
    stopMotors();
    return;
  }

  if (onLine == 0) {  // Sensor sees white, follow line
    moveForward();
  } else {  // Sensor sees black, search for line
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
      if (digitalRead(MIDDLE_SENSOR) == 0) {  // Found white line
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
      if (digitalRead(MIDDLE_SENSOR) == 0) {  // Found white line
        found = true;
        break;
      }
    }

    stopMotors();
    delay(100);
  }
}

int getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);
  return duration * 0.034 / 2;
}
