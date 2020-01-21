import machine
import time

print("scanning...")


bus = machine.I2C(scl=machine.Pin(15), sda=machine.Pin(2))


def get_data(address):
	data_bytes = bus.readfrom(address, 5)

	temp_data = (data_bytes[1] * 256) + data_bytes[0]
	cell_data = (data_bytes[3] * 256) + data_bytes[2]

	duty_cycle = data_bytes[3]

	return (cell_data, temp_data, duty_cycle)

def scan():
	while True:
		print("sending...")
		# bus.writeto(50, b\'123\')
		# print("scanning...")
		result = bus.scan()
		print(result)
		time.sleep(1)

def get_duty(address):

	_, _, duty_cycle = get_data(address)
	return duty_cycle

def set_duty(address, duty):
	bus.writeto(address, duty);

def get_cell_voltage(address):
	data = get_data(address)
	voltage, _, _, = data

	voltage = ((voltage / 1024) * 5) - 1

	return voltage

def voltage_test():
	while True:
		voltage = get_cell_voltage(12)
		print('{} V'.format(voltage))
		time.sleep(.1)


def begin():
	while True:
		cellmen = bus.scan()
		print('Addresses:', cellmen)
		print()

		for cellman in cellmen:
			cell_minus, temp, duty_cycle = get_data(cellman)
			temp_int = int(temp)
			cell_minus_int = int(cell_minus)
			print('{} \t temp: {} \t voltage: {}'.format(cellman, temp_int, cell_minus_int))
			print(duty_cycle)
			print()

		time.sleep(0.5)