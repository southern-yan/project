#pragma once

#include <iostream>
#include <string>
#include <vector>

#define INFO 0 //没有危害，只是打印出来
#define WARNING 1 //警告
#define FATAL 2 //致命错误

using namespace std;

vector<string> g_error_level={"INFO","WARNING","FATAL",};

void Log(int level,string message,string file,int line)
{
    cout << "[ " << g_error_level[level] << " ][ "<< message <<" ] : "<< file << " : "<<line << endl;
}

#define LOG(level,message) Log(level,message, __FILE__, __LINE__)
