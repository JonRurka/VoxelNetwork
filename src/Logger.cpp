#include "Logger.h"

char Logger::_inputStr[INPUT_MAX];
int Logger::_inputStr_size = 0;
std::string Logger::_logFile;
int Logger::_messageCount;
std::mutex Logger::mtx;

std::list<Logger::LogEntry> Logger::_entries;
std::list<Logger::LogEntry> Logger::_currentEntries;

char Logger::sepChar()
{
    return VoxelServer::sepChar();
}
std::string Logger::LogFile()
{
    return _logFile;
}

void Logger::Init()
{
    Draw(LogLevel_Print, "");
}

void Logger::Update()
{
    if (_entries.size() > 0)
    {
        mtx.lock();

        _currentEntries.clear();

        while (_entries.size() > 0)
        {
            _currentEntries.push_back((const LogEntry)_entries.front());
            _entries.pop_front();
        }
        _entries.clear();

        mtx.unlock();

        for (LogEntry ent : _currentEntries)
        {
            Draw(ent.Level, ent.Message);
        }
    }
}

void Logger::Draw(LogLevel level, std::string message)
{
    if (level == LogLevel_ResetInput)
    {
        ClearCurrentConsoleLine(0);
        printf("> ");
    }
    else
    {
        
        _messageCount++;

        std::string lineStr = "\x1B[";
        switch (level)
        {
        case LogLevel_Print:
        case LogLevel_Log:
            lineStr += CONSOLE_NC;
            break;
        case LogLevel_Warning:
            lineStr += CONSOLE_YELLOW;
            break;
        case LogLevel_Error:
            lineStr += CONSOLE_RED;
            break;
        }
        //mtx.lock();

        int inp_size = VoxelServer::GetInputProcessor()->GetCurrentInputSize();
        std::string cur = VoxelServer::GetInputProcessor()->GetCurrentInput();

        lineStr += message + "\033[0m\n";

        ClearCurrentConsoleLine(inp_size);
        printf(lineStr.c_str());
        printf(("> " + cur).c_str());
        //mtx.unlock();
    }
}

void Logger::ClearCurrentConsoleLine(int size)
{
    printf("\x1B[2K"); // clear current line.
    printf("\033[100D"); // Reset cursor position.

}
