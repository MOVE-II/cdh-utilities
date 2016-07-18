#ifndef MOVEIO_PIN_MAPPER_H
#define MOVEIO_PIN_MAPPER_H

#include <string>
#include <vector>

/**
The methods in this namespace can be used to find pin ids by more convenient names (like the names on the PCB).

e.g. PinMapper::find("P8_20") returns an id that corresponds to pin at P8_20 on a beagle bone.
*/
namespace PinMapper
{
	struct PinInfo
	{
		std::string key;
		std::string id;
		int exportId;
	};

	/**
		Find the gpio pin info for the given key, returns nullptr when the key does not exist.
	*/
	const PinInfo* findInfo(const std::string& key);

	/**
		Returned when the pin can't be found.
	*/
	extern const std::string invalid;

	/**
		Find the gpio pin id for the given key. Returns "PinMapper::invalid" when the key could not be found.
	*/
	const std::string& find(const std::string& key);

	/**
		Returns true when the mapping file could be successfully loaded.
	*/
	bool valid();
	
	/**
		Returns all PinInfos
	*/
	std::vector<PinInfo> listMappings();
};

#endif //MOVEIO_PIN_MAPPER_H
