#pragma once

#include "../stdafx.h"

typedef unsigned int Protocal;

#define Protocal_Tcp 0;
#define Protocal_Udp 1;

class Data {
public:
    Protocal Type;
    char command;
    std::vector<char> Buffer;
    std::string Input;

    Data(Protocal type, char cmd, std::vector<char> data) {
        Type = type;
        command = cmd;
        Buffer = data;
        Input = std::string(Buffer.begin(), Buffer.end());
    }
}