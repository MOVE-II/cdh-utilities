#include "SPIDevice.h"
#include "PinMapper.h"

#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

using namespace std;

const string GPIOPin::gpio_sysfs = "/sys/class/gpio/";

GPIOPin::GPIOPin(const std::string& gpioId)
{
	// pin should already be exported!
	// -> directly open pin
	this->pinPath = GPIOPin::gpio_sysfs + gpioId + "/";
	
	std::string path = this->pinPath + "value";
	this->fd = open(path.data(), O_RDWR);
}

GPIOPin::~GPIOPin()
{
	this->close();
}

bool GPIOPin::isOK() const
{
	return this->fd >= 0;
}

void GPIOPin::close()
{
	if (this->fd >= 0)
	{
		::close(this->fd);
		this->fd = -1;
	}
}

void GPIOPin::setDirection(GPIOPin::DIRECTION direction)
{
	if (!this->isOK())
	{
		return;
	}
	
	fstream file(this->pinPath + "direction");
	
	if (!file.is_open())
	{
		return;
	}
	
	switch (direction)
	{
	case GPIOPin::INPUT:
		file << "in\n";
		break;
	case GPIOPin::OUTPUT:
		file << "out\n";
		break;
	case GPIOPin::DIRECTION_FAILED:
		break;
	}
}

GPIOPin::DIRECTION GPIOPin::getDirection()
{
	if (!this->isOK())
	{
		return GPIOPin::DIRECTION_FAILED;
	}
	
	fstream file(this->pinPath + "direction");
	
	if (!file.is_open())
	{
		return GPIOPin::DIRECTION_FAILED;
	}
	
	std::string value;
	file >> value;
	
	GPIOPin::DIRECTION direction = GPIOPin::DIRECTION_FAILED;
	if (value.size() > 0)
	{
		if (value[0] == 'o')
		{
			direction = GPIOPin::OUTPUT;
		}
		else if (value[0] == 'i')
		{
			direction = GPIOPin::INPUT;
		}
	}
	
	return direction;
}

bool GPIOPin::readPin()
{
	if (!this->isOK())
	{
		return false;
	}
	
	lseek(this->fd, 0, SEEK_SET);
	
	char value;
	ssize_t ret = read(this->fd, &value, 1);
	if (ret < 0)
	{
		return false;
	}
	
	return value == '1';
}

bool GPIOPin::setPin(bool value)
{
	if (!this->isOK())
	{
		return false;
	}
	
	char data[] = { '0', '\n' };
	if (value)
	{
		data[0] = '1';
	}
	
	lseek(this->fd, 0, SEEK_SET);
	ssize_t ret = write(this->fd, data, 2);
	if (ret < 0)
	{
		return false;
	}
	
	return true;
}

bool GPIOPin::setEdgeTrigger(GPIOPin::EDGE_TRIGGER trigger)
{
	if (!this->isOK())
	{
		return false;
	}
	
	fstream file(this->pinPath + "edge");
	
	if (!file.is_open())
	{
		return false;
	}
	
	switch (trigger)
	{
	case GPIOPin::TRIGGER_NONE:
		file << "none\n";
		break;
	case GPIOPin::TRIGGER_RISING:
		file << "rising\n";
		break;
	case GPIOPin::TRIGGER_FALLING:
		file << "falling\n";
		break;
	case GPIOPin::TRIGGER_BOTH:
		file << "both\n";
		break;
	case GPIOPin::TRIGGER_FAILED:
		break;
	}
	
	return true;
}

GPIOPin::EDGE_TRIGGER GPIOPin::getEdgeTrigger()
{
	if (!this->isOK())
	{
		return GPIOPin::TRIGGER_FAILED;
	}
	
	fstream file(this->pinPath + "edge");
	
	if (!file.is_open())
	{
		return GPIOPin::TRIGGER_FAILED;
	}
	
	std::string value;
	file >> value;
	
	GPIOPin::EDGE_TRIGGER trigger = GPIOPin::TRIGGER_NONE;
	if (value.size() > 0)
	{
		switch (value[0])
		{
		case 'n': trigger = GPIOPin::TRIGGER_NONE; break;
		case 'r': trigger = GPIOPin::TRIGGER_RISING; break;
		case 'f': trigger = GPIOPin::TRIGGER_FALLING; break;
		case 'b': trigger = GPIOPin::TRIGGER_BOTH; break;
		}
	}
	
	return trigger;
}

bool GPIOPin::waitForEdge()
{
	if (!this->isOK())
	{
		return false;
	}
	
	struct pollfd fdset[1];
	fdset[0].fd = this->fd;
	fdset[0].events = POLLPRI;
	fdset[0].revents = 0;
	
	poll(fdset, 1, 10000);
	
	return true;
}

const std::string& GPIOPin::getPinPath()
{
	return this->pinPath;
}
