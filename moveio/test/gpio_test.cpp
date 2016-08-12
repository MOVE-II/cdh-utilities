#include <iostream>
#include <unistd.h>

#include <moveio/PinMapper.h>
#include <moveio/GPIOPin.h>

using namespace std;

int main()
{
	if (!PinMapper::valid())
	{
		cout << "Couldn't load PinMapper\n";
		return 1;
	}
	
	std::string pinId = PinMapper::find("P8_46");
	cout << "Using: " << pinId << "\n";
	
	GPIOPin pin(pinId);
	
	if (!pin.isOK())
	{
		cout << "Couldn't load pin, is it exported?\n";
		return 2;
	}
	
	bool toggle = false;
	for (int i=0 ; i<20 ; i++)
	{
		toggle = !toggle;
		
		cout << "Pin to: " << toggle;
		pin.setPin(toggle);
		cout << " " << pin.readPin() << "\n";
		
		sleep(1);
	}
}
