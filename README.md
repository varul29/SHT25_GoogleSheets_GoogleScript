# SHT25_GoogleSheets_GoogleScript
Sending SHT25 Sensor data to google spreadsheet by using the gscript editor as a bridge

# Hardware
 - [Adafruit Huzzah ESP8266](https://www.thingbits.net/products/adafruit-huzzah-esp8266-wifi-breakout-board?utm_source=google&utm_medium=cpc&gclid=Cj0KCQiA3IPgBRCAARIsABb-iGK_yrh94I-yVBeadNt0Hj_m0wOOg0dfDr81zkfdsZZ1O1vGUczu-vkaAqOIEALw_wcB)
 - [SHT25 Sensor](https://store.ncd.io/product/sht25-humidity-and-temperature-sensor-%C2%B11-8rh-%C2%B10-2c-i2c-mini-module/)
 - [Adafruit Huzzah I2C adapter](https://store.ncd.io/product/i2c-shield-for-adafruit-huzzah-esp8266-integrated-usb-and-i2c-port/)
 - [I2C Cable](https://store.ncd.io/product/i%C2%B2c-cable/)

# How stuff works

Fetching the readings of real-time data of sensors through ESP8266 and send the data to different Cloudplatforms is much simple.

We are going to use Wire.h library in Arduino IDE for creating the two wired communication between Adafruit Huzzah Board and SHT25 sensor I2C module and I2C cable.

### Note: To avoid the complex wiring structure, I am going to use the I2C adapter for Adafruit Huzzah designed to connect the I2C sensor.

For the newbie to setup the Esp8266 yo must go through the [ESP8266 setup](https://github.com/varul29/ESP8266/blob/master/README.md).

   - First Initialize the the libraries:
        - I2C :  #include <Wire.h>
        - ESP8266 WiFi : #include <ESP8266WiFi.h>
        - HTTPS WiFi Client : #include <WiFiClientSecure.h>
 
# I2C module Execution Process in Arduino IDE

After Initializing libraries, we will be defining the I2C process to be used to fetch the sensor readings convert them and shift the 8 bits data as per the requirements:

- Initialize the registers in two wire I2C protocol for the I2C sensor module 

      #define Addr 0x40

- Start I2C transmission and initialize the registers and request for 2 bytes data from where we will read the sensor data.
- If 2 bytes data will be available then read the sensor data and using mentioned below formulas we are converting the desired values 
      
      float humidity = (((data[0] * 256.0 + data[1]) * 125.0) / 65536.0) - 6; 
      float cTemp = (((data[0] * 256.0 + data[1]) * 175.72) / 65536.0) - 46.85;
      float fTemp = (cTemp * 1.8) + 32;
      
- Print the values in a serial monitor screen

## ESP8266 connection with WiFi and google spreadsheet
After the execution  I2C modules we are going to learn about how to fetch the data and by using WiFi libraries and host id as well as API keys to send the data to google sheets.

- Define WiFi credentials globally  in ESP8266 which will help us to connect the board with internet 
- As we are going to use the HTTP client and will be defining HTTPS = 443 protocol for securing the HTTP path as the script will working in the secure path only.
- Initialize the host details in code 
  
      const char* host = "script.google.com";
      const int httpsPort = 443; 
      String SCRIPT_ID = "<GScript Link ID>";


### Note: Script ID is mentioned in “web apps URL” while Gscript code will be published, just copy and paste the id mentioned below and initialize in above commands :

        /s/<Script ID>/exec

- Also using volatile with variable, we are going to initialize variable globally which fetch the data from the I2C module and send it to the URL script which will further send the data to the destination.
- Using ESP8266 WiFi Library we will be able to connect the board with internet 
- The sensor data will be hosted to a local server after every 5s.
- With the help of URL script, the data will be hosted to google script published link Active page after every 15 seconds.

# Automate the google sheet using GScript Editor 

As we all have a google account to sign in to google sheet with your account
 - Mention the values which you need to get from the sensor connected with ESP8266
 
 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/Add%20sheet%20value.PNG)
 
 - Go to Tools>Script Editor
 - In Script Editor page mention the name 
 
 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/Script%20editor.PNG)
 
 - Use "Doget" function to receive the events
 - In "Doget" function to initialize the spreadsheet API key as well as connect the Active Sheet where you want to send the sensor values
 
 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/spread%20sheet%20link.PNG)
 
 - With the help of automation function to mentioned in the code to represent the data in rows and columns easily.
 - Finally, save the data and click on “Publish”>> Click “Deploy as web apps”
 
 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/Publish.PNG)
 
 - Make sure whenever there will be any change to select the “project version” >> “New”>> press “update”
 
 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/Project%20vision.PNG)

Current web app URL will appear as below :

        https://script.google.com/macros/s/”<GScript API Key>”/exec:
        
 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/Scipt%20API%20key.PNG)

further used in ESP8266 code for fetching the data from sensors

# Hosting connections 

We are going to use the HTTPS Get request to connect the data to the host id mentioned in the gscript editor where we coded our data further to connect with google sheet.

        function doGet(e) { 
        Logger.log( JSON.stringify(e) );  // view parameters
        var result = 'Ok'; // assume success
        if (e.parameter == 'undefined') 
        {
          result = 'No Parameters';
          }
        else {
        var sheet_id = '<google spreadsheet Api Key>';       // Spreadsheet ID
        var sheet = SpreadsheetApp.openById(sheet_id).getActiveSheet();
        var newRow = sheet.getLastRow() + 1;                        
        var rowData = [];
         }
        Logger.log(JSON.stringify(rowData));
        // Write new row below
        var newRange = sheet.getRange(newRow, 1, 1, rowData.length);
        newRange.setValues([rowData]);
        }

# Limitations:

 - This project is only limited to store the data of I2C sensor in google sheets
 - We are using HTTPS GET request to receive the values through I2C functions
 - We have to change the value in string format and then send the data to gscript URL link.

# Output

 ![alt text](https://github.com/varul29/SHT25_GoogleSheets_GoogleScript/blob/master/GScript_SHT25_Snaps/Output.PNG)
