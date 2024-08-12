void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
}

void loop() {
  if (Serial.available()){
  digitalWrite(13, HIGH);
  }
}
