int PIR_IN = 12; // Pin D6
// # define LED_OUT LED_BUILTIN // Pin D7
int led1 = 16;// pin D0
int led2 = 5;// pin D1
int led3 = 4;// pin D2
int led4 = 0;// pin D3
int led5 = 13;// pin D4
int led6 = 14; // pin D5

int led_stair_arr[] = {led1, led2, led3, led4, led5,led6};

void setup() {
  // put your setup code here, to run once:

  pinMode(PIR_IN, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(led6,OUTPUT);

  Serial.begin(9600);
  reset_LED();
}

void led_staircase()
{
  for (int i = 0 ; i < 6 ; i++)
  {
    digitalWrite(led_stair_arr[i], HIGH);
    Serial.println(i);
    delay(300);
  }
}

void reset_LED()
{
  for (int i = 0 ; i < 6 ; i++)
    digitalWrite(led_stair_arr[i], LOW);
  
}

void loop() {

  bool PIR_val = digitalRead(PIR_IN);

  if(PIR_val == HIGH)
  {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("Motion Detected");
    
    led_staircase();
    delay(3000);

    reset_LED();
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH);
    // Serial.println("Motion NOT Detected");
  }

  digitalWrite(led5, LOW);

  // delay(1000);
  // put your main code here, to run repeatedly:
   
}
