#include <Servo.h>

// Servo objects
Servo servos[6]; // 0:base, 1:shoulder, 2:elbow, 3:wristPitch, 4:wristRoll, 5:gripper
const byte servoPins[6] = {4, 6, 8, 9, 11, 12};

// Ultrasonic and Pump (ACTIVE-LOW RELAY)
const int trigPin = 5;     // D2
const int echoPin = 10;     // D3
const int relayPin = 7;    // D5 (connected to relay IN)


// Movement parameters
const int moveDuration = 6000; // ms per movement
const int gripDelay = 100;     // ms for grip/release

// Safety limits
const int angleLimits[6][2] = {
  {10, 180}, {10, 180}, {10, 180}, {10, 180}, {10, 180}, {0, 180}
};

// Calibrated positions
const int homePos[6] = {90, 90, 90, 90, 90, 130};
const int approachPos[6] = {160, 30, 160, 90, 180, 130};
const int pickPos[6] = {160, 40, 150, 90, 170, 10};    // Gripper closed to 5°
const int liftPos[6] = {160, 60, 150, 90, 150, 5};    // Keep closed
const int releasePos[6] = {20, 30, 150, 90, 180, 140}; // Gripper open to 120°

void setup() {
  Serial.begin(9600);
  
  // Initialize ultrasonic and relay (ACTIVE-LOW)
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH); // Start with pump OFF
  
  // Attach servos
  for(int i=0; i<6; i++) {
    servos[i].attach(servoPins[i]);
    safeWrite(i, homePos[i]);
  }
  delay(1000);
}

void loop() {
  pickAndPlaceCycle();
  delay(7000); // Pause between cycles
}
// ----------------------------
// Existing movement functions (unchanged)
// ----------------------------
void smoothMove(const int target[6], bool moveGripper = true) {
  int start[6];
  float current[6];
  unsigned long startTime = millis();
  
  for(int i=0; i<6; i++) {
    start[i] = servos[i].read();
    current[i] = start[i];
    if(!moveGripper && i==5) continue;
    validateAngle(i, target[i]);
  }

  while(millis() - startTime < moveDuration) {
    float progress = easeInOutCubic(float(millis() - startTime) / moveDuration);
    for(int i=0; i < (moveGripper ? 6 : 5); i++) {
      current[i] = start[i] + (target[i] - start[i]) * progress;
      safeWrite(i, round(current[i]));
    }
    delay(20);
  }

  for(int i=0; i < (moveGripper ? 6 : 5); i++) {
    safeWrite(i, target[i]);
  }
}

float easeInOutCubic(float t) {
  return t < 0.5 ? 4 * t * t * t : 1 - pow(-2 * t + 2, 3) / 2;
}

void pickAndPlaceCycle() {
  /* ----- PICK PHASE ----- */
  smoothMove(approachPos);
  delay(100);
  
  smoothMove(pickPos);
  safeWrite(5, 5); // Close gripper firmly
  delay(gripDelay);
  
  smoothMove(liftPos);
  
  /* ----- TRANSPORT PHASE ----- */
  smoothMove(releasePos, false); // Move without changing gripper
  
  /* ----- RELEASE & DISPENSE PHASE ----- */
  safeWrite(5, 130); // Open gripper
  delay(100);
  
  // Check for cup and dispense
  
  
  // Return home
  int clearPos[6];
  memcpy(clearPos, releasePos, sizeof(releasePos));
  clearPos[1] = 60; // Lift shoulder
  smoothMove(clearPos);
  
  smoothMove(homePos);
  
  if(checkCupPresent()) {
    delay(2000); // Wait 2 seconds
     // Run pump for 2 seconds
  }
}

// Ultrasonic distance check
bool checkCupPresent() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  float distance = pulseIn(echoPin, HIGH) * 0.034 / 2;
  
  if (distance < 10) {
    delay (2000); // If cup is detected within 10 cm
    digitalWrite(relayPin, LOW); // Turn ON pump (Relay active LOW)
    delay(4000); // Run for 2 seconds
    digitalWrite(relayPin, HIGH); // Turn OFF pump
  }

  delay(500);
}


// Pump control (ACTIVE-LOW RELAY)


void safeWrite(int servoIndex, int angle) {
  angle = constrain(angle, angleLimits[servoIndex][0], angleLimits[servoIndex][1]);
  servos[servoIndex].write(angle);
  Serial.print("Servo "); Serial.print(servoIndex);
  Serial.print(" set to "); Serial.println(angle);
}

void validateAngle(int servoIndex, int angle) {
  if(angle < angleLimits[servoIndex][0] || angle > angleLimits[servoIndex][1]) {
    Serial.print("WARNING: Servo ");
    Serial.print(servoIndex);
    Serial.print(" target ");
    Serial.print(angle);
    Serial.print(" exceeds limits (");
    Serial.print(angleLimits[servoIndex][0]);
    Serial.print(",");
    Serial.print(angleLimits[servoIndex][1]);
    Serial.println(")");
  }
}