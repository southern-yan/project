#pragma once

#include <iostream>
#include <rpc_client.hpp>
#include <chrono>
#include <string>
#include <stdlib.h>
#include <fstream>
#include "codec.h"

using namespace std;
using namespace std::chrono_literals;
using namespace rest_rpc;
using namespace rest_rpc::rpc_service;


uint32_t Register(const string &ip,const int &port,string &name,string &passwd,uint32_t &id)
{
    cout<<"请输入昵称:> ";
    cin>>name;
    cout<<"请输入密码:> ";
    cin>>passwd;
    string _passwd;
    cout<<"请确认密码:> ";
    cin>>_passwd;
    if(passwd!=_passwd)
    {
        cout<<"密码输入不一致"<<endl;
        return 1;
    }

	try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 2;
		}

       id = client.call<uint32_t>("RpcRegister",name,passwd);
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
    return 0;
}

uint32_t Login(const string &ip,const int &port)
{
    uint32_t id;
    string passwd;
    cout<<"请输入登录ID: ";
    cin>>id;
    cout<<"请输入登录密码: ";
    cin>>passwd;
    uint32_t result=0;
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

       result = client.call<uint32_t>("RpcLogin",id,passwd);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
    return result;
}

bool PushMatchPool(string &ip,int &port,uint32_t &id)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

      return client.call<bool>("RpcMatchAndWait",id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    //cout<<"!!!!!!!!!!!!!!!!!!!"<<endl;
    }
}

int CheckReady(string &ip,int &port,uint32_t &id)
{

    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

      return client.call<int>("RpcPlayerReady",id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
}

bool Match(string &ip,int &port,uint32_t &id)
{
    PushMatchPool(ip,port,id);//首先把用户id放入匹配池
    while(1)
    {
        int ret=CheckReady(ip,port,id);//检测用户的状态
        if(ret == 3)
        {
            return true;
        }
        else if(ret == 1)
        {
            return false;
        }
        else
        {
            sleep(1);
        }
    }
}

void PlayGame()
{
    cout<<"匹配成功啦!开始游戏吧..."<<endl;
}

void Game(string &ip,int &port,uint32_t &id)
{
    int select = 0;
    volatile bool quit = false;
    while(!quit)
    {
        cout<<"###############################"<<endl;
        cout<<"####  1.匹配      2.退出   ####"<<endl;
        cout<<"###############################"<<endl;
        cout<<"请选择:> ";
        cin>>select;
        switch(select)
        {
             case 1:
                 {
                     if(Match(ip,port,id))//匹配成功
                     {
                         PlayGame();//玩游戏
                     }
                     else//匹配失败
                     {
                         cout<<"匹配失败，请尝试重新匹配！"<<endl;
                     }
                 }
                 break;
            case 2:
                 quit = true;
                 break;
            default:
                 cout<<"选择有误，请重新选择！"<<endl;
                break;
        }
    }
}
