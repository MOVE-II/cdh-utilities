#include <iostream>
#include <string>

#include "moveio/SPIDevice.h"
#include "moveio/PinMapper.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 2 && argc != 3)
	{
		cout << "Usage:\n";
		cout << "\tspitest /dev/spidev1.1 [MODE]\n";
		cout << "\tspitest /dev/spidev1.1#P8_45 [MODE]\n";
		cout << "\n";
		cout << "Modes:\n";
		cout << "\t0-3\n";
		
		return 1;
	}
	
	SPIDevice::SPIMODE mode = SPIDevice::MODE_0;
	if (argc >= 3)
	{
		switch (stoi(argv[2]))
		{
			case 0: mode = SPIDevice::MODE_0; break;
			case 1: mode = SPIDevice::MODE_1; break;
			case 2: mode = SPIDevice::MODE_2; break;
			case 3: mode = SPIDevice::MODE_3; break;
			default:
				cout << "Unknown mode: " << argv[2] << "\n";
				return 1;
		}
	}
	
	string testString = "MOVEMOVEMOVEMOVE";
	char testBuffer[100];
	
	SPIDevice dev(argv[1], mode);
	
	dev.setSpeed(50000);

	for (int i=0 ; i<10 ; i++)
	{
		dev.write(testString.data(), testString.size());
		dev.read(testBuffer, 10);
		dev.write(testBuffer, 10);
	}
	
	return 0;
}

