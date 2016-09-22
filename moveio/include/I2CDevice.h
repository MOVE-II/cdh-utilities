#ifndef MOVEIO_I2C_DEVICE_H
#define MOVEIO_I2C_DEVICE_H

#include <string>
#include <linux/types.h>

class I2CDevice {
public:
	I2CDevice(const std::string& devicePath, int address);
    ~I2CDevice();
	
	/**
		Returns true when this I2CDevice is open and can be used to transfer data.
	*/
	bool isOK() const;

	/**
		Closes this I2CDevice in case it was ok. After that operation,
		the I2CDevice is not ok anymore and can't be used for further accesses.
	*/
	void close();
	
	/**
		equivalent to: i2c_smbus_read_byte_data
		Reads one byte from I2C register at "command". The result gets
		stored into "value".

		Returns true on success, false on failure.
	*/
	bool read_byte(__u8 command, __u8& value) const;

	/**
		equivalent to: i2c_smbus_write_byte_data
		Writes the byte "value" to I2C register at "command".

		Returns true on success, false on failure.
	*/
	bool write_byte(__u8 command, __u8 value) const;

	/**
		equivalent to: i2c_smbus_read_word_data
		Reads one word from I2C register at "command". The result gets
		stored into "value".

		Returns true on success, false on failure.
	*/
	bool read_word(__u8 command, __u16& value) const;

	/**
		equivalent to: i2c_smbus_write_word_data
		Writes the word "value" to I2C register at "command".

		Returns true on success, false on failure.
	*/
	bool write_word(__u8 command, __u16 value) const;

	/**
		equivalent to: i2c_smbus_read_block_data
		Reads the block register "command" and writes the data to the
		values array. The length of the data is returned in bytes.

		The values array must be able to store the whole content of
		the register. To be safe, it's size should be 32 bytes.

		Returns -1 on failure.
	*/
	__s32 read_block(__u8 command, __u8 length, __u8* values) const;

	/**
		equivalent to: i2c_smbus_write_block_data
		Writes "length" bytes stored in "values" to the register
		"command". In case length is bigger than 32, only 32 bytes
		are transferred.

		Returns true on success, false on failure.
	*/
	bool write_block(__u8 command, __u8 length, __u8* values) const;

private:
    int fd;
};


#endif //MOVEIO_I2C_DEVICE_H

