#include "PinMapper.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <fstream>
#include <iostream>
#include <sstream>

namespace PinMapper
{

const std::string invalid = "invalid";
const std::string path_mapping = "/etc/moveio/mapping.cfg";

/*
	class Data
*/

class Data
{
public:
	bool valid;
	std::unordered_map<std::string, PinInfo> mapping;

	Data();
	bool loadMapping(const std::string& path);

	const PinInfo* find(const std::string& key) const;
};

Data::Data() :
	valid(false)
{
	bool success = false;

	success = success || loadMapping(path_mapping);

	this->valid = success;
	if (!success)
	{
		std::cerr << "Could not load mapping for PinMapper!\n";
	}
}

bool Data::loadMapping(const std::string& path)
{
	std::ifstream file(path);

	if (!file)
	{
		return false;
	}

	this->mapping.clear();
	while (!file.eof())
	{
		if (!file)
		{
			return false;
		}

		PinInfo info;

		file >> info.key >> info.id >> info.exportId;

		if (!info.key.empty())
		{
			this->mapping[info.key] = info;
		}
	}

	return true;
}

const PinInfo* Data::find(const std::string& key) const
{
	auto ptr = this->mapping.find(key);
	
	if (ptr == this->mapping.end())
	{
		return nullptr;
	}
	
	return &ptr->second;
}

/*
	end class Data
*/


/*
	static wrappers
*/

std::unique_ptr<Data> data = nullptr;

void init()
{
	if (data == nullptr)
	{
		//data = new Data();
		data.reset(new Data());
	}
}

const PinInfo* findInfo(const std::string& key)
{
	init();

	return data->find(key);
}

const std::string& find(const std::string& key)
{
	init();
	
	const PinInfo* info = data->find(key);

	if (info == nullptr)
	{
		return invalid;
	}

	return info->id;
}

bool valid()
{
	init();

	return data->valid;
}

std::vector<PinInfo> listMappings()
{
	init();
	
	std::vector<PinInfo> mapping;
	mapping.reserve(data->mapping.size());
	
	for (auto& pair : data->mapping)
	{
		mapping.push_back(pair.second);
	}
	
	return mapping;
}

} // namespace PinMapper
