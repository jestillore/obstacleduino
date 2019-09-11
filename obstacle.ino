
#include<Servo.h>

#define trigger 13
#define echo 12
#define leftAngle 150
#define rightAngle 30
#define frontAngle 90
#define ena 11
#define in1 9
#define in2 8
#define enb 5
#define in3 7
#define in4 6
#define obstacleStop 20
#define obstacleTurn 35

Servo sonarServo;
int fromBackward = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(trigger, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(ena, OUTPUT);
  pinMode(enb, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  analogWrite(ena, 200);
  analogWrite(enb, 200);

  sonarServo.attach(10);
  lookFront();
  delay(2000);
}

void loop() {
  int distance, leftDistance, rightDistance;
  distance = getDistance();

  if (distance > obstacleStop && fromBackward == 0) {
    moveForward();
  } else {
    fromBackward = 0;
    
    stopCar();
    delay(1000);
    lookLeft();
    leftDistance = getDistance();
    delay(1000);
    lookRight();
    rightDistance = getDistance();
    delay(1000);
    lookFront();
    delay(1000);

    // check if left and right obstacle distance is more than minimum
    // then we turn to whichever's obstacle is farther
    if (leftDistance > obstacleTurn && leftDistance >= rightDistance) {
      turnLeft();
      delay(750);
      stopCar();
      delay(750);
    } else if (rightDistance > obstacleTurn && rightDistance >= leftDistance) {
      turnRight();
      delay(750);
      stopCar();
      delay(750);
    } else {
      moveBackward();
      delay(500);
      stopCar();
      delay(750);
      fromBackward = 1;
    }
  }
  delay(100);
}

void lookLeft() {
  sonarServo.write(leftAngle);
}

void lookRight() {
  sonarServo.write(rightAngle);
}

void lookFront() {
  sonarServo.write(frontAngle);
}

void moveForward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void turnLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void stopCar() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void moveBackward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

int getDistance() {
  int duration, distance;
  
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  duration = pulseIn(echo, HIGH);
  distance = (1 / 29.1) * (duration / 2);
  
  return distance;
}
