#include "settings.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>

namespace
{
const char* kConfigFileName = GAME_TITLE ".config";

std::string Trim(std::string value)
{
	value.erase(value.begin(), std::find_if(value.begin(), value.end(), [](unsigned char ch)
	{
		return std::isspace(ch) == 0;
	}));

	value.erase(std::find_if(value.rbegin(), value.rend(), [](unsigned char ch)
	{
		return std::isspace(ch) == 0;
	}).base(), value.end());

	return value;
}

bool ParseBool(const std::string& value, bool fallback)
{
	std::string normalized = value;
	std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char ch)
	{
		return static_cast<char>(std::tolower(ch));
	});

	if (normalized == "1" || normalized == "true")
		return true;
	if (normalized == "0" || normalized == "false")
		return false;

	return fallback;
}
}

void LoadSettings()
{
	std::ifstream file(kConfigFileName);
	if (!file.is_open())
		return;

	std::string line;
	while (std::getline(file, line))
	{
		line = Trim(line);
		if (line.empty() || line[0] == '#' || line[0] == ';')
			continue;

		const std::size_t split = line.find('=');
		if (split == std::string::npos)
			continue;

		const std::string key = Trim(line.substr(0, split));
		const std::string value = Trim(line.substr(split + 1));

		try
		{
			if (key == "screenWidth")
				screenWidth = std::stoi(value);
			else if (key == "screenHeight")
				screenHeight = std::stoi(value);
			else if (key == "scaleMultiplier")
				scaleMultiplier = std::stof(value);
			else if (key == "fullscreen")
				fullscreen = ParseBool(value, fullscreen);
			else if (key == "movingLeftKey")
				movingLeftKey = static_cast<KeyboardKey>(std::stoi(value));
			else if (key == "movingLeftKeySecondary")
				movingLeftKeySecondary = static_cast<KeyboardKey>(std::stoi(value));
			else if (key == "movingRightKey")
				movingRightKey = static_cast<KeyboardKey>(std::stoi(value));
			else if (key == "movingRightKeySecondary")
				movingRightKeySecondary = static_cast<KeyboardKey>(std::stoi(value));
		}
		catch (...)
		{
			TraceLog(LOG_WARNING, "Invalid setting in %s: %s", kConfigFileName, key.c_str());
		}
	}
}

void SaveSettings()
{
	std::ofstream file(kConfigFileName, std::ios::trunc);
	if (!file.is_open())
	{
		TraceLog(LOG_WARNING, "Failed to write settings file: %s", kConfigFileName);
		return;
	}

	file << "# " << GAME_TITLE << " configuration file\n";
	file << "screenWidth=" << screenWidth << '\n';
	file << "screenHeight=" << screenHeight << '\n';
	file << "scaleMultiplier=" << scaleMultiplier << '\n';
	file << "fullscreen=" << (fullscreen ? 1 : 0) << '\n';
	file << "movingLeftKey=" << movingLeftKey << '\n';
	file << "movingLeftKeySecondary=" << movingLeftKeySecondary << '\n';
	file << "movingRightKey=" << movingRightKey << '\n';
	file << "movingRightKeySecondary=" << movingRightKeySecondary << '\n';
}
