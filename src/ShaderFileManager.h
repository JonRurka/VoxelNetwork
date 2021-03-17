#pragma once

#include "stdafx.h"

class ShaderFileManager
{
private:

	static std::map<std::string, std::string> fileContents;
	static std::string script_dir;

public:

	static void Init(std::string dir);
	static bool Load(std::string fileName);
	static std::string Get(std::string fileName);
};

