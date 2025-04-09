// Define sensor pins
const int sensor1 = 24; // Channel 1 connected to pin 24
const int sensor2 = 2;  // Channel 2 connected to pin 2
const int sensor3 = 3;  // Channel 3 connected to pin 3
const int sensor4 = 4;  // Channel 4 connected to pin 4
const int sensor5 = 5;  // Channel 5 connected to pin 5
const int sensor6 = 6;  // Channel 6 connected to pin 6
const int sensor7 = 7;  // Channel 7 connected to pin 7
const int sensor8 = 8;  // Channel 8 connected to pin 8

// Define motor control pins
const int motor1Dir = 11;   // Direction control for Motor 1
const int motor1PWM = 9;    // PWM control for Motor 1
const int motor2Dir = 12;   // Direction control for Motor 2
const int motor2PWM = 10;   // PWM control for Motor 2

void setup() {
    // Initialize IR sensor pins
    pinMode(sensor1, INPUT);
    pinMode(sensor2, INPUT);
    pinMode(sensor3, INPUT);
    pinMode(sensor4, INPUT);
    pinMode(sensor5, INPUT);
    pinMode(sensor6, INPUT);
    pinMode(sensor7, INPUT);
    pinMode(sensor8, INPUT);

    // Initialize Motor Driver pins
    pinMode(motor1Dir, OUTPUT);
    pinMode(motor1PWM, OUTPUT);
    pinMode(motor2Dir, OUTPUT);
    pinMode(motor2PWM, OUTPUT);

    // Initialize serial communication for marker detection
    Serial.begin(9600);
}

void loop() {
    // Check for serial input from Python
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n'); // Read the command
        Serial.print("Received command: ");
        Serial.println(command);

        if (command == "STOP") {
            stopMotors();
            Serial.println("Marker Detected: Robot Stopped");
        } 
    }

    // Read sensor values
    int s1 = digitalRead(sensor1);
    int s2 = digitalRead(sensor2);
    int s3 = digitalRead(sensor3);
    int s4 = digitalRead(sensor4);
    int s5 = digitalRead(sensor5);
    int s6 = digitalRead(sensor6);
    int s7 = digitalRead(sensor7);
    int s8 = digitalRead(sensor8);

    // // Debugging: print sensor values to Serial Monitor
    // Serial.print(s1);
    // Serial.print(",");
    // Serial.print(s2);
    // Serial.print(",");
    // Serial.print(s3);
    // Serial.print(",");
    // Serial.print(s4);
    // Serial.print(",");
    // Serial.print(s5);
    // Serial.print(",");
    // Serial.print(s6);
    // Serial.print(",");
    // Serial.print(s7);
    // Serial.print(",");
    // Serial.println(s8);

    // Logic for forward motion
    if (s1 == 1 && s2 == 1 && s3 == 1 && s4 == 1 && s5 == 1 && s6 == 1 && s7 == 1 && s8 == 1) {
        stopMotors();
    } else if (s4 == 1 && s5 == 1) {
        moveForward();
    } else if ((s4 == 1 && s5 == 1) && (s3 == 1 || s6 == 1) && (s1 == 0 && s2 == 0 && s7 == 0 && s8 == 0)) {
        moveForward();
    } else if ((s6 == 0 && s2 == 1 && s7 == 0 && s8 == 0) || (s5 == 0 && s1 == 1)) {
        turnRight();
    } else if ((s3 == 0 && s7 == 1 && s2 == 0 && s1 == 0) || (s4 == 0 && s8 == 1)) {
        turnLeft();
    } else {
        stopMotors();
    }
}


void moveForward() {
    digitalWrite(motor1Dir, LOW); // Set motor 1 direction
    analogWrite(motor1PWM, 50);  // Set motor 1 speed
    digitalWrite(motor2Dir, LOW); // Set motor 2 direction
    analogWrite(motor2PWM, 50);  // Set motor 2 speed
    //Serial.println("Moving Forward");
}

void turnRight() {
    digitalWrite(motor1Dir, LOW); // Motor 1 forward
    analogWrite(motor1PWM, 50);   // Motor 1 speed

    digitalWrite(motor2Dir, LOW);  // Motor 2 brake or stop
    analogWrite(motor2PWM, 0);     // Ensure Motor 2 is completely stopped

    //Serial.println("Turning Right");
}

void turnLeft() {
    digitalWrite(motor1Dir, LOW);  // Motor 1 stop
    analogWrite(motor1PWM, 0);     // Motor 1 stopped

    digitalWrite(motor2Dir, LOW); // Motor 2 forward
    analogWrite(motor2PWM, 50);   // Motor 2 speed

    //Serial.println("Turning Left");
}

void stopMotors() {
    digitalWrite(motor1Dir, LOW); // Optional: set direction to neutral or opposite
    analogWrite(motor1PWM, 0);   // Stop motor 1
    digitalWrite(motor2Dir, LOW); // Optional: set direction to neutral or opposite
    analogWrite(motor2PWM, 0);   // Stop motor 2
    
}
