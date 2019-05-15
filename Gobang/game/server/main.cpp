#include <rpc_server.h>
#include <string>
#include "Hall.hpp"

using namespace rest_rpc;
using namespace rpc_service;
using namespace std;

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

bool RpcMatchAndWait(connection* conn,uint32_t id)
{
    return GameHall.PushIdInMatchPool(id);
}

int RpcPlayerReady(connection* conn,uint32_t id)
{
    return GameHall.IsPlayerReady(id);
}

int main() {
	rpc_server server(9001,4);
    LOG(INFO,"初始化服务器成功...");
    server.register_handler("RpcRegister",RpcRegister);//把注册方法注册进来
    server.register_handler("RpcLogin",RpcLogin);//把登录方法注册进来
    server.register_handler("RpcMatchAndWait",RpcMatchAndWait);//把匹配并等待方法注册进来
    server.register_handler("RpcPlayerReady",RpcPlayerReady);//
    GameHall.InitHall();//初始化大厅

    LOG(INFO,"所有方法注册完毕...");
    LOG(INFO,"服务器开始启动...");
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
