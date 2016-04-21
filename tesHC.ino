#include <SoftwareSerial.h>
#define DEBUG true

SoftwareSerial ser(10, 11); // RX, TX

void setup() {
  // put your setup code here, to run once:
  delay(5000);               
  Serial.begin(115200);
  ser.begin(115200);
  connectWifi();
}

void loop() {
  // put your main code here, to run repeatedly:
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

void connectWifi() {
  //Set-wifi
  Serial.print("Restart Module...\n");
  sendCommand("AT+RST\r\n",2000,DEBUG); // reset module
  delay(5000);
  Serial.print("Set wifi mode : STA...\n");
  sendCommand("AT+CWMODE=1\r\n",1000,DEBUG); // configure as access point
  delay(5000);
  Serial.print("Connect to access point...\n");
  sendCommand("AT+CWJAP=\"TUBIS VIII/10\",\"1010101010\"\r\n",3000,DEBUG);
  delay(5000);
  Serial.print("Check IP Address...\n");
  sendCommand("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  delay(5000);
  Serial.print("Ini apa...\n");
  sendCommand("AT+CIPMUX=1\r\n",2000,DEBUG); // reset module
  delay(5000);
  Serial.print("Configuring...\n");
  sendCommand("AT+CIPSTART=4,\"TCP\",\"184.106.153.149\",80\r\n",1000,DEBUG); // get ip address
  delay(5000);
  String cmd = "GET https://api.thingspeak.com/update?api_key=1TIP1A4WH38U3VXU&field1=5;";
  String pjg = "AT+CIPSEND=";
  pjg += cmd.length();
  pjg += "\r\n";
  Serial.print("Configuring...\n");
  sendCommand(pjg,1000,DEBUG); // get ip address
  delay(5000);
  Serial.print("Updating Channel...\n");
  sendCommand(cmd,3000,DEBUG);
  delay(5000);
}
