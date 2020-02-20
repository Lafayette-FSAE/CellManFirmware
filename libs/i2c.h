#ifndef __I2C
#define __I2C

#include <stdint.h>
#include "stm8.h"

void i2c_init(int address){
	
	rim(); // enable interrupts

	I2C_CR1 = 0;					// Disable I2C peripheral in order to modify it
	I2C_FREQR = 16;					//  Set the internal clock frequency (MHz).

	// I2C_CCRH_F_S = 0;				//  I2C running is standard mode.
	// I2C_CCRH_CCR = 0x00;

	I2C_CCRH &= 0b01110000;
	I2C_CCRL = 0xa0;				//  SCL clock speed is 50 KHz.

	// I2C_OARH_ADDMODE = 0;           //  7 bit address mode.
	// I2C_OARH_ADD = 0;               //  Set this device address to be 0x50.
	// I2C_OARH_ADDCONF = 1;           //  Docs say this must always be 1.

	I2C_OARH &= 0b01111001;
	I2C_OARH |= 0b01000001;

	I2C_OARL = address << 1;		//  Set this device address.

	// I2C_OARL = 10 << 1;

	I2C_TRISER = 17;

	// I2C_ITR_ITBUFEN = 1;            //  Buffer interrupt enabled.
	// I2C_ITR_ITEVTEN = 1;            //  Event interrupt enabled.
	// I2C_ITR_ITERREN = 1;

	I2C_ITR &= 0b1111111;
	I2C_ITR |= 0b0000111;

	I2C_CR1 |= 0b00000001;					// re-enable peripheral

	I2C_CR2 |= 0b00000100;
}

// An array of 256 registers that can be written
// by the i2c bus. A register's address
// corresponds to its position in this
// array, expressed as hex
int16_t i2c_registers[256];

// The address of the register the master
// intends to write to. This is always set by
// the first byte in the 2 byte write sequence
// and is reset to 0 
unsigned char register_to_write = 0;

int write_buffer_index = 0;
unsigned char write_buffer[3];

void on_byte_received(unsigned char byte){

	// if register_to_write is set to 0,
	// then assume this is the first byte in the write sequence
	// and treat it as the address,
	//
	// otherwise, treat it as the data to be written
	// and write it to whatever is currently in register_to_write

	if(register_to_write == 0){
		register_to_write = byte;
	} else {
		write_buffer[write_buffer_index] = byte;
		write_buffer_index += 1;

		// We only expect 2 bytes at a time, so if the master
		// tries to continue writing, write to an unused
		// third byte
		if(write_buffer_index >= 2) write_buffer_index = 2;
	}
}

// Called whenever the master is done writing data
void on_write_finished(){
	// copy write_buffer to register
	int16_t data_to_write = (write_buffer[0] << 8) + write_buffer[1];
	i2c_registers[register_to_write] = data_to_write;

	// reset buffer variables
	write_buffer_index = 0;
	write_buffer[0] = 0;
	write_buffer[1] = 0;
	write_buffer[2] = 0;
	register_to_write = 0;
}

#define I2C_PACKET_SIZE 24
unsigned char data_to_transmit[I2C_PACKET_SIZE];

int data_index = 0;

unsigned char on_byte_requested(){
	unsigned char output;

	// determine current mode
	int debug_mode = i2c_registers[0x21];

	// determine packet length 
	// (9 bytes in normal mode or 24 in debug mode)
	int packet_length = debug_mode ? 9 : 24;

	// copy data from data_to_transmit
	output = data_to_transmit[data_index];

	// determine whether to increment or reset the index
	// for next time
	if(data_index < packet_length){
		data_index += 1;
	} else {
		data_index = 0;
	}

	return output;
}

// setup the i2c interrupt service routine
void i2c_inter (void) __interrupt 19 {

	// Check Address Bit
	// Occurs at start of I2C packet
	if(I2C_SR1 & 0b00000010){

		// clear ADDR by reading SR1 followed by SR3
		I2C_SR1;
		I2C_SR3;

		return;
	}

	// Check RXNE bit
	// Occurs after each byte when there is data to read in the DR register
	if(I2C_SR1 & 0b01000000){

		// reading I2C_DR also resets the RXNE bit
		// so it is important that this gets read every time
		unsigned char i2c_received_value = I2C_DR;
		on_byte_received(i2c_received_value);

		return;
	}

	// Check TXE bit
	if(I2C_SR1 & 0b10000000){

		unsigned int next_byte = on_byte_requested();
		I2C_DR = next_byte;

		return;
	}

	// Check AF bit
	// should occur when master stops requesting data
	if(I2C_SR2 & 0b00000100){

		// force data_index back to 0 to reset for next request
		data_index = 0;

		// clear AF bit by writing a 0 to it
		I2C_SR2 &= 0b11111011;

		return;
	}

	// Check STOPF bit
	// Occurs at the end of the I2C packet
	if(I2C_SR1 & 0b00010000){

		on_write_finished();

		// clear STOPF by reading SR1 then writing CR2
		I2C_SR1;

		// I2C_CR2 |= 0b00000010;
		I2C_CR2 &= 0b11111101;
		return;
	}

	// getting here likely indicates an error
	// reset the BERR bit
	I2C_SR2 &= 0b11111110;

	return;
}

#endif