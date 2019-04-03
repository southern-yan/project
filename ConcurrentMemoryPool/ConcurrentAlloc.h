#pragma once
#include "Common.h"
#include "ThreadCache.h"

inline static void* ConcurrentAlloc(size_t size)//多个线程并发申请内存
{
	if (size > MAX_BYTES)
	{
		//向pagecache申请
		return malloc(size);
	}
	else//每个线程首先要能找到属于自己的ThreadCache，这时候，就引入了TLS
	{
		if (tlslist == nullptr)
		{
			//cout << std::this_thread::get_id() << endl;//获取线程id
			tlslist = new ThreadCache;
			//cout << tlslist << endl;
		}
		//cout << tlslist << endl;


		return tlslist->Allocate(size);
	}
}

inline static void ConcurrentFree(void* ptr, size_t size)//并发释放内存
{
	if (size > MAX_BYTES)
	{
		free(ptr);
	}
	else
	{
		tlslist->Deallocate(ptr,size);
	}
}