import machine
import time

# bus = machine.I2C(scl=machine.Pin(15), sda=machine.Pin(2))
bus = machine.I2C(scl=machine.Pin(18), sda=machine.Pin(19))

def send(address, data):
	data_bytes = bytearray(data)
	bus.writeto(address, data_bytes)


addresses = bus.scan()

print("current addresses on bus:")
print(addresses)

power_supply = 3.5

def read(address, number):
	output = []
	data_bytes = bus.readfrom(address, number)

	for byte in data_bytes:
		output.append( int(byte) )

	return output


def adc_to_voltage(byte_high, byte_low):
	adc_value = (byte_high * 256) + byte_low

	voltage = (adc_value * 5) / 1024

	return round(voltage, 2)

def get_voltage(pos_high, pos_low):
	data_bytes = bus.readfrom(8, 11)
	voltage = adc_to_voltage(byte_high=data_bytes[pos_high], byte_low=data_bytes[pos_low])

	return voltage

def volts_to_temp(voltage):
	return (voltage * 30.14) - 16.6

def temp(address):
	data = bus.readfrom(address, 5)
	temp = (data[4] * 256) + data[3]

	return temp / 10

def cell_voltage(address):
	data = bus.readfrom(address, 3)
	voltage = (data[2] * 256) + data[1]

	return voltage

def calibrate_voltage(raw_value):
	pin_voltage = (raw_value / 1024) * power_supply # 10bit ADC on 3.3V supply
	cell_voltage = (pin_voltage * 2) - 1

	return cell_voltage

def calibrate_cell_minus(raw_value):
	pin_voltage = (raw_value / 1024) * power_supply
	minus_voltage = pin_voltage * (1 / 0.16)

	return minus_voltage

def get_data(address):
	data_bytes = bus.readfrom(address, 11)

	bal_current_raw = (data_bytes[1] * 256) + data_bytes[0]
	bal_feedback_raw = (data_bytes[3] * 256) + data_bytes[2]
	cell_temp_raw = (data_bytes[5] * 256) + data_bytes[4]
	cell_minus_raw = (data_bytes[7] * 256) + data_bytes[6]
	cell_voltage_raw = (data_bytes[9] * 256) + data_bytes[8]
	
	write_register = data_bytes[10]

	return [bal_current_raw, bal_feedback_raw, cell_temp_raw, cell_minus_raw, cell_voltage_raw, write_register]

def poll(address):
	while True:
		data = get_data(address)
		
		print(data)
		print(calibrate_voltage(data[1]))
		print(calibrate_cell_minus(data[0]))

		print()
		time.sleep(0.2)