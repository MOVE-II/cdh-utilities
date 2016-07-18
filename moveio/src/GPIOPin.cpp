#include "SPIDevice.h"
#include "PinMapper.h"

using namespace std;

const string GPIOPin::gpio_sysfs = "/sys/class/gpio/";

GPIOPin::GPIOPin(const std::string& gpioId)
{
	// pin should already be exported!
	// -> directly open pin
	this->pinPath = GPIOPin::gpio_sysfs + gpioId + "/";
	this->fileHandle.reset(new fstream(this->pinPath + "value"));
}

GPIOPin::~GPIOPin()
{
}

bool GPIOPin::isOK()
{
	return this->fileHandle != nullptr && this->fileHandle->is_open();
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
	
	this->fileHandle->seekg(0);
	
	int value;
	(*this->fileHandle) >> value;
	
	return value != 0;
}

void GPIOPin::setPin(bool value)
{
	if (!this->isOK())
	{
		return;
	}
	
	this->fileHandle->seekp(0);
	(*this->fileHandle) << (value ? 1 : 0) << '\n';
	this->fileHandle->flush();
}

const std::string& GPIOPin::getPinPath()
{
	return this->pinPath;
}
