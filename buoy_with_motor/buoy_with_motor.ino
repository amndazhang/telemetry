///Motor Pins
#define outputA 13
#define outputB 12

//Encoder
#define encoder 0
#define encoderRegLength 5
int encoderRegister[encoderRegLength];  //  <----------------------Shift register
int encoderRegSum = 0;                  //  <-------------/
int encoderVal = 5;      //   <----------depth of buoy in rotations
int dir = 1;  //direction of motor  (positive means going into water)

#define stopper 22
#define stopperRegLength 10
int stopperRegister[stopperRegLength];  //  <----------------------Shift register
int stopperRegSum = 0;                  //  <-------------/
int stopperActive = 0;  //<----------will be 1 when buoy is at top

int state = 2;
int maxDepth = 5; //33
#define deploy 14

void t() {
  Serial.print("\t");
}

void up() {
  digitalWrite(outputA, HIGH);
  digitalWrite(outputB, LOW);
  Serial.println("UP_____________");
}

void down() {
  digitalWrite(outputB, HIGH);
  digitalWrite(outputA, LOW);
  Serial.println("DOWN_______________");
}

void stopMotor() {
  digitalWrite(outputA, LOW);
  digitalWrite(outputB, LOW);
  Serial.println("STOP_______________");

}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Serial is now up");
  //motor
  pinMode(outputA, OUTPUT);
  pinMode(outputB, OUTPUT);

  pinMode(encoder, INPUT_PULLUP);
  pinMode(stopper, INPUT_PULLUP);

  pinMode(deploy, INPUT_PULLUP);

}

void loop() {
  //Telling the motors to stop


  //  // put your main code here, to run repeatedly:
  //  Serial.print(digitalRead(13));
  //  Serial.print('\t');
  //  Serial.print(digitalRead(encoderPin));
  //  Serial.print('\t');
  //  Serial.print(digitalRead(button));
  //  Serial.print('\t');
  //  Serial.println(digitalRead(button1));
  //  Serial.print('\t');
  //  Serial.println(digitalRead(12));
  //
  //


  //---------------------------------------------------------------------------------------------encoder shift register
  encoderRegSum = 0;
  for ( int index = 0; index < encoderRegLength - 1; index++) {
    encoderRegister[index] = encoderRegister[index + 1];
    encoderRegSum += encoderRegister[index];
  }
  encoderRegister[encoderRegLength - 1] = digitalRead(encoder);

  if ( encoderRegSum == 1  && encoderRegister[0] == HIGH) {  // falling edge (from high to low) (default is high ie. pullup)
    encoderVal += dir;
  }
  //-----------------------------------------------------------------------------------------------






  //---------------------------------------------------------------------------------------------stopper shift register
  stopperRegSum = 0;
  for ( int index = 0; index < stopperRegLength - 1; index++) {
    stopperRegister[index] = stopperRegister[index + 1];
    stopperRegSum += stopperRegister[index];
  }
  stopperRegister[stopperRegLength - 1] = digitalRead(stopper);

  if ( stopperRegSum < 3) {  // Magnet detected filter noise smaller than 9 cycles
    stopperActive = 1;
  }
  else {
    stopperActive = 0;
  }
  //-----------------------------------------------------------------------------------------------



  if (stopperActive == 1) encoderVal = 0;

  //wait for signal to go down
  if ( state == 0) {
    if (digitalRead(deploy) == LOW) {
      state = 1;
    }
    stopMotor();
  }

  //go down until reach bottom
  if (state == 1) {
    dir = 1; //depth larger ie. down
    if ( encoderVal >= maxDepth ) {
      state = 2;
    }
    down();
  }

  //go up until reach top
  if ( state  == 2 ) {
    dir = -1; //depth smaller ie. up
    if (encoderVal < 1) {
      state = 0;
    }
    up();
  }





  Serial.print(state); t();
  Serial.print(encoderVal); t();
  Serial.print(stopperActive); t();
  Serial.print(digitalRead(deploy)); t();
  //  Serial.print(stopperRegSum); t();
  //  Serial.print(stopperRegister[0]); t();
  //  Serial.print(digitalRead(stopper)); t();

  Serial.println();




}
