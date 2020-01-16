import machine
import time

print("scanning...")


bus = machine.I2C(scl=machine.Pin(15), sda=machine.Pin(2))


def get_data(address):
	data_bytes = bus.readfrom(address, 4)

	temp_data = (data_bytes[1] * 256) + data_bytes[0]
	cell_data = (data_bytes[3] * 256) + data_bytes[2]

	return (cell_data, temp_data)

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
			cell_minus, temp = get_data(cellman)
			temp_int = int(temp)
			cell_minus_int = int(cell_minus)
			print('{} \t temp: {} \t voltage: {}'.format(cellman, temp_int, cell_minus_int))
			print()

		time.sleep(0.5)