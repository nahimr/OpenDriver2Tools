#include <cstdio>
#include <cstdlib>
#include <sstream>
#include "mission.hpp"
#include "script.hpp"
#include "StringsStack.h"
#include "utils/rapidxml.hpp"
#include "utils/rapidxml_utils.hpp"

using namespace rapidxml;

std::vector<std::string> split(const std::string& s, const char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

int mapValues(std::vector<std::string> &list)
{
	int value = 0;
	for (std::string &str : list)
	{
		value |= settingsFlags[str];
	}
	return value;
}

int* splitValues(std::vector<std::string> &in)
{
	int* list = static_cast<int*>(malloc(sizeof(int) * in.size()));
	for (int i = 0; i < in.size(); i++)
	{
		list[i] = atoi(in[i].data());
	}
	return list;
}

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

MS_MISSION ParseSettings(xml_node<>* el, StringsStack* strings)
{
	MS_MISSION settings;
	memset(&settings, 0, sizeof(MS_MISSION));

	// Parse attributes	
	settings.id = MISSION_IDENT;
	settings.size = 0x84;
	settings.city = settingsFlags[el->first_attribute("city")->value()];
	settings.time = settingsFlags[el->first_attribute("time")->value()];
	settings.weather = settingsFlags[el->first_attribute("weather")->value()];
	settings.playerStartPosition = {
		atoi(el->first_attribute("x")->value()),
		atoi(el->first_attribute("y")->value()),
	};
	settings.playerStartRotation = atoi(el->first_attribute("rotation")->value());
	settings.type = mapValues(split(el->first_attribute("type")->value(), '|'));
	settings.timer = atoi(el->first_attribute("timer")->value());
	settings.timerFlags = mapValues(split(el->first_attribute("timerFlags")->value(), '|'));
	settings.cops = {
		atoi(el->first_attribute("copsSpeed")->value()),
		atoi(el->first_attribute("copsPower")->value()),
		atoi(el->first_attribute("copsMin")->value()),
		atoi(el->first_attribute("copsMax")->value()),
	};
	settings.region = atoi(el->first_attribute("region")->value());
	settings.playerCarModel = atoi(el->first_attribute("playerCarModel")->value());
	settings.playerCarColour = atoi(el->first_attribute("playerCarColour")->value());
	settings.maxDamage = atoi(el->first_attribute("maxDamage")->value());
	settings.nCutscenes = atoi(el->first_attribute("nbCut")->value());
	auto rdModels = split(el->first_attribute("residentModels")->value(), '|');
	settings.residentModels[0] = settings.playerCarModel;
	for (int i = 1; i < 5; i++)
	{
		settings.residentModels[i] = atoi(rdModels[i - 1].data());
	}

	xml_node<>* messages = el->first_node("Messages");

	settings.msgCarWrecked = strings->getOffset(
		messages->first_node("CarWrecked")->first_attribute("strId")->value()
	);

	settings.msgComplete = strings->getOffset(
		messages->first_node("Complete")->first_attribute("strId")->value()
	);

	settings.msgOutOfTime = strings->getOffset(
		messages->first_node("OutOfTime")->first_attribute("strId")->value()
	);

	settings.msgDrowned = strings->getOffset(
		messages->first_node("Drowned")->first_attribute("strId")->value()
	);

	settings.msgDrowned = strings->getOffset(
		messages->first_node("PoliceCar")->first_attribute("strId")->value()
	);
	
	settings.msgDrowned = strings->getOffset(
		messages->first_node("DoorsLocked")->first_attribute("strId")->value()
	);
	
	return settings;
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
	MS_MISSION settings = ParseSettings(node = node->next_sibling(), strings);
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