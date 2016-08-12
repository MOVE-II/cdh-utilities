#include <iostream>
#include <unistd.h>

#include <moveio/I2CDevice.h>

using namespace std;

int main()
{
	cout << "Opening device 0x41\n";
	
	I2CDevice dev("/dev/i2c-2", 0x41);
	
	if (!dev.isOK())
	{
		cout << "Couldn't open device\n";
		return 1;
	}
	
	int retv = 0;
	for (int i=0 ; i<3 ; i++)
	{
		int cmd = 17 + i*53;
		
		cout << "Using command " << cmd << "\n";
		
		for (int j=0 ; j<3 ; j++)
		{
			int val = 4+j;
			cout << "Sending value " << val << "\n";
			
			if (!dev.write_byte(cmd, val))
			{
				retv = 2;
				cout << "failed ...\n";
			}
		}
	}
	
	__u8 mchar;
	__u16 mshort;
	
	dev.read_byte(0, mchar);
	dev.write_byte(0, mchar);
	
	dev.read_word(0, mshort);
	dev.write_word(0, mshort);
	
	__u8 block[32];
	dev.read_block(0, block);
	dev.write_block(0, 20, block);
	
	return retv;
}
