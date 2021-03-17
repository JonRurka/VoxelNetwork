#include "ShaderFileManager.h"

std::map<std::string, std::string> ShaderFileManager::fileContents;
std::string ShaderFileManager::script_dir;

void ShaderFileManager::Init(std::string dir)
{
	script_dir = dir;
}

bool ShaderFileManager::Load(std::string fileName)
{
	std::string fileText;
	std::fstream script_file;

	script_file.open(script_dir + fileName, std::ios::in);
	if (!script_file)
	{
		return false;
	}

	std::string tmpTxt;
	while (getline(script_file, tmpTxt))
		fileText += tmpTxt + "\n";
	script_file.close();

	fileContents[fileName] = fileText;

	return true;
}

std::string ShaderFileManager::Get(std::string fileName)
{
	return fileContents[fileName];
}
