void setup() {
  // Set pins to digital output
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
}

void output(int values) {
  digitalWrite(6, values & 0x01);
  digitalWrite(7, values & 0x02);
  digitalWrite(10, values & 0x04);
  digitalWrite(13, values & 0x08);
}

void colorChoose(int color) {
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  
  if (color == 0) {
    digitalWrite(2, HIGH);
  } else if (color == 1) {
    digitalWrite(3, HIGH);
  } else if (color == 2) {
    digitalWrite(4, HIGH);
  }
}

void loop()
{
  for (int i=0; i<4; i++) {
    colorChoose(i);
    delay(1000);
  }
  
  digitalWrite(4, HIGH);
  analogWrite(3, 255);
  delay(1000);
  
  for (int i=255; i>=0; i--) {
    analogWrite(3, i);
    delay(20);
  }

  digitalWrite(2, HIGH);
  digitalWrite(4, HIGH);

  
  for (;;) {
    output(1);
    delay(250);
    output(3);
    delay(250);
    output(7);
    delay(250);
    output(15);
    delay(1000);
    output(7);
    delay(250);
    output(3);
    delay(250);
    output(1);
    delay(250);
    output(0);
    delay(1000);

    output(15);
    delay(125);
    output(0);
    delay(125);
    output(15);
    delay(125);
    output(0);
    delay(2000);
  }
}

