// Motor control pins
const int ENA = 12;  // Left motor speed
const int IN1 = 8;   // Left motor direction
const int IN2 = 11;

const int IN3 = 10;  // Right motor direction
const int IN4 = 9;
const int ENB = 13;  // Right motor speed

void setup() {
  // Set all the motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Set motor directions to forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // Set motor speeds (0 to 255)
  analogWrite(ENA, 200); // Left motor speed
  analogWrite(ENB, 200); // Right motor speed
}

void loop() {
  // Keep moving forward
}