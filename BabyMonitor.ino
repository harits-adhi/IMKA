#include <SoftwareSerial.h>
#define DEBUG true

int sensorPin = A0; // select the input pin for the potentiometer
int ledPin = 13; // select the pin for the LED
int sensorValue = 0; // variable to store the value coming from the sensor

SoftwareSerial ser(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  delay(5000);               
  Serial.begin(115200);
  ser.begin(115200);
  connectWifi();
  //sendDataID("10");
}

void loop() {
  // put your main code here, to run repeatedly:
  int SV = analogRead (sensorPin);
  digitalWrite (ledPin, HIGH);
  delay (100);
  digitalWrite (ledPin, LOW);
  delay (100);
  String sigval = String(SV, DEC);
  //sendDataID(sigval);
  if (SV >= 41) {
    sendDataID();
  }
}

String sendCommand(String command, const int timeout, boolean debug) {
  String response = "";
      
  ser.print(command); // send the read character to the esp8266
      
  long int time = millis();
      
  while( (time+timeout) > millis()) {
    while(ser.available()) {
      // The esp has data so display its output to the serial window 
      char c = ser.read(); // read the next character.
      response+=c;
    }  
  }
      
  if(debug) {
    Serial.print(response);
  }
      
  return response;
}

void sendDataID() {
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  
  cmd += "192.168.1.152";
  cmd += "\",80\r\n";
  sendCommand(cmd,1000,DEBUG);
  delay(5000);
  
  String cmd2 = "GET /cam/cam.php"; // Link ke skrip PHP                    
  //cmd2 += id;  
  cmd2 += " HTTP/1.1\r\n";
  cmd2 += "Host: 192.168.1.152\r\n\r\n\r\n"; // Host
  String pjg="AT+CIPSEND=";
  pjg += cmd2.length();
  pjg += "\r\n";
  
  sendCommand(pjg,1000,DEBUG);
  delay(500);
  sendCommand(cmd2,1000,DEBUG);
  delay(5000);
}

void connectWifi() {
  //Set-wifi
  Serial.print("Restart Module...\n");
  sendCommand("AT+RST\r\n",2000,DEBUG); // reset module
  delay(5000);
  Serial.print("Set wifi mode : STA...\n");
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  delay(5000);
  Serial.print("Connect to access point...\n");
  sendCommand("AT+CWJAP=\"Andromax-M2S-2B0D\",\"konfmj57bn\"\r\n",3000,DEBUG);
  delay(5000);
  Serial.print("Check IP Address...\n");
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  delay(5000);
  /*Serial.print("Ini apa...\n");
  sendCommand("AT+CIPMUX=1\r\n",2000,DEBUG); // reset module
  delay(5000);*/
}
