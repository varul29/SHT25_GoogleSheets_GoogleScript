# SHT25_GoogleSheets_GoogleScript
Sending SHT25 Sensor data to google spreadsheet without using gscript editor as bridge

# How stuff works

Create the compatibility to fetch the readings of real-time data of sensors through ESP8266 is much simple.

We are going to use Wire.h library in Arduino IDE for creating the two wired communication between Adafruit Huzzah Board and SHT25 sensor I2C module.
Note: To avoid the complex wiring structure, I am going to use the I2C adapter for Adafruit Huzzah designed to connect the I2C sensor.
For the newbie to setup the Esp8266 yo must go through the tutorial setup.

   - First Initialize the the libraries:
        - I2C :  #include <Wire.h>
        - ESP8266 WiFi : #include <ESP8266WiFi.h>
        - HTTPS WiFi Client : #include <WiFiClientSecure.h>
 
# I2C module Execution Process in Arduino IDE

First We will be defining the I2C process to be use to fetch the sensor readings convert the and shift the 8 bits data as per the requirements:

- Initialize the registers in two wire I2C protocol for I2C sensor module 

      #define Addr 0x40

- Start I2C transmission and initialize the registers and request for 2 bytes data from where we will read the sensor data.
- If 2 bytes data will be available then we will read the sensor data and using mentioned below formulas we are converting the desired values 
- Print the values in serial monitor screen

## ESP8266 connection with WiFi and google spreadsheet
After the execution  I2C modules we are going to learn about how to fetch the data and by using WiFi libraries  and host id as well as API keys to send the data to google sheets.

- Define WiFi credentials globally  in ESP8266 which will help us to connect the board with internet 
- As we are going to use the HTTP client and will be defining HTTPS = 443 protocol for securing the HTTP path as script will working in secure path only.
- Initialize the host details in code 
  
      hostid  = "script.google.com"
      const char* fingerprint = "copy paste similar values mention in code";
      String SCRIPT_ID = " Replace by your Gscript service id “

### Note: Script ID is mentioned in “webapp URL” while Gscript code will be published, just copy and paste the id mentioned below and initialize in above commands :

        /s/<Script ID>/exec

- Also using volatile with variable, we are going to initialize variable globally which fetch the data from the I2C module and send it to the url script which will further send the data to destination.
- Using ESP8266 WiFi Library we will be able to connect the board with internet 
- The sensor data will be hosted to local server after every 5s.
- With the help of URL script the data will be hosted to google script published link Active page after every 15 seconds.

# Automate the google sheet using GScript Editor 

As we all have google account so sign in to google sheet with your account
 - Mention the values which you need to get from the sensor connected with ESP8266
 - Go to Tools>Script Editor
 - In Script Editor page mention the name 
 - Use Doget function to receive the events
 - In do get function initialize the spreadsheet API key as well as connect the Active Sheet where you want to send the sensor values
 - With the help of automation function to mentioned in code to represent the data in rows and columns easily.
 - Finally save the data and click on “Publish”>> Click “Deploy as web apps”
 - Make sure whenever there will be any change do select the “project version” >> “New”>> press “update”

Current web app URL will appear as below :

        https://script.google.com/macros/s/”<GScript API Key>”/exec:

further used in ESP8266 code for fetching the data from sensors

# Hosting connections 

We are going to use the HTTPS Get request to connect the data to the host id mentioned in gscript editor where we coded our data further to connect with google sheet.

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

 - This project is only limitize to store the data of I2C sensor in google sheets
 - We are using HTTPS GET request to receive the values through I2C functions
 - We have to change the value in string format and then send the data to gscript url link.

