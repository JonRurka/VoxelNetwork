#pragma once

#include "stdafx.h"
#include "VoxelServer.h"
#include <mutex>

#define MAX_MESSAGES 8000
#define INPUT_MAX    1024

typedef unsigned int LogLevel;

#define LogLevel_Print      0
#define LogLevel_Log        1
#define LogLevel_Warning    2
#define LogLevel_Error      3
#define LogLevel_ResetInput 4

#define CONSOLE_NC      "0m"
#define CONSOLE_YELLOW  "93m"
#define CONSOLE_RED     "91m"

class VoxelServer;

class Logger {
public:
    
    struct LogEntry
    {
    public:
        std::string Message;
        LogLevel Level;

        LogEntry(LogLevel level, std::string message)
        {
            Level = level;
            Message = message;
        }
    };


private:
    

    static char _inputStr[INPUT_MAX];
    static int _inputStr_size;

    static std::string _logFile;
    static int _messageCount;
    static std::mutex mtx;

    static char sepChar();

    static std::string LogFile();

    static std::list<LogEntry> _entries;
    static std::list<LogEntry> _currentEntries;

    static bool replace(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = str.find(from);
        if (start_pos == std::string::npos)
            return false;
        str.replace(start_pos, from.length(), to);
        return true;
    }

    static std::string Format(std::string message, std::string* parts, int num)
    {
        for (int i = 0; i < num; i++)
        {
            replace(message, "{"+std::to_string(i)+"}", parts[i]);
        }
        return message;
    }
    
public:

    static std::string InputStr() {
        return std::string(_inputStr);
    }
    static std::string InputStr(char* inpt, int len) {
        mtx.lock();
        strcpy(_inputStr, inpt);
        _inputStr_size = len;
        mtx.unlock();
        return inpt;
    }

    static void Print(std::string message)
    {
        AddEntry(LogLevel_Print, message);

        /*std::string* args = new std::string[num];
        va_list valist;
        va_start(valist, num);
        for (int i = 0; i < num; i++) { //access all the arguments assigned to valist
            args[i] = va_arg(valist, std::string);
        }
        va_end(valist);

        AddEntry(LogLevel_Print, Format(message, args, num));

        delete args;*/

        /*AddEntry(LogLevel.Print, string.Format(message.ToString(), args));
        Action a = () => LogToFile(string.Format(message.ToString(), args));
        QueueLog(a);*/
    }

    static void PrintNoFormat(std::string message)
    {
        AddEntry(LogLevel_Print, message);
        /*AddEntry(LogLevel.Print, message.ToString());
        Action a = () => LogToFile(message.ToString());
        QueueLog(a);*/
    }

    static void Log(std::string message)
    {
        AddEntry(LogLevel_Log, "[time]: " + message);

        /*std::string* args = new std::string[num];
        va_list valist;;
        va_start(valist, num);
        for (int i = 0; i < num; i++) { //access all the arguments assigned to valist
            args[0] = va_arg(valist, int);
        }
        va_end(valist);

        std::string messageStr = Format(message, args, num);
        AddEntry(LogLevel_Log, "[time]: " + messageStr);*/


        /*string messageStr = string.Format(message.ToString(), args);
        AddEntry(LogLevel.Log, string.Format("[{0}]: {1}", GetTime(), messageStr));
        Action a = () => LogToFile(string.Format("[{0}]: {1}", GetTime(), messageStr));
        QueueLog(a);*/
    }

    static void LogWarning(std::string message)
    {
        AddEntry(LogLevel_Warning, "[time]: " + message);

        /*std::string* args = new std::string[num];
        va_list valist;;
        va_start(valist, num);
        for (int i = 0; i < num; i++) { //access all the arguments assigned to valist
            args[0] = va_arg(valist, int);
        }
        va_end(valist);

        std::string messageStr = Format(message, args, num);
        AddEntry(LogLevel_Warning, "[time]: " + messageStr);*/


        /*string messageStr = string.Format(message.ToString(), args);
        AddEntry(LogLevel.Warning, string.Format("[{0}]: {1}", GetTime(), messageStr));
        Action a = () => LogToFile(string.Format("[{0} W]: {1}", GetTime(), messageStr));
        QueueLog(a);*/
    }

    static void LogError(std::string message)
    {
        AddEntry(LogLevel_Error, "[time]: " + message);

        /*std::string* args = new std::string[num];
        va_list valist;;
        va_start(valist, num);
        for (int i = 0; i < num; i++) { //access all the arguments assigned to valist
            args[0] = va_arg(valist, int);
        }
        va_end(valist);

        std::string messageStr = Format(message, args, num);
        AddEntry(LogLevel_Error, "[time]: " + messageStr);*/

        /*string messageStr = string.Format(message.ToString(), args);
        AddEntry(LogLevel.Error, string.Format("[{0}]: {1}", GetTime(), messageStr));
        Action a = () => LogToFile(string.Format("[{0} E]: {1}", GetTime(), messageStr));
        QueueLog(a);*/
    }

    static void ResetInput() {
        AddEntry(LogLevel_ResetInput, "");
    }

    static void Clear()
    {
        //InputStr = string.Empty;
        //Console.Clear();
        //AddEntry(LogLevel.Print, "");
    }

    static std::string GetTime()
    {
        //DateTime time = DateTime.Now;
        //return string.Format("{0:00}:{1:00}:{2:00}", time.Hour, time.Minute, time.Second);
        return "";
    }

    static std::string GetDateTime()
    {
        //DateTime time = DateTime.Now;
        //return time.ToString();
        return "";
    }

    static long GetEpoch()
    {
        //return (long)(DateTime.UtcNow - new DateTime(1970, 1, 1)).TotalMilliseconds;
        return 0;
    }

    static void AddEntry(LogLevel level, std::string message)
    {
        mtx.lock();
        _entries.push_back(LogEntry(level, message));
        mtx.unlock();
    }

    static void Init();

    static void Update();

    static void Draw(LogLevel level, std::string message);

    static void ClearCurrentConsoleLine(int size);
};
