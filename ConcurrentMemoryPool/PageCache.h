#pragma once
#include "Common.h"

//PageCache也要设计成单例模式

class PageCache
{
public:
 static PageCache* GetInstance()
	{
		return &_inst;
	}

 Span* NewSpan(size_t n);//申请一个新的span

 PageCache(const PageCache&) = delete;//拷贝构造函数声明为delete函数
 PageCache& operator=(const PageCache&) = delete;//赋值函数声明为delete函数

private:
	PageCache(){}//构造函数私有化
	static PageCache _inst;

};
