int PIR_IN = 12; // Pin D6
// # define LED_OUT LED_BUILTIN // Pin D7

void setup() {
  // put your setup code here, to run once:

  pinMode(PIR_IN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  bool PIR_val = digitalRead(PIR_IN);

  if(PIR_val == HIGH)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Motion Detected");
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    // Serial.println("Motion NOT Detected");
  }

  // delay(1000);
  // put your main code here, to run repeatedly:

  
}
