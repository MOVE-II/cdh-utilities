#ifndef MOVEIO_GPIO_PIN_H
#define MOVEIO_GPIO_PIN_H

#include <string>
#include <fstream>
#include <memory>

/**
GPIOPin is used to access gpio pins through the sys-fs filesystem. This class won't fail when the given pin is not available. This can be tested using the isOK() method.

usage:
GPIOPin pin(PinMapper::find("PA_45"));

pin.setDirection(GPIOPin::OUTPUT);
pin.setPin(true);

*/
class GPIOPin {
public:
	static const std::string gpio_sysfs;
	//static const std::string gpio_export = gpio_sysfs + "export"; // export should be called before application!
	//static const std::string gpio_unexport = gpio_sysfs + "unexport";

	enum DIRECTION
	{
		INPUT, // pin is an input
		OUTPUT, // pin is an output
		DIRECTION_FAILED // reading direction failed
	};
	
	enum EDGE_TRIGGER
	{
		TRIGGER_NONE,
		TRIGGER_RISING,
		TRIGGER_FALLING,
		TRIGGER_BOTH,
		TRIGGER_FAILED
	};
	
	/**
		Creates a new GPIOPin instance and tries to open the specified gpio pin.
	*/
	GPIOPin(const std::string& gpioId);
    ~GPIOPin();
	
	/**
		Returns true when this gpio pin is ok (= pin is open and can be modified).
	*/
	bool isOK() const;
	
	/**
		Closes the this gpio pin. After that it isn't ok anymore and can't be used for further accesses.
	*/
	void close();
	
	/**
		Sets the direction of the pin to the supplied state, when this gpio pin is ok.
			GPIOPin::INPUT configures the pin as an input
			GPIOPin::OUTPUT configures the pin as an output
			every other value is ignored.
	*/
	void setDirection(GPIOPin::DIRECTION direction);
	
	/**
		Reads the current pin direction and returns GPIOPin::DIRECTION_FAILED, when this pin is not ok.
	*/
	GPIOPin::DIRECTION getDirection();
	
	/**
		Reads the current value at the pin depending on configured pin direction:
			GPIOPin::INPUT reads logic value at the pin
			GPIOPin::OUTPUT should return the last value set using setPin(...)
			returns false when this gpio pin is not ok
	*/
	bool readPin();
	
	/**
		Sets the current value at the pin. Only changes the value at the pin, when this gpio pin is ok.
		
		Returns true on success
	*/
	bool setPin(bool value);
	
	/**
		Set edge trigger.
		
		Returns true on success.
	*/
	bool setEdgeTrigger(GPIOPin::EDGE_TRIGGER trigger);
	
	/**
		Returns current edge trigger state
	*/
	GPIOPin::EDGE_TRIGGER getEdgeTrigger();
	
	/**
		Waits for edge trigger.
		This method returns when the edge trigger fires, or when 10 seconds have passed.
		
		Returns false, when the file descriptor is not open.
	*/
	bool waitForEdge();

	const std::string& getPinPath();
private:
	std::string pinPath;
	
    //std::unique_ptr<std::fstream> fileHandle;
	int fd;
};


#endif //MOVEIO_GPIO_PIN_H
