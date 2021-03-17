#pragma once

#include "stdafx.h"
#include "VoxelServer.h"
#include "Logger.h"

#define INPUT_MAX    1024

class CommandExecuter {
private:
	bool running;
	std::thread c_thread;

	char _inputStr[INPUT_MAX];
	int _inputStr_size;

	void Loop();
public:
	CommandExecuter();
	void Run(bool multiThread);
	void Process();
	void Stop();

	int GetCurrentInputSize()
	{
		return _inputStr_size;
	}

	std::string GetCurrentInput()
	{
		return std::string(_inputStr, _inputStr_size);
	}
};