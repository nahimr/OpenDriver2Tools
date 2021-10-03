#pragma once
#include <map>
#include <string>

class StringsStack
{
public:
	unsigned int offset = 0;
	std::map<std::string, std::tuple<unsigned int, std::string>>* data;
	
	explicit StringsStack();

	~StringsStack();

	void addString(const std::string& key, const std::string& str);

	std::string getString(const std::string& key) const;

	unsigned int getOffset(const std::string& key) const;

	size_t getSize() const;
};

