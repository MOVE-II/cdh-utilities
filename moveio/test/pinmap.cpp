#include <iostream>
#include <string>
#include <fstream>

#include <moveio/GPIOPin.h>
#include <moveio/PinMapper.h>

using namespace std;

void show_usage()
{
	cout << "Usage:\n";
	cout << "\tpinmap list\n";
	cout << "\tpinmap get P8_45\n";
	cout << "\tpinmap export P8_45\n";
	cout << "\tpinmap direction P8_45 out\n";
	cout << "\tpinmap value P8_45 1\n";
	cout << "\tpinmap unexport P8_45\n";
}

int main(int argc, char* argv[])
{
	if (!PinMapper::valid())
	{
		return 1;
	}

	if (argc < 2 || argc > 4)
	{
		show_usage();
		return 2;
	}

	std::string cmd = std::string(argv[1]);
	
	if (cmd == "list")
	{
		for (PinMapper::PinInfo& info : PinMapper::listMappings())
		{
			cout << info.key << "\n"
			     << info.id << " " << info.exportId << "\n\n";
		}
		
		return 0;
	}
	
	if (argc < 3)
	{
		show_usage();
		return 2;
	}

	const PinMapper::PinInfo *info = PinMapper::findInfo(std::string(argv[2]));
	if (info == nullptr)
	{
		cerr << "Could not find pin '" << argv[2] << "'!\n";
		return 3;
	}

	if (cmd == "get")
	{
		if (info != nullptr)
		{
			cout << info->id << "\n";
		}
		else
		{
			cerr << "Could not find pin!";
			return 3;
		}
	}
	else if (cmd == "export")
	{
		ofstream file(GPIOPin::gpio_sysfs + "export");
		file << info->exportId;
	}
	else if (cmd == "unexport")
	{
		ofstream file(GPIOPin::gpio_sysfs + "unexport");
		file << info->exportId;
	}
	else
	{
		GPIOPin pin(info->id);

		if (!pin.isOK())
		{
			cerr << "Could not open pin '" << info->id << "'!";
			return 4;
		}

		if (cmd == "direction")
		{
			if (argc != 4)
			{
				show_usage();
				return 2;
			}

			ofstream file(pin.getPinPath() + "direction");
			file << argv[3];
		}
		else if (cmd == "value")
		{
			if (argc != 4)
			{
				show_usage();
				return 2;
			}
			
			ofstream file(pin.getPinPath() + "value");
			file << argv[3];
		}
	}

	return 0;
}

