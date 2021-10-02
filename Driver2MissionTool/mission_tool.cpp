#include <cstdio>
#include <cstdlib>
#include "mission.hpp"
#include "script.hpp"
#include <string>
#include "StringsStack.h"
#include "utils/rapidxml.hpp"
#include "utils/rapidxml_utils.hpp"

using namespace rapidxml;

StringsStack* ParseStrings(xml_node<>* element)
{
	StringsStack* strings = new StringsStack();
	xml_node<>* string = element->first_node();
	
	do
	{
		strings->addString(string->first_attribute("id")->value(), string->value());
	} while ((string = string->next_sibling()));

	return strings;
}

MS_MISSION ParseSettings(xml_node<>* element)
{
	printf("%s\n", element->name());
	return MS_MISSION();
}

MS_TARGET* ParseTargets(xml_node<>* element)
{
	printf("%s\n", element->name());
	MS_TARGET* targets = static_cast<MS_TARGET*>(malloc(sizeof(MS_TARGET) * 16));
	return targets;
}

Stack ParseScript(xml_node<>* element)
{
	printf("%s\n", element->name());
	return Stack();
}

void CreateMission(std::string &filename)
{
	printf("File: %s\n", filename.data());
	file<> file(filename.data());
	xml_document<> document;
	document.parse<0>(file.data());
	xml_node<>* node = document.first_node()->first_node();

	StringsStack* strings = ParseStrings(node);
	MS_MISSION settings = ParseSettings(node = node->next_sibling());
	MS_TARGET* targets = ParseTargets(node = node->next_sibling());
	Stack stack = ParseScript(node = node->next_sibling());
	
	free(targets);
}

//-----------------------------------------------------

void PrintCommandLineArguments()
{
	printf("Example usage:\n");
	printf("\tDriver2MissionTool --decompile MISSIONS.BLK [mission_number]\n");
	printf("\tDriver2MissionTool --compile <mission_script.xml>\n");
}

int main(const int argc, char** argv)
{
	printf("---------------\nDriverMissionTool - Driver 2 mission decompiler\n---------------\n\n");

	if (argc <= 1)
	{
		PrintCommandLineArguments();
		return -1;
	}

	for (int i = 0; i < argc; i++)
	{
		if (!stricmp(argv[i], "--compile") || !stricmp(argv[i], "-c"))
		{
			if (argc > 2)
			{
				CreateMission(std::string(argv[i + 1]));
			}
		}

	}

	return 0;
}