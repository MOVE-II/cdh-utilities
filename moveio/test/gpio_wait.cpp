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
	
	std::string pinId = PinMapper::find("P8_45");
	cout << "Using: " << pinId << "\n";
	
	GPIOPin pin(pinId);
	if (!pin.isOK())
	{
		cout << "Couldn't load pin, is it exported?\n";
		return 2;
	}
	
	cout << "Pin is: " << pin.readPin() << "\n";
	
	pin.setEdgeTrigger(GPIOPin::TRIGGER_BOTH);
	cout << "Waiting for edge (both) ...\n";
	
	for (int i=0 ; i<8 ; i++)
	{
		pin.waitForEdge();
		cout << "edge: " << pin.readPin() << "\n";
	}
	
	pin.setEdgeTrigger(GPIOPin::TRIGGER_RISING);
	cout << "Waiting for edge (rising) ...\n";
	
	for (int i=0 ; i<3 ; i++)
	{
		pin.waitForEdge();
		cout << "edge: " << pin.readPin() << "\n";
	}
	
	pin.setEdgeTrigger(GPIOPin::TRIGGER_FALLING);
	cout << "Waiting for edge (falling) ...\n";
	
	for (int i=0 ; i<3 ; i++)
	{
		pin.waitForEdge();
		cout << "edge: " << pin.readPin() << "\n";
	}
}
