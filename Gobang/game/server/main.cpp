#include <rpc_server.h>
#include <string>
#include "Hall.hpp"

using namespace rest_rpc;
using namespace rpc_service;
using namespace std;

    //GameHall.InitHall();//初始化大厅
//#include "qps.h"
//
//struct dummy{
//	int add(connection* conn, int a, int b) { return a + b; }
//};
//
//std::string translate(connection* conn, const std::string& orignal) {
//	std::string temp = orignal;
//	for (auto& c : temp) { 
//		c = std::toupper(c); 
//	}
//	return temp;
//}
//
//void hello(connection* conn, const std::string& str) {
//	std::cout << "hello " << str << std::endl;
//}
//
//struct person {
//	int id;
//	std::string name;
//	int age;
//
//	MSGPACK_DEFINE(id, name, age);
//};
//
//std::string get_person_name(connection* conn, const person& p) {
//	return p.name;
//
//
//person get_person(connection* conn) {
//	return { 1, "tom", 20 };
//}
//
//void upload(connection* conn, const std::string& filename, const std::string& content) {
//	std::cout << content.size() << std::endl;
//	std::ofstream file(filename, std::ios::binary);
//	file.write(content.data(), content.size());
//}
//
//std::string download(connection* conn, const std::string& filename) {
//	std::ifstream file(filename, std::ios::binary);
//	if (!file) {
//		return "";
//	}
//
//	file.seekg(0, std::ios::end);
//	size_t file_len = file.tellg();
//	file.seekg(0, std::ios::beg);
//	std::string content;
//	content.resize(file_len);
//	file.read(&content[0], file_len);
//	std::cout << file_len << std::endl;
//
//	return content;
//}
//
//qps g_qps;
//
//std::string get_name(connection* conn, const person& p) {
//	g_qps.increase();
//	return p.name;
//}

Hall GameHall;//定义一个全局变量

uint32_t RpcRegister(connection* conn,string name,string passwd)
{
    //std::cout<<"获得一个注册请求:昵称->"<<name<<" 密码-> "<<passwd<< std::endl;
    return GameHall.Register(name,passwd);
}

uint32_t RpcLogin(connection* conn,uint32_t id,string passwd)
{
    return GameHall.Login(id,passwd);
}

bool RpcMatchAndWait(connection* conn,uint32_t id)//1.把自己的id放入匹配池并等待匹配
{
    return GameHall.PushIdInMatchPool(id);
}

int RpcPlayerReady(connection* conn,uint32_t id)//2.用户检查自己是否匹配成功
{
    return GameHall.IsPlayerReady(id);
}

string RpcBoard(connection* conn,uint32_t room_id)//5.获得棋盘
{
   return GameHall.GetPlayerBoard(room_id);
}

uint32_t RpcPlayerRoomId(connection* conn ,uint32_t id)//3.获得对应的房间号
{
    return GameHall.GetPlayerRoomId(id);
}

char RpcPlayerPiece(connection* conn ,uint32_t room_id,uint32_t id)//4.获得自己的棋子
{
    return GameHall.GetPlayerPiece(room_id,id);
}

bool RpcIsMyTurn(connection* conn,uint32_t room_id,uint32_t id)//6.玩游戏时，判断是否该自己走
{
    return GameHall.IsMyTurn(room_id,id);
}

void RpcStep(connection* conn,uint32_t room_id,uint32_t id,int x,int y)//6.用户开始下棋(也就是把特定的下标设置为对应的棋子即可)
{
     GameHall.Step(room_id,id,x,y);
}

char RpcJudge(connection* conn,uint32_t room_id,uint32_t id)//7.判定输赢(判定特定房间里的特定用户是否赢)
{
    return GameHall.Judge(room_id,id);
}

bool RpcPopMatchPool(connection* conn,uint32_t id)//8.把自己的id从匹配池拿走
{
    return GameHall.PopIdMatchPool(id);
}

int main() {
	rpc_server server(9001,4);
    LOG(INFO,"初始化服务器成功...");
    server.register_handler("RpcRegister",RpcRegister);//把注册方法注册进来
    server.register_handler("RpcLogin",RpcLogin);//登录
    server.register_handler("RpcMatchAndWait",RpcMatchAndWait);//匹配并等待
    server.register_handler("RpcPlayerReady",RpcPlayerReady);//检测用户是否已经就绪(就绪是指用户已经准备好开始游戏了)
    server.register_handler("RpcPlayerRoomId",RpcPlayerRoomId);//获得房间号
    server.register_handler("RpcPlayerPiece",RpcPlayerPiece);//获得棋盘的棋子
    server.register_handler("RpcBoard",RpcBoard);//获得棋盘
    server.register_handler("RpcIsMyTurn",RpcIsMyTurn);//是否该我走
    server.register_handler("RpcStep",RpcStep);//玩家下棋
    server.register_handler("RpcJudge",RpcJudge);//判断输赢
    server.register_handler("RpcPopMatchPool",RpcPopMatchPool);//把自己的id从匹配池拿走

    LOG(INFO,"所有方法注册完毕...");
    LOG(INFO,"服务器开始启动...");

    GameHall.InitHall();//初始化大厅
	//dummy d;
	//server.register_handler("add", &dummy::add, &d);
	//server.register_handler("translate", translate);
	//server.register_handler("hello", hello);
	//server.register_handler("get_person_name", get_person_name);
	//server.register_handler("get_person", get_person);
	//server.register_handler("upload", upload);
	//server.register_handler("download", download);
	//server.register_handler("get_name", get_name);

	server.run();

	std::string str;
	std::cin >> str;
}
