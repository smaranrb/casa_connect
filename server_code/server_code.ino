// Load Wi-Fi library
#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "SSSR 4337";
const char* password = "Rome@123";

int PIR_IN_1 = 12; // Pin D6
int PIR_IN_2 = 13; // Pin D6

// # define LED_OUT LED_BUILTIN // Pin D7
// LED's on staircase
int led1 = 2;// pin D0
int led2 = 5;// pin D1
int led3 = 4;// pin D2
int led4 = 0;// pin D3
int led5 = 16;// pin D4
int led6 = 14; // pin D5

int led_stair_arr[] = {led1, led2, led3, led4, led5,led6};

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output1State = "off";
String output3State = "off";
String server_control = "off";

// Assign output variables to GPIO pins
// const int output1 = 13;
const int output3 = 15; // d8

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  
  pinMode(output3, OUTPUT); //Rx
  // Set outputs to LOW
  // digitalWrite(output1, LOW);
  digitalWrite(output3, LOW);

  pinMode(PIR_IN_1, INPUT);
  pinMode(PIR_IN_2, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(led6,OUTPUT);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}


// Function fro serially lighing the LED's on the staircse
void led_staircase(){
  for (int i = 0 ; i < 6 ; i++)  {
    digitalWrite(led_stair_arr[i], HIGH);
    Serial.println(i);
    delay(300);
  }
}

// Function to reset all LED's on the staircase to LOW
void reset_LED(){
  for (int i = 0 ; i < 6 ; i++)
    digitalWrite(led_stair_arr[i], LOW);
}

// Server Loop - When Control is shifter to server
void server_loop(WiFiClient client){

    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();         
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            
            // turns the GPIOs on and off
            if (header.indexOf("GET /5/on") >= 0) {
              Serial.println("GPIO x on");
              output1State = "on";
              
              led_staircase();
            } else if (header.indexOf("GET /5/off") >= 0) {
              Serial.println("GPIO x off");
              output1State = "off";
              reset_LED();
              
            } else if (header.indexOf("GET /4/on") >= 0) {
              Serial.println("GPIO 15 on");
              output3State = "on";
              digitalWrite(output3, HIGH);

            } else if (header.indexOf("GET /4/off") >= 0) {
              Serial.println("GPIO 15 off");
              output3State = "off";
              digitalWrite(output3, LOW);
            
            } else if (header.indexOf("GET /S/on") >= 0) {
              Serial.println("Server Control on");
              server_control = "on";
            
            } else if (header.indexOf("GET /S/off") >= 0) {
              Serial.println("Server Control off");
              server_control = "off";
            }
            
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Web Page Heading
            client.println("<body><h1>CasaConnect Home Server</h1>");
            
            // Display current state, and ON/OFF buttons for GPIO 5  
            client.println("<p>Staircase Series Light (LED Series) - State " + output1State + "</p>");
            // If the output1State is off, it displays the ON button       
            if (output1State=="off") {
              client.println("<p><a href=\"/5/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/5/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
               
            // Display current state, and ON/OFF buttons for GPIO 15  
            client.println("<p>Room Light (LED 2) - State " + output3State + "</p>");
            // If the output3State is off, it displays the ON button       
            if (output3State=="off") {
              client.println("<p><a href=\"/4/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/4/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display the current state of Server control
            client.println("<p>Server Master Control State:" + server_control + "</p>");
            // If the output3State is off, it displays the ON button       
            if (server_control=="off") {
              client.println("<p><a href=\"/S/on\"><button class=\"button\">ON</button></a></p>");
            } else  { 
              client.println("<p><a href=\"/S/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            client.println("</body></html>");
            
            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
}

void loop(){
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects, a new server instance is initites
    Serial.print("Server mode - Mannual Control");
    server_loop(client); 
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }

  else if (server_control == "off") // If contontol is based on PIR Sensors
  {
    bool PIR_val_1 = digitalRead(PIR_IN_1);
    bool PIR_val_2 = digitalRead(PIR_IN_2);

    if(PIR_val_1 == HIGH)    { // If PIR Next to staircase is HIGH
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Motion Detected - 1");
      
      led_staircase();
      delay(1000);

      reset_LED();
      PIR_val_1 = 0;
    }
    
    else    { // else turn off LED's on staircase
      digitalWrite(LED_BUILTIN, HIGH);
      // Serial.println("Motion NOT Detected");
    }
     
    if(PIR_val_2 == HIGH)    { // If PIR Next in room is HIGH
      digitalWrite(LED_BUILTIN, LOW);
      digitalWrite(output3, HIGH);
      Serial.println("Motion Detected -2");
      PIR_val_2 = 0;
      delay(1000);
    }

    else    { // else turn off LED's in room
      digitalWrite(LED_BUILTIN, HIGH);
      digitalWrite(output3, LOW);
      // Serial.println("Motion NOT Detected");
    }
  }
}