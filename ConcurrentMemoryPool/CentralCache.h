#pragma once
#include "Common.h"



//CentralCache要设计为单例模式
class CentralCache
{
public:
	static CentralCache* GetInstance()//获取一份实例对象
	{
		return &_inst;
	}
	//从page cache获取一个span
	Span* GetOneSpan(SpanList& spanlist, size_t byte_size);

	//从中心缓存获取一定数量的对象返回给thread cache
	size_t FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size);

	//将一定数量的对象释放到span跨度
	void ReleaseListToSpans(void* start, size_t size);



	CentralCache(const CentralCache&) = delete;//防止拷贝
	CentralCache& operator=(const CentralCache&) = delete;//防赋值

private:
	SpanList _spanlist[NLISTS];

	CentralCache(){}//拷贝构造私有化

	//懒汉模式在第一次调用时才创建，多个线程第一次调用时，需要加锁，一加锁效率就会降低，所以不建议使用懒汉模式
	static CentralCache _inst;//这里写的是饿汉模式，本身就是线程安全的，它在main函数之前就创建，而线程是在main函数之后才创建的，这样的话，就不用加锁了，也没有双重检查
};
