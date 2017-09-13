/******************************************************************************
Gamer Glove Project by MoCo Makers
www.MoCoMakers.com

Lisence: CC0
https://creativecommons.org/publicdomain/zero/1.0/
******************************************************************************/
//These flex sensor values may need to be calibrated based on the person's finger
//or design of the glove. We used the Arduino IDE to calibrate these values by using
// Tools -> Serial Plotter as well as Tools -> Serial Monitor
const int LEFT_FLEX_THRESHOLD = 730;
const int RIGHT_FLEX_THRESHOLD = 775;

const int FLEX_PIN = A11;
const int TILT_PIN = 3;

const int LEFT_OUT = 2;
const int RIGHT_OUT = 0;
const int JUMP_OUT = 1 ;

// LEFT: Flora D2 --> Raspberry Pi GPIO 4
// RIGHT: Flora D0 --> Raspberry Pi GPIO 16
// Y Button (Jump): Flora D1 --> Raspberry Pi GPIO 18

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 3.3; // Measured voltage of Ardunio 5V line
const float R_DIV = 33000.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 11000.0; // resistance when straight
const float BEND_RESISTANCE = 22000.0; // resistance at 90 deg

void setup() 
{
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  pinMode(TILT_PIN, INPUT_PULLUP);
  pinMode(LEFT_OUT, OUTPUT);
  pinMode(RIGHT_OUT, OUTPUT);
  pinMode(JUMP_OUT, OUTPUT);
}

void loop() 
{
  // Read the ADC, and calculate voltage and resistance from it
  int flexADC = analogRead(FLEX_PIN);
  float flexV = flexADC * VCC / 1023.0;
  float flexR = R_DIV * (VCC / flexV - 1.0);
//  Serial.println("Resistance: " + String(flexR) + " ohms");
  Serial.print(flexADC, DEC);
  int tilt = digitalRead(TILT_PIN);
  Serial.print(",");
  Serial.println(tilt*500, DEC);

  //Check if Left Direction should be triggered
  if (flexADC < LEFT_FLEX_THRESHOLD){
    digitalWrite(LEFT_OUT, LOW);
    digitalWrite(RIGHT_OUT, HIGH);
  }

  //Check if Right Direction should be triggered
  else if (flexADC > RIGHT_FLEX_THRESHOLD){
    digitalWrite(LEFT_OUT, HIGH);
    digitalWrite(RIGHT_OUT, LOW);
  }
  else{
    //Since 'grounding' the GPIO pins of the Raspberry Pi triggers the GamerG
    digitalWrite(LEFT_OUT, HIGH);
    digitalWrite(RIGHT_OUT, HIGH);
  }

  if (tilt>0){
    digitalWrite(JUMP_OUT, LOW);
  }
  else{
    digitalWrite(JUMP_OUT,HIGH);
  }

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
//  Serial.println("Bend: " + String(angle) + " degrees");
//  Serial.println();

  delay(50);
}

