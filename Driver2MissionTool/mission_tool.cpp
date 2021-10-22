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

	// TODO: Refactoring MS_MISSION
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
	
	assert(sizeof(settings) == 0x84);

	return settings;
}

void FillTarget(int* ptr, 
	std::map<int, std::string> list, 
	xml_node<>* el, 
	StringsStack* strings)
{
	int val = 0;
	
	for (std::pair<int, std::string> pair : list)
	{
		const int valueType = pair.first & 0xff;
		// int pos = (pair.first & 0xff00) >> 8;
		xml_attribute<>* attr = el->first_attribute(pair.second.data());

		if (!attr || SKIP == valueType)
		{
			ptr++;
			continue;
		}
		printf("%p", ptr);
		std::string value = attr->value() ? attr->value() : "";

		if (valueType == MAP_VALUES)
		{
			val = mapValues(split(value, '|'));
		}
		else if (valueType == STRING_OFFSET)
		{
			val = value.empty() ? -1 : strings->getOffset(value);
		}
		else if (valueType == INTEGER)
		{
			val = strtol(value.data(), nullptr, 0);
		}

		*ptr = val;
		assert(*ptr == val);
		printf(", %d, %d, %s\n", *ptr, val, pair.second.data());
		ptr++;
	}
}

MS_TARGET* ParseTargets(xml_node<>* element, StringsStack* strings)
{
	const auto targets = static_cast<MS_TARGET*>(malloc(sizeof(MS_TARGET) * 16));
	assert(memset(targets, 0, sizeof(MS_TARGET) * 16) == targets);

	xml_node<>* el = element->first_node();

	// Parse Targets
	for (int i = 0; i < 16 && el; i++)
	{
		std::string name = el->name();
		printf("%s\n", name.data());
		const auto ptr = reinterpret_cast<int*>(targets + i);

		if (name == "Car" || name == "Player2Start")
		{
			FillTarget(ptr, carTargetsProp, el, strings);

			xml_node<>* nel = el;
			if (nel->first_node("Chasing"))
			{
				nel = nel->first_node("Chasing");
				targets[i].car.type = 3;
				FillTarget(ptr, carChasingProp, nel, strings);
			}
			else if (nel->first_node("Tailing"))
			{
				nel = nel->first_node("Tailing");
				FillTarget(ptr, carTailingProp, nel, strings);
			}
		}
		else if (name == "Point")
		{
			targets[i].type = Target_Point;
			FillTarget(ptr, pointTargetsProp, el, strings);
		}
		else if (name == "Event")
		{
			// TODO: Handle Event
		}
		else if (name == "MultiCar")
		{
			// TODO: Handle MultiCar
		}

		el = el->next_sibling();
	}

	return targets;
}

void ProcessThread(xml_node<>* element, 
	Thread* thread, StringsStack* strings,
	Stack* stack)
{
	// TODO: Here we parse thread !!
	const xml_node<>* instr = element->first_node();
	unsigned int arg = 0;
	do
	{
		// Here process instructions
		if (!instr) break;
		std::string op_name = instr->name();
		const std::pair<int, int> tupple = functionsProp.at(op_name);

		for (int i = 0; i < tupple.first; i++)
		{
			char attr_name[7];
			sprintf_s(attr_name, 7, "arg%d", i);
			const xml_attribute<>* argument = instr->first_attribute(attr_name);

			if (op_name == "ShowPlayerMessage" && i == 0)
			{
				arg = strings->getOffset(argument->value());
				push(thread, arg);
				continue;
			}

			if (op_name == "Push")
			{
				arg = atoi(argument->value());
				push(thread, arg);
				continue;
			}

			push(thread, arg);
		}

		push(thread, tupple.second);
	} while ((instr = instr->next_sibling()));
}

Stack ParseScript(xml_node<>* element, StringsStack* strings)
{
	Stack stack;
	initStack(&stack);
	const auto threads = static_cast<Thread*>(malloc(sizeof(Thread) * 16));
	for (int i = 0; i < 16; i++)
	{
		init(threads + i);
	}

	xml_node<>* main = element->first_node("Main");
	xml_node<>* thread = element->first_node("Thread");
	unsigned int i = 1;

	ProcessThread(main, threads, strings, &stack);
	addThread(&stack, threads);
	do
	{
		const size_t index = atoi(thread->first_attribute("id")->value());
		ProcessThread(thread, threads + i, strings, &stack);
		
		addThread(&stack, threads + index);
		i++;
	} while ((thread = thread->next_sibling("Thread")));

	processStack(&stack);
	return stack;
}

void CompileMission(const char* filename,
	MS_MISSION settings, 
	MS_TARGET targets[16],
	const StringsStack strings, 
	const Stack stack)
{
	FILE* file;
	if (fopen_s(&file, filename, "w"))
	{
		fprintf(stderr, "Error when trying to open file!\n");
		exit(EXIT_FAILURE);
	}

	settings.strings = stack.nbOperations;
	fwrite(&settings, sizeof(settings), 1, file);
	fwrite(targets, sizeof(MS_TARGET), 16, file);

	fwrite(stack.operations, sizeof(u_int), stack.nbOperations, file);

	char aZero = 0;

	for (auto& pair : *strings.data)
	{
		auto str = std::get<std::string>(pair.second);
		fwrite(str.data(), sizeof(char), str.size(), file);
		fwrite(&aZero, sizeof(char), 1, file);
	}

	fseek(file, 0, SEEK_END); // seek to end of file
	const long sizeMFile = ftell(file); // get size of file

	const int nearestPow = static_cast<int>(floor(static_cast<int>(log(sizeMFile) / log(2)))) + 1;
	const int twoPowNear = static_cast<int>(pow(2, nearestPow));
	const size_t fillSize = twoPowNear - sizeMFile;

	const char val = 0x21; // '!'
	//Function to fill the end of the mission file
	for (size_t j = 0; j < fillSize; j++) {
		fwrite(&val, 1, 1, file);
	}

	fclose(file);
}

void CreateMission(std::string &filename)
{
	printf("File: %s\n", filename.data());
	file<> file(filename.data());
	xml_document<> document;
	document.parse<0>(file.data());
	xml_node<>* node = document.first_node();
	int missionNumber = strtol(node->first_attribute("id")->value(), nullptr, 0);
	char missionFile[9];
	sprintf_s(missionFile,9, "M%d.D2MS", missionNumber);

	StringsStack* strings = ParseStrings(node = node->first_node("Strings"));
	MS_MISSION settings = ParseSettings(node = node->next_sibling("Settings"), strings);
	MS_TARGET* targets = ParseTargets(node = node->next_sibling("Targets"), strings);
	Stack stack = ParseScript(node = node->next_sibling("Script"), strings);

	CompileMission(missionFile, settings, targets, *strings, stack);
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
		if (!_stricmp(argv[i], "--compile") || !_stricmp(argv[i], "-c"))
		{
			if (argc > 2)
			{
				CreateMission(std::string(argv[i + 1]));
			}
		}

	}

	return 0;
}