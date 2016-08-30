#ifndef MOVEIO_SPI_DEVICE_H
#define MOVEIO_SPI_DEVICE_H

#include <string>
#include <memory>

#include "GPIOPin.h"

/**
SPIDevice can be used to easily receive and send data over the SPI bus.

usage:
SPIDevice device("/dev/spidev1.1#P8_20", SPIDevice::MODE_2);
//device.setSpeed(100000); // uncomment to use that speed instead of maximum one.

char data[100]; // reserve buffer with size 100
device.read(data, 16); // read 16 bytes from the spi bus into the buffer
// ... process data

data[0] = 1;
data[1] = 42;
device.write(data, 2); // write [1, 42] (2 bytes) to the buffer
*/
class SPIDevice {
public:
	typedef enum SPIMODE
	{
		MODE_0 = 0, // = 0
		MODE_1 = 1, // = MODE_CPHA
		MODE_2 = 2, // = MODE_CPOL
		MODE_3 = 3, // = MODE_CPHA | MODE_CPOL
		MODE_CPHA = 1,
		MODE_CPOL = 2
	} SPIMODE;
	
	/**
		Creates a new SPIDevice instance and tries to open the given spi device.
		On success, the mode is set to the given value, and the speed is set to the maximum speed.
	*/
    SPIDevice(const std::string& devicePath, SPIMODE mode = SPIDevice::MODE_0);
    ~SPIDevice();
	
	/**
		Reads the maximum speed from the spi device node.
		Returns 100000 on failure.
	*/
	int getMaxSpeed() const;
	
	/**
		Returns the currently set speed (default 100000)
	*/
	int getSpeed() const;

	/**
		Sets the speed for the next transfer.
	*/
	void setSpeed(int speed_hz);
	
	bool setMode(SPIMODE mode);
	SPIMODE getMode() const;
	
	/**
		Returns true when this SPIDevice is open and can be used to transfer data.
	*/
	bool isOK() const;

	/**
		Closes this SPIDevice in case it was ok. After that operation,
		the SPIDevice is not ok anymore and can't be used for further accesses.
	*/
	void close();
	
	/**
		When this SPIDevice is not ok, nothing happens and false is returned.
		
		On a successfull transfer, true is returned.

		When this SPIDevice is ok, transfer exactly "size" bytes over the SPI bus.
		This does a bidirectional transfer with data going in and out at the same time.
		Operation blocks until all data have been transmit.
		
		Incoming data is written to the rx-buffer and read from the SPI bus.
		Outgoing data is read from the tx-buffer and put on the SPI bus.
		
		The rx-buffer parameter is optional and can be set to nullptr.
		The tx-buffer is mandatory (at least on BeagleBones).
		It is allowed to pass the same buffer for rx and tx.

		When a GPIOPin has been opened, a lock will be automatically placed on the SPI device node
		before the GPIOPin is set to ensure that only device can be selected at all times. After
		the transfer, the GPIOPin will be reset and the lock will be cleared.

		When there is no GPIOPin, the driver is assumed to do the locking and assertion of the chip select line.
	*/
	bool transfer(void* rx, const void* tx, int size) const;

	/**
		Does a transfer that only reads data from the SPI bus.
		As there is no tx-buffer, zeros will be transmitted.

		See SPIDevice::transfer.
	*/
	bool read(void* rx, int size) const;

	/**
		Does a transfer that only ouptuts data to the SPI bus.

		See SPIDevice::transfer.
	*/
	bool write(const void* tx, int size) const;
	
private:
    int fd;
	
	int speed_hz;
	SPIMODE mode;
	
	std::unique_ptr<GPIOPin> custom_cs;
	
	static std::string getSpiDevicePath(const std::string& path);
	static std::unique_ptr<GPIOPin> getCustomCS(const std::string& path);
};


#endif //MOVEIO_SPI_DEVICE_H
