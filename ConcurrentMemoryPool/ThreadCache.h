#pragma once
#include "Common.h"

class ThreadCache
{
public:
	//申请内存对象
	void* Allocate(size_t size);

	//释放内存对象
	void Deallocate(void* ptr, size_t size);

	//从中心缓存获取对象
	void* FetchFromCentralCache(size_t index, size_t size);

	//释放对象时，如果链表过长，就对象将放回到中心缓存
	void ListTooLong(FreeList* list, size_t size);

private:
	FreeList _FreeList[NLISTS];//对象数组
	
};
//TLS
_declspec (thread) static ThreadCache* tlslist = nullptr;//把它定义为静态的，是为了只在当前文件可见