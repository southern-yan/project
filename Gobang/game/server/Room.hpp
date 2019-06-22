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
        Room()
        {}

        Room(uint32_t &id1,uint32_t &id2)
            :one(id1)
            ,two(id2)
        {
            piece[0]='X';
            piece[1]='O';
            memset(board,' ',sizeof(board));//初始化棋盘
            result='N';//结果默认是继续
            current=one;
            pthread_mutex_init(&lock,NULL);
        }

        void Board(string &_board)
        {
            for(auto i = 0;i< SIZE;i++)
            {
                for(auto j = 0;j<SIZE;j++)
                {
                    _board.push_back(board[i][j]);
                }
            }
        }

        char Piece(uint32_t &id)
        {
            int pos =0;
            if(id == one)
            {
                pos = 0;
            }
            else
            {
                pos = 1;
            }

            return piece[pos];
        }

        bool IsMyTurn(uint32_t &id)
        {
            return id ==current ? true : false;
        }

        void Step(uint32_t &id,int &x,int &y)
        {
            if(current == id)
            {
                int pos = (id == one ? 0 : 1);
                board[x][y] = piece[pos];
                current = (id == one ? two : one);
                result=Judge();
            }
        }

        char GameResult(uint32_t &id)
        {
            return result;
        }
        
        char Judge()
        {
            int row = SIZE;
            int col = SIZE;
            for(auto i = 0;i < row;i++)//行
            {
                if(board[i][0]!=' '&& \
                   board[i][0] == board[i][1]&& \
                   board[i][1] == board[i][2]&& \
                   board[i][2] == board[i][3]&& \
                   board[i][3] == board[i][4])
                {
                    return board[i][0];
                }
            }

            for(auto i = 0;i < col;i++)//列
            {
                if(board[0][i]!=' '&& \
                   board[0][i] == board[1][i]&& \
                   board[1][i] == board[2][i]&& \
                   board[2][i] == board[3][i]&& \
                   board[3][i] == board[4][i])
                {
                    return board[0][i];
                }
            }

            //正对角线
            if(board[0][0]!=' '&& \
               board[0][0] == board[1][1]&& \
               board[1][1] == board[2][2]&& \
               board[2][2] == board[3][3]&& \
               board[3][3] == board[4][4])
              {
                    return board[0][0];
              }

           //反对角线
           if(board[0][4]!=' '&& \
              board[0][4] == board[1][3]&& \
              board[1][3] == board[2][2]&& \
              board[2][2] == board[3][1]&& \
              board[3][1] == board[4][0])
             {
                    return board[0][4];
             }

           //判断棋盘是否已满
           for(auto i =0;i < row ;i++)
           {
               for(auto j =0;j < col;j++)
               {
                   if(board[i][j] == ' ')
                       return 'N';
               }
           }
           return 'E';
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
