#include "I2CDevice.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

//#include <linux/i2c-dev.h>
#include "i2c-dev.h"

I2CDevice::I2CDevice(const std::string& devicePath, int address)
{
	this->fd = open(devicePath.data(), O_RDWR);

	if (this->isOK())
	{
		if (ioctl(this->fd, I2C_SLAVE, address) < 0)
		{
			this->close();
		}
	}
}

I2CDevice::~I2CDevice()
{
	this->close();
}

bool I2CDevice::isOK() const
{
	return this->fd >= 0;
}

void I2CDevice::close()
{
	if (this->isOK())
	{
		::close (this->fd);
		this->fd = -1;
	}
}

// bool I2CDevice::write(const char* data, int size) const
// {
// 	if (!this->isOK())
// 	{
// 		return false;
// 	}
//
// 	if (::write(this->fd, data, size) != size)
// 	{
// 		return false;
// 	}
//
// 	return true;
// }

// bool I2CDevice::read(char* data, int size) const
// {
// 	if (!this->isOK())
// 	{
// 		return false;
// 	}
//
// 	if (::read(this->fd, data, size) != size)
// 	{
// 		return false;
// 	}
//
// 	return true;
// }

bool I2CDevice::read_byte(__u8 command, __u8& value) const
{
	if (!this->isOK())
	{
		return false;
	}

	__s32 ret = ::i2c_smbus_read_byte_data(this->fd, command);
	if (ret < 0)
	{
		return false;
	}

	value = (__u8) ret;
	return true;
}

bool I2CDevice::write_byte(__u8 command, __u8 value) const
{
	if (!this->isOK())
	{
		return false;
	}

	__s32 ret = ::i2c_smbus_write_byte_data(this->fd, command, value);
	if (ret < 0)
	{
		return false;
	}

	return true;
}

bool I2CDevice::read_word(__u8 command, __u16& value) const
{
	if (!this->isOK())
	{
		return false;
	}

	__s32 ret = ::i2c_smbus_read_word_data(this->fd, command);
	if (ret < 0)
	{
		return false;
	}

	value = (__u16) ret;
	return true;
}

bool I2CDevice::write_word(__u8 command, __u16 value) const
{
	if (!this->isOK())
	{
		return false;
	}

	__s32 ret = ::i2c_smbus_write_word_data(this->fd, command, value);
	if (ret < 0)
	{
		return false;
	}

	return true;
}

__s32 I2CDevice::read_block(__u8 command, __u8 length, __u8* values) const
{
	if (!this->isOK() || values == nullptr)
	{
		return -1;
	}

	__s32 ret = ::i2c_smbus_read_i2c_block_data(this->fd, command, length, values);
	
	return ret;
}

bool I2CDevice::write_block(__u8 command, __u8 length, __u8* values) const
{
	if (!this->isOK() || values == nullptr || length > 32)
	{
		return false;
	}

	__s32 ret = ::i2c_smbus_write_i2c_block_data(this->fd, command, length, values);
	if (ret < 0)
	{
		return false;
	}

	return true;
}
