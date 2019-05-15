#include "Room.hpp"
#include <iostream>
#include <unordered_map>
#include <string>

#define ROOM_ID 1024

using namespace std;
class RoomManager
{
    public:
        RoomManager()
            :assign_id(ROOM_ID)
        {

        }

        void CreateRoom(uint32_t &one,uint32_t &two)
        {
            Room r(one,two);
            uint32_t id=assign_id++;
            room_set.insert({id,r});
        }

        ~RoomManager()
        {

        }

    private:
        unordered_map<uint32_t,Room> room_set;
        int assign_id;//给用户分配房间
};
