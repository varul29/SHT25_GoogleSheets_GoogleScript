#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Wire.h>

String readString;

// ***Software Interrupts*** create interval for 5 seconds and execute it in main code
unsigned long Timer = 0;
unsigned long Interval = 5000;

//Define I2C address of SHT25 Sensor
#define Addr 0x40

//Define ESP8266 userid and password to connect with wifi network
const char* ssid = "ssid";
const char* password = "password

//Define hostid where data will be send in script format
const char* host = "script.google.com";
const int httpsPort = 443; // define the secure port for HTTP protocol = 443

//Define global variable
volatile float Ctemp,Ftemp,humid; // using volatile with variable - It tells the compiler that the value of the variable may change at any time--without any action being taken by the code the compiler finds nearby.
// Use WiFiClientSecure class to create TLS connection ***Not In use***
WiFiClientSecure client;
// SHA1 fingerprint of the certificate, don't care with your GAS service
const char* fingerprint = "fd 85 80 08 94 28 7b 0e 2f 13 06 09 d7 fd f0 23 40 7c e4 34";
String SCRIPT_ID = "Replace by your Gscript service idE";    

void setup() 
{ 
  // Initialise I2C communication as MASTER
  Wire.begin(2,14); //pins used for SDA, SCL connection in I2C
  Serial.begin(115200);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA); // Enables the ESP8266 works as station
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 }

void loop() 
{
 delay(300);
 Timer = millis();
 while(millis()- Timer<=Interval)// use intervels as per mentioned earlier 
 {
  temptask();// function provided for temperature sensors 
  delay(500);
 }
  sendData();
}

// Function for Send data into Google Spreadsheet
void sendData()
{
  
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
  Serial.println("certificate matches");
  } 
  else {
  Serial.println("certificate doesn't match");
  }
//***Mentioned below directly executed in String url***  
//  String tempC =  String(Ctemp, 1); 
//  String tempF =  String(Ftemp, 1); 
//  String humiD =  String(humid, 1); 

  String url = String("/macros/s/" + SCRIPT_ID + "/exec?tempC=" + String(Ctemp, 1) + "&tempF=" + String(Ftemp, 1) + "&humiD=" + String(humid, 1));
  Serial.print("requesting URL: ");
  Serial.println(url);

  //get the sensor data in script format using get method
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  while (client.connected()) {
  String line = client.readStringUntil('\n');
  Serial.println(line);// to check whether line is giving the exact same value which we mentioned or not
  }
  
  String line1 = client.readStringUntil('\n');
  
  //Check whether arduino command interface able to perform the task or not
  if (line1.startsWith("{\"state\":\"success\"")) {
  Serial.println("esp8266/Arduino CI successfull!");
  } 
  else {
  Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.println("reply was:");
  Serial.println("==========");
  Serial.println(line1);
  Serial.println("==========");
  Serial.println("closing connection");
}

void temptask()
{
unsigned int data[2];
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send humidity measurement command, NO HOLD master
  Wire.write(0xF5);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data
  // humidity msb, humidity lsb
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
  }
    // Convert the data
    float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6; 
    // Output data to Serial Monitor
    Serial.print("Relative Humidity :");
    Serial.print(humidity);
    Serial.println(" %RH");
    humid = humidity;
    
  // Start I2C transmission
  Wire.beginTransmission(Addr);
  // Send temperature measurement command, NO HOLD master
  Wire.write(0xF3);
  // Stop I2C transmission
  Wire.endTransmission();
  delay(500);

  // Request 2 bytes of data
  Wire.requestFrom(Addr, 2);

  // Read 2 bytes of data 1 byte = 8 bits total 16 bits 
  // temp msb, temp lsb
  if(Wire.available() == 2)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();  
  }

    // Convert the data and we will shift MSB value 8 bits (by multiplying with 256) to left and add it with LSB values
    
    float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
    float fTemp = (cTemp * 1.8) + 32;

    // Output data to Serial Monitor
    Serial.print("Temperature in Celsius :");
    Serial.print(cTemp);
    Serial.println(" C");
    Serial.print("Temperature in Fahrenheit :");
    Serial.print(fTemp);
    Serial.println(" F");
    Ctemp = cTemp;
    Ftemp = fTemp;
    delay(300);  
}
