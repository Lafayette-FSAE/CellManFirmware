import machine
import time

print("scanning...")


bus = machine.I2C(scl=machine.Pin(2), sda=machine.Pin(15))

# print("scanning...")
# result = bus.scan()
# print(result)

def scan():
	while True:
		print("sending...")
		# bus.writeto(50, b\'123\')
		# print("scanning...")
		result = bus.scan()
		print(result)
		time.sleep(1)



def begin():
	while True:
		cellmen = bus.scan()
		print('Addresses:', cellmen)
		print()

		for cellman in cellmen:
			temp = bus.readfrom(cellman, 1)
			temp_int = int(temp[0])
			print(cellman, ' ', temp_int)
			print()

		time.sleep(0.5)

# blink
# import time
# from machine import Pin
# led=Pin(15, Pin.OUT)

# while True:
# 	led.value(1)
# 	time.sleep(0.5)
# 	led.value(0)
# 	time.sleep(0.5)'
