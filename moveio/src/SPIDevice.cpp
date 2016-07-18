#include "SPIDevice.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/file.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <cstring>
#include <iostream>

#include "PinMapper.h"

using namespace std;

SPIDevice::SPIDevice(const std::string& devicePath, SPIMODE mode) :
	mode(mode)
{
	std::string realpath = SPIDevice::getSpiDevicePath(devicePath);
	this->fd = open(realpath.data(), O_RDWR);
	
	this->custom_cs = SPIDevice::getCustomCS(devicePath);
	
	this->speed_hz = this->getMaxSpeed();
	this->delay_usecs = 0;
	this->bits_per_word = 8;
}

SPIDevice::~SPIDevice()
{
	this->close();
}

std::string SPIDevice::getSpiDevicePath(const std::string& path)
{
	std::size_t index = path.find('#');
	
	if (index != std::string::npos)
	{
		return path.substr(0, index);
	}
	else
	{
		return path;
	}
}

std::unique_ptr<GPIOPin> SPIDevice::getCustomCS(const std::string& path)
{
	std::size_t index = path.find('#');
	
	if (index != std::string::npos)
	{
		const std::string key = path.substr(index+1);

		auto pin = std::unique_ptr<GPIOPin>(new GPIOPin(PinMapper::find(key)));

		if (!pin->isOK())
		{
			std::cerr << "WARNING: could not open pin '" << key << "'!\n";
		}
		else
		{
			if (pin->getDirection() != GPIOPin::OUTPUT)
			{
				std::cerr << "WARNING: chip select pin '" << key << "' not set as output yet!\n";
				pin->setDirection(GPIOPin::OUTPUT);
			}

			if (pin->readPin() != true)
			{
				pin->setPin(true);
				std::cerr << "WARNING: chip select pin '" << key << "' not set to high value!\n";
			}
		}

		return pin;
	}
	else
	{
		return nullptr;
	}
}

bool SPIDevice::isOK() const
{
	return this->fd >= 0;
}

void SPIDevice::close()
{
	if (this->custom_cs != nullptr)
	{
		this->custom_cs->setPin(true);
		this->custom_cs = nullptr;
	}
	
	if (this->fd > 0)
	{
		::close(this->fd);
		this->fd = -1;
	}
}

int SPIDevice::getMaxSpeed() const
{
	if (this->isOK())
	{
		int speed;
		
		if (ioctl(this->fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) == 0)
		{
			return speed;
		}
	}
	
	return 100000; // 100 khz
}

int SPIDevice::getSpeed() const
{
	return this->speed_hz;
}

void SPIDevice::setSpeed(int speed_hz)
{
	this->speed_hz = speed_hz;
}

bool SPIDevice::transfer(void* rx, const void* tx, int size) const
{
	if (this->isOK())
	{
		if (this->custom_cs != nullptr)
		{
			if (flock(this->fd, LOCK_EX) != 0)
			{
				return false;
			}
			
			this->custom_cs->setPin(false);
		}
		
		struct spi_ioc_transfer tr;
		tr.tx_buf = reinterpret_cast<__u64>(tx);
		tr.rx_buf = reinterpret_cast<__u64>(rx);
		tr.len = size;
		tr.delay_usecs = this->delay_usecs;
		tr.speed_hz = this->speed_hz;
		tr.bits_per_word = this->bits_per_word;
		
		int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
		
		if (this->custom_cs != nullptr)
		{
			this->custom_cs->setPin(true);
			flock(this->fd, LOCK_UN);
		}
		
		return ret >= 1;
	}
	
	return false;
}

bool SPIDevice::read(void* rx, int size) const
{
	if (this->isOK())
	{
		// return this->transfer(nullptr, rx, size);
		
		// on the BeagleBone you have to provide tx_buf, it is not optional!
		// -> zero rx buffer and pass it as tx_buf
		memset(rx, 0, size);
		
		return this->transfer(rx, rx, size);
	}
	
	return false;
}

bool SPIDevice::write(const void* tx, int size) const
{
	if (this->isOK())
	{
		return this->transfer(nullptr, tx, size);
	}

	return false;
}
