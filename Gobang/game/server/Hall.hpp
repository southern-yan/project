#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <pthread.h>
#include "PlayerManager.hpp"
#include "RoomManager.hpp"

#define MATCH_LEVEL 101 

using namespace std;

class Hall
{
    public:
        Hall()
            :match_pool(MATCH_LEVEL)
            ,match_num(0)
        {}

        int MatchNum()
        {
            return match_num;
        }

        void IncMatchNum()//递增匹配个数
        {
            match_num++;
        }

        void DecMatchNum()
        {
            match_num--;
        }

        void ResetMatchNum()//重置匹配个数
        {
            match_num=0;
        }

        void GetAllMatchId(vector<uint32_t> &id_list)//把匹配池中所有的id都放到id_list中
        {
            for(auto i=MATCH_LEVEL-1;i>=0;i--)
               {
                   auto& v=match_pool[i];//从胜率为100的开始遍历
                   if(v.empty())//为空
                   {
                       continue;
                   }
                   auto it = v.begin();
                   while(it!=v.end())
                   {
                       id_list.push_back(*it);
                       it++;
                   }
               }
        }

        void LockMatchPool()//给匹配池加锁
        {
            pthread_mutex_lock(&match_lock);
        }

        void UnlockMatchPool()//给匹配池解锁
        {
            pthread_mutex_unlock(&match_lock);
        }

        void ServiceWait()//匹配服务线程等待
        {
            pthread_cond_wait(&match_cond,&match_lock);
        }

        void ServiceWakeup()//唤醒服务线程
        {
            pthread_cond_signal(&match_cond);
        }

        uint32_t Register(string &name,string &passwd)
        {
            return pm.InsertPlayer(name,passwd);
        }

        uint32_t Login(uint32_t &id,string &passwd)
        {
            return pm.SearchPlayer(id,passwd);
        }
         
        bool PushIdInMatchPool(uint32_t& id)//把用户id放入匹配池中
        {
            LOG(INFO,"把用户id放入匹配池中...");
            pm.SetMatching(id);//把当前用户设置为匹配状态
            int rate=pm.GetRate(id);
            cout << "rate: " << rate << endl;

            LockMatchPool();//加锁
            auto &v = match_pool[rate];//拿到了相同胜率的vector
            auto it = v.begin();
            while(it!=v.end())
            {
                if(*it == id)//说明已经有了
                    return false;
                ++it;
            }
            v.push_back(id);//把用户插入到匹配池中
            IncMatchNum();//递增match_num
            UnlockMatchPool();//解锁
            ServiceWakeup();//唤醒匹配线程

           return true;
            //用户要等匹配线程把它匹配成功
          // return  pm.PlayerWait(id);//让用户进行等待
        }

        bool PopIdMatchPool(uint32_t &id)//把自己的id从匹配池拿走
        {
            LockMatchPool();
           // MatchPoolClear(id);
            int rate = pm.GetRate(id);//获得自己的胜率
            auto &v=match_pool[rate];//找到匹配池中对应的级别
            for(auto it = v.begin();it!=v.end();it++)//删掉用户自己的id
            {
                if(*it == id)
                {
                    v.erase(it);
                    break;
                }
            }
            DecMatchNum();//匹配池的个数减1
            UnlockMatchPool();
            pm.SetOnline(id);
            return true;
        }

        void MatchPoolClear(uint32_t &id)//清空匹配池
        {
            LOG(INFO,"匹配池被清空...");
            for(int i = MATCH_LEVEL-1;i >= 0;i--)
            {
                auto &v=match_pool[i];
                if(v.empty())
                {
                    continue;
                }
                vector<uint32_t>().swap(v);
            }
            ResetMatchNum();//清空所有id
            if(id>=1000)
            {
                pm.SetOnline(id);
            }
        }

        int IsPlayerReady(uint32_t &id)//检测用户状态是否是ready
        {
            return pm.Ready(id);
        }

        void GamePrepare(uint32_t &one,uint32_t &two)
        {
            pm.SetPlayerStatus(one,two);//设置玩家游戏状态
            uint32_t room_id = rm.CreateRoom(one,two);//创建房间
            pm.SetPlayerRoom(room_id,one,two);//设置玩家的房间号
            cout<< "debug: room_id: "<<room_id <<endl;
            //pm.SignalPlayer(one,two);//唤醒两个用户
        }

       static void *MatchService(void* arg)//提供一个线程不断检测当前用户是否可以匹配
        {
            pthread_detach(pthread_self());//分离该线程
            Hall *hp=(Hall*)arg;
            while(1)
            {
                uint32_t last = 0;
                 hp->LockMatchPool();//加锁
                 while(hp->MatchNum() < 2)//循环检测(防止误唤醒)
                 {
                     LOG(INFO,"服务线程开始等待啦...");
                    hp-> ServiceWait();//等待
                 }
                 LOG(INFO,"服务线程被唤醒了...");
                 vector<uint32_t> id_list;
                 hp->GetAllMatchId(id_list);
                 //两两开始匹配
                 int num=id_list.size();
                 if(num & 1)
                 {
                     last = id_list[id_list.size()-1];
                     num &=(~1);//保证num为偶数
                 }
                 else
                 {
                     last = 0;
                 }
                 for(int i = 0;i <= num;i+=2)
                 {
                     uint32_t play_one = id_list[i];
                     uint32_t play_two = id_list[i+1];
                     hp->GamePrepare(play_one,play_two);//游戏准备
                 }
                 //匹配完毕后，要清空匹配池
                 hp->MatchPoolClear(last);//清空当前匹配池内容
                 hp->UnlockMatchPool();//解锁
            }
        }

       string GetPlayerBoard(uint32_t &room_id)
       {
           string board;
           //uint32_t room_id=pm.GetPlayerRoomId(id);
           rm.GetBoard(room_id,board);
           return board;
       }

       uint32_t GetPlayerRoomId(uint32_t &id)
       {
          return pm.GetPlayerRoomId(id);
       }

       char GetPlayerPiece(uint32_t &room_id,uint32_t &id)
       {
           return rm.GetPlayerPiece(room_id,id);
       }

       bool IsMyTurn(uint32_t &room_id,uint32_t &id)
       {
           return rm.IsMyTurn(room_id,id);
       }

       void Step(uint32_t &room_id,uint32_t &id,int x,int y)
       {
           rm.Step(room_id,id,x,y);
       }

       char Judge(uint32_t &room_id,uint32_t &id)
       {
           return rm.Judge(room_id,id);
       }

        void InitHall()//初始化大厅
        {
            pthread_mutex_init(&match_lock,NULL);
            pthread_cond_init(&match_cond,NULL);

            pthread_t tid;
            pthread_create(&tid,NULL,MatchService,this);
        }

        ~Hall()
        {
            pthread_mutex_destroy(&match_lock);
            pthread_cond_destroy(&match_cond);
        }

    private:
        PlayerManager pm;
        RoomManager rm;
        vector<vector<uint32_t> > match_pool;//匹配池
        int match_num;//匹配用户的个数
        pthread_mutex_t match_lock;
        pthread_cond_t match_cond;
};
