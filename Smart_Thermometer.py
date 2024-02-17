from machine import Pin
from gpio_lcd import GpioLcd
from machine import I2C
from binascii import hexlify
from mqttclient import MQTTClient
import network
import sys
import time
 


# Check wifi connection
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
ip = wlan.ifconfig()[0]
if ip == '0.0.0.0':
    print("no wifi connection")
    sys.exit()
else:
    print("connected to WiFi at IP", ip)


# Upload Room Temperature data collected by LSM6DSO to ESP32 
# Then Print Temperature value to the LCD screen
    # Create the LCD object
lcd = GpioLcd(rs_pin=Pin(19),
              enable_pin=Pin(23),
              d4_pin=Pin(18),
              d5_pin=Pin(17),
              d6_pin=Pin(16),
              d7_pin=Pin(15),
              num_lines=2, num_columns=16)
              
    # Create I2C object for LSM6DSO
i2c = I2C(1,scl=Pin(22),sda=Pin(23),freq=400000)




for i in range(len(i2c.scan())):
	print(hex(i2c.scan()[i]))




def WHOAMI(i2caddr):
	whoami = i2c.readfrom_mem(i2caddr,0x0F,1)
	print(hex(int.from_bytes(whoami,"little")))


def Temperature(i2caddr):
	temperature = i2c.readfrom_mem(i2caddr,0x20,2)
	if int.from_bytes(temperature,"little") > 32767:
		temperature = int.from_bytes(temperature,"little")-65536
	else:
		temperature = int.from_bytes(temperature,"little")
	print("%4.2f" % ((temperature)/(256) + 25))




buff=[0xA0]
i2c.writeto_mem(i2c.scan()[i],0x10,bytes(buff))
i2c.writeto_mem(i2c.scan()[i],0x11,bytes(buff))
time.sleep(0.1)




try:
	while(1):
		#WHOAMI(i2c.scan()[i])
		Temperature(i2c.scan()[i])
		time.sleep(1)


except KeyboardInterrupt:
	i2c.deinit()
	pass


while True:
    
    lcd.cursor_pos = (0, 0)
    lcd.write_string("Current Room Temperature is " + Temperature() + unichr(223) + "C")
    lcd.cursor_pos = (1, 0)


# Conect ESP32 to IFTTT protocol
# Set up Adafruit connection
adafruitIoUrl = 'io.adafruit.com'
adafruitUsername = 'jisoo'
adafruitAioKey = 'aio_foeg88q1sQINjPaqVmv7wLltOsio'


# Define callback function
def sub_cb(topic, msg):
    print((topic, msg))


# Connect to Adafruit server
print("Connecting to Adafruit")
mqtt = MQTTClient(adafruitIoUrl, port='1883', user=adafruitUsername, password=adafruitAioKey)
time.sleep(0.5)
print("Connected!")


# This will set the function sub_cb to be called when mqtt.check_msg() checks
# that there is a message pending
mqtt.set_callback(sub_cb)


currentTemp = Number(temperature)


if currentTemp > 70:
   Sms.sendMeText.skip(`Temperature is ${currentTemp}`)
   testMessage = "Temperature is ${currentTemp}"
break


# Send test message
feedName = "jisoo/feeds/jisoos-feed"
mqtt.publish(feedName,testMessage)
print("Published {} to {}.".format(testMessage,feedName))


mqtt.subscribe(feedName)
for i in range(0, 60):
    mqtt.check_msg()
    time.sleep(1)
