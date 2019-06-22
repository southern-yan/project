#pragma once
#include <iostream>
#include <unordered_map>
#include <string>
#include <utility>
#include <pthread.h>
#include "Player.hpp"
#include "Log.hpp"

#define DEFAULT_ID 1000

using namespace std;

class PlayerManager
{
    public:
        PlayerManager()
            :assign_id(DEFAULT_ID)
        {
            pthread_mutex_init(&lock,NULL);
        }

        void Lock()
        {
            pthread_mutex_lock(&lock);
        }

        void Unlock()
        {
            pthread_mutex_unlock(&lock);
        }

        uint32_t InsertPlayer(string &name,string &passwd)//插入用户
        {
            Lock();
            uint32_t _id=assign_id++;//key值
            Player p(name,passwd,_id);//定义一个用户（value值）
            player_set.insert({_id,p});
            Unlock();
            LOG(INFO,"用户插入完毕....");

            return _id;
        }

        uint32_t SearchPlayer(uint32_t id,string &passwd)//查找用户
        {
            Lock();
            auto it=player_set.find(id);
            if(it!=player_set.end())
            {
                Player &p=it->second;//拿到用户所对应的second信息
                if(p.GetPasswd()!=passwd)
                {
                    Unlock();
                    LOG(WARNING,"用户密码错误...");
                    return 2;//认证失败
                }

                p.Online();//把用户状态设置为上线状态
                Unlock();
                LOG(INFO,"用户进入上线状态...");
                return id;
            }
            else
            {
                LOG(WARNING,"用户不存在....");
                Unlock();
                return 1;//用户不存在
            }
        }

        int GetRate(uint32_t &id)
        {
            Lock();
            int rate = player_set[id].Rate();
            Unlock();
            return rate;
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
    
        uint32_t GetPlayerRoomId(uint32_t &id)
        {
            Lock();
           uint32_t _room_id = player_set[id].Room();
            Unlock();
            return _room_id;
        }

       void SetPlayerStatus(uint32_t &one,uint32_t &two)
       {
           Lock();
           player_set[one].Playing();
           player_set[two].Playing();
           Unlock();
       }

       void SetPlayerRoom(uint32_t &room_id,uint32_t &one,uint32_t &two)
       {
           Lock();
           player_set[one].SetRoom(room_id);
           player_set[two].SetRoom(room_id);
           Unlock();
       }

       void SetMatching(uint32_t &id)
       {
           Lock();
           player_set[id].Matching();
           Unlock();
       }

       void SetOnline(uint32_t &id)
       {
           Lock();
           player_set[id].Online();
           Unlock();
       }

       int Ready(uint32_t &id)//判断用户是否匹配成功
       {
           Lock();
           int st= player_set[id].Status();
           Unlock();
           return st;
       }

       ~PlayerManager()
       {
           pthread_mutex_destroy(&lock);
       }
    private:
        unordered_map<uint32_t,Player> player_set;
        uint32_t assign_id;//所有的用户id都是由assign_id分配的（每次加1）
        pthread_mutex_t lock;
};
