#include "StringsStack.h"

StringsStack::StringsStack()
{
	this->data = new std::map<std::string, std::tuple<unsigned int, std::string>>();
	this->offset = 0;
}

StringsStack::~StringsStack()
{
	delete data;
}

void StringsStack::addString(const std::string &key, const std::string &str)
{
	std::tuple<int, std::string> tup(this->offset, str);
	this->offset += str.size() + 1;

	this->data->insert({ key, tup });
}

std::string StringsStack::getString(const std::string& key) const
{
	return std::get<std::string>(this->data->at(key));
}

unsigned int StringsStack::getOffset(const std::string& key) const
{
	return std::get<unsigned int>(this->data->at(key));
}
