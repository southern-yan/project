#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include "Player.hpp"
#include "Log.hpp"

#define DEFAULT_ID 1000

using namespace std;

class PlayerManager
{
    public:
        PlayerManager()
            :assign_id(DEFAULT_ID)
        {}
       
        uint32_t InsertPlayer(string &name,string &passwd)//插入用户
        {
            uint32_t _id=assign_id++;//key值
            Player p(name,passwd,_id);//定义一个用户（value值）
            player_set.insert({_id,p});
            LOG(INFO,"用户插入完毕....");

            return _id;
        }

        uint32_t SearchPlayer(uint32_t id,string &passwd)//查找用户
        {
            auto it=player_set.find(id);
            if(it!=player_set.end())
            {
                Player &p=it->second;//拿到用户所对应的second信息
                if(p.GetPasswd()!=passwd)
                {
                    LOG(WARNING,"用户密码错误...");
                    return 2;//认证失败
                }

                p.Online();//把用户状态设置为上线状态
                LOG(INFO,"用户进入上线状态...");
                return id;
            }
            else
            {
                LOG(WARNING,"用户不存在....");
                return 1;//用户不存在
            }
        }

        int GetRate(uint32_t &id)
        {
            return player_set[id].Rate();
        }

        //bool PlayerWait(uint32_t &id)
        //{
        //   if(player_set[id].Wait()==ETIMEDOUT)
        //   {
        //       return false;
        //   }
        //   return true;
        //}

       //void SignalPlayer(uint32_t &one,uint32_t &two)
       //{
       //    player_set[one].Signal();
       //    player_set[two].Signal();
       //}

       void SetPlayerStatus(uint32_t &one,uint32_t &two)
       {
           player_set[one].Playing();
           player_set[two].Playing();
       }

       void SetMatching(uint32_t &id)
       {
           player_set[id].Matching();
       }

       int Ready(uint32_t &id)//判断用户是否匹配成功
       {
            return player_set[id].Status();
       }

       ~PlayerManager()
       {}
    private:
        unordered_map<uint32_t,Player> player_set;
        uint32_t assign_id;//所有的用户id都是由assign_id分配的（每次加1）
};
