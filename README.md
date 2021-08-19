# COVID Daily HealthChecker
This repository contains the Arduino source code for the Daily HealthChecker.

Fever is a key indicator of a COVID-19 infection.  People often are asymptomatic through the initial period and it’s critical to effectively measure body temperature for signs of mild fever. The COVID Daily HealthChecker is a tool that allows for a controlled measurement of body temperature which can be validated by health professionals to rule out false positives.

## Project Link
For more information about this project, please visit: https://bit.ly/covid-daily-checker

## Required Hardware
- Arduino Mega 2560 
- Arduino Ethernet Shield to communicate with APIs and store measurements on a local SD card 
- Sunfounder Humiture Sensor (with DHT11 sensor) for accurate measurement of room humidity and body temperature 
- Sunfounder Touch Switch to allow the temperature sensing mode to be turned in a controlled, repeatable process 
Sainsmart LCD (I2C LCD1602) 
A router to provide network connection to the internet. In my case, I used my phone as an internet hotspot. 
