#pragma once
#include <iostream>
#include <pthread.h>
#include <string>
#include <time.h>

#define TIMEOUT 20
using namespace std;

typedef enum
{
    OFFLINE = 0,//离线
    ONLINE,//刚刚登陆
    MATCHING,//匹配中
    PLAYING,//游戏中
}status_t;

class Player//玩家用户
{
    public:
        Player()
        {}
        Player(string &_name,string &_passwd,uint32_t &_id)
            :name(_name)
            ,passwd(_passwd)
            ,id(_id)
        {
            win = 0;
            lose = 0;
            tie = 0;
            st = OFFLINE;
            //pthread_mutex_init(&lock,NULL);
           // pthread_cond_init(&cond,NULL);
        }
        const string& GetPasswd()
        {
            return passwd;
        }

        void Online()//上线
        {
            st=ONLINE;
        }

        void Matching()//匹配
        {
            st=MATCHING;
        }

        void Playing()//游戏中
        {
            st=PLAYING;
        }

        int Status()
        {
            return st;
        }

       int Rate()//胜率
       {
           int total = win+lose;
           if(total == 0)//说明一次都没有玩过
           {
               return 0;
           }

           return win*100/total;//取值范围是0-100
       }

       uint32_t Room()
       {
           return room_id;
       }

       void SetRoom(uint32_t &_room_id)
       {
           room_id = _room_id;
       }

      // int Wait()//等待
      // {
      //    struct timespec ts;
      //    clock_gettime(CLOCK_REALTIME,&ts);
      //    ts.tv_sec+=TIMEOUT;
      //    return pthread_cond_timedwait(&cond,&lock,&ts);
      // }

      // void Signal()//唤醒
      // {
      //     pthread_cond_signal(&cond);
      // }

       ~Player()
        {
            //pthread_mutex_destroy(&lock);
           // pthread_cond_destroy(&cond);
        }
   
    private:
        //基本信息
        string name;
        string passwd;
        uint32_t id;

        //游戏信息
        int win;//赢的次数
        int lose;//输的次数
        int tie;//平局的次数

        status_t st;//状态
        uint32_t room_id;//房间号

        //pthread_mutex_t lock;//一把锁
        //pthread_cond_t cond;//条件变量
};
