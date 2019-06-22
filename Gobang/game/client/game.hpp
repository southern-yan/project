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
    return 1;
}

bool PopMatchPool(string &ip,int &port,uint32_t &id)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

      return client.call<bool>("RpcPopMatchPool",id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
    return false;
}

bool Match(string &ip,int &port,uint32_t &id)
{
    int count =20;
    PushMatchPool(ip,port,id);//把用户id放入匹配池
    while(1)
    {
        int ret=CheckReady(ip,port,id);//检测用户的状态
        //cout<<"check: "<<ret<<endl;
        if(ret == 3)
        {
            return true;
        }
        else if(ret == 1)
        {
            cout<<"匹配失败!"<<endl;
            return false;
        }
        else
        {
            printf("匹配中........................ %2d\r",count--);
            fflush(stdout);
            if(count < 0)
            {
                cout<<endl;
                cout<<"匹配超时!"<<endl;
                PopMatchPool(ip,port,id);
                //remove client
                break;
            }
            sleep(1);
        }
    }
    return false;
}

int GetBoard(string &ip,int &port,uint32_t &room_id,string &board)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

        board = client.call<string>("RpcBoard",room_id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
}

uint32_t GetMyRoomId(string &ip,int &port, uint32_t &id)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

        return client.call<uint32_t>("RpcPlayerRoomId",id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
}

char GetMyPiece(string &ip,int &port,uint32_t room_id,uint32_t id)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

        return client.call<char>("RpcPlayerPiece",room_id,id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
}

void ShowBoard(string &board)//显示棋盘
{
    cout<< "    ";
    for(auto i=1;i <= 5;i++)
    {
        cout<< i <<"   ";//输入3个空格
    }
    cout<<endl;
    for(auto i=0;i <= 5;i++)
    {
        cout<< "----";
    }
    cout<<endl;
    int size = board.size()/5;
    for(auto i=0;i<size;i++)
    {
        cout<< i+1 <<" |";
        for(auto j=0;j<size;j++)
        {
            cout<<" "<<board[i*size+j]<<" |";
        }
        cout<<endl;
        for(auto i=0;i<= 5;i++)
        {
           cout<< "----";
        }
        cout<<endl;
    }


}

bool IsMyTurn(string &ip,int &port,uint32_t &room_id,uint32_t &id)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

        return client.call<bool>("RpcIsMyTurn",room_id,id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
    return false;
}

bool PosIsRight(string &board,int x,int y)
{
    int pos=(x-1)*5+(y-1);
    return board[pos] == ' ' ? true : false;
}

int Step(string &ip,int &port,uint32_t &room_id,uint32_t &id,int x,int y)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

        client.call<void>("RpcStep",room_id,id,x-1,y-1);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
}

char Judge(string &ip,int &port,uint32_t room_id,uint32_t &id)
{
    try {
		rpc_client client(ip, port);
		bool r = client.connect();
		if (!r)
        {
			std::cout << "connect timeout" << std::endl;
			return 3;
		}

        return client.call<char>("RpcJudge",room_id,id);
	}
	catch (const std::exception& e) {
	std::cout << e.what() << std::endl;
    }
    return 'N';
}

void PlayGame(string &ip,int &port, uint32_t &id)
{
    int x,y;
    char result = 'N';
    string board;
    uint32_t room_id=GetMyRoomId(ip,port,id);//获得我的房间号
    if(room_id<1024)
    {
        return;
    }
    cout<<"room_id: "<<room_id<<endl;
    char piece=GetMyPiece(ip,port, room_id,id);//获得我的棋子
    cout<<"piece: "<<piece<<endl;
    while(1)
    {
        //system("clear");
        GetBoard(ip,port,room_id,board);//获得棋盘
        ShowBoard(board);//展示棋盘

        //判断当前该谁走
        if((result = Judge(ip,port,room_id,id))!='N')//判定结果
        {
            break;
        }
        if(IsMyTurn(ip,port,room_id,id))//当前是否该我走
        {
            cout<<"请输入你的落子位置:> ";
            cin>>x>>y;
            if(x>= 1 && x<= 5 && y >=1 && y<= 5)
            {
                if(!PosIsRight(board,x,y))
                {
                    cout<<"输入位置已被占用，请重新输入！"<<endl;
                }
                else//坐标合法,且没有被占用
                {
                    Step(ip,port,room_id,id,x,y);//该id这个用户走，走的坐标为(x,y)
                    result = Judge(ip,port,room_id,id);//判断当前谁赢谁输
                    if(result != 'N')//不是继续，说明有结果了
                    {
                        break;
                    }
                }
            }
            else
            {
                cout<<"您输入的位置有误，请重新输入！"<<endl;
            }
        }
        else//不该我走
        {
            cout<<"对方正在思考，请稍等..."<<endl;
            sleep(1);
            continue;
        }
    }

    if(result == 'E')
    {
        cout<<"双方达成平局..."<<endl;
    }
    else if(result == piece)
    {
        cout<<"恭喜您，赢得游戏..."<<endl;
    }
    else
    {
        cout<<"很遗憾，您输了..."<<endl;
    }
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
                         PlayGame(ip,port,id);//玩游戏
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
