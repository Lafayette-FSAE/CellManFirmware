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

notes = {
	'C_MIDDLE': 262,
	'D': 294,
	'E': 330,
	'F': 349,
	'G': 392,
	'A': 440,
	'B': 494,
	'C': 523,
}

song = ['C_MIDDLE', 'C_MIDDLE', 'G', 'G', 'A', 'A', 'G', 'F', 'F', 'E', 'E', 'D', 'D', 'C_MIDDLE']
# note_lengths = ['']

def twinkle(volume=20):
	address = 8

	for note in song:
		frequency = notes[note] * 2
		print(frequency)
		balance(8, True)
		freq(8, frequency)
		duty(8, volume)
		time.sleep(0.5)
		balance(8, False)
		time.sleep(0.1)

def balance(address, enable):
	if enable:
		value = 0x01
	else:
		value = 0x00

	send(address, [0x11, 0x00, value])

def duty(address, duty):
	send(address, [0x12, 0x00, int(duty)])

def freq(address, frequency):
	freq = frequency

	msb = int(freq) // 256
	lsb = int(freq) %  256



	send(address, [0x13, msb, lsb])

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


def poll(address):
	while True:
		data = get_data(address)
		
		print(data)
		print(calibrate_voltage(data[1]))
		print(calibrate_cell_minus(data[0]))

		print()
		time.sleep(0.2)