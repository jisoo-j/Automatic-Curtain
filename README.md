# Automatic-Curtain

- Photoresistor Sensor Code
Independent sensor code before integration to the motor code. We initially tested out the sensor by assigning the data through pin 36. As the brightness value is within the range between 0-4200, we created the if statement separately.

- Motor code combined with light sensor code (Arduino)
As the light sensor infinitely collected the data within the loop, we had to create a stop condition as the code actuated by if statement, we assigned 0,1 value to temp variable. With this condition, even if the brightness met every if statement within void loop, it stopped the motor after it activated to open and close the curtain. 

- Smart Thermometer Code (MicroPython)
Operate ESP32, LSM6DSO sensor, and LCD connected to MQTT protocol. The data collection will be through an LSM6DSO sensor to collect room temperature data and upload it to Wifi-connected ESP32 and the MQTT protocol. While printing the current room temperature on the LCD screen, the IFTTT will be triggered if the value reaches the threshold (70°C) and send an email to the user. 

