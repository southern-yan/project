#pragma once
#include <iostream>
#include <string>

using namespace std;

#define SIZE 5
#define BLACK 'X'//黑子
#define WHITE 'O'//白子


class Room
{
    public:
        Room(uint32_t &id1,uint32_t &id2)
            :one(id1)
            ,two(id2)
        {
            piece[0]='X';
            piece[1]='O';
            memset(board,' ',sizeof(board));//初始化棋盘
            result='N';//结果默认是继续
            pthread_mutex_init(&lock,NULL);
        }




        ~Room()
        {
            pthread_mutex_destroy(&lock);
        }

    private:
        uint32_t one;//指定one用户拿的是黑子'X'
        uint32_t two;//指定two用户拿的是白子'O'
        char piece[2];//规定0号下标对应one的棋子，1号下标对应two的棋子
        uint32_t current;//当前该谁走
        char board[SIZE][SIZE];//定义棋盘
        char result;//  X(one用户赢), O(two用户赢), E(平局),N(继续)

        pthread_mutex_t lock;//定义一把锁

};
