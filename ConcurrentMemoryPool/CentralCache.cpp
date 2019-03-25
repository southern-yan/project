#include "CentralCache.h"
#include "PageCache.h"


CentralCache CentralCache::_inst;//在类外定义

//从PageCache获取一个span
Span* CentralCache::GetOneSpan(SpanList& spanlist, size_t byte_size)
{
	Span* span = spanlist.Begin();
	while (span != spanlist.End())
	{
		if (span->_list != nullptr)//判断一个span是否为空
		{
			return span;
		}
		else
		{
			span = span->_next;
		}			
	}

	//走到这里，表示span是空的
	Span* newspan = PageCache::GetInstance()->NewSpan(1);
	//Span* newspan = new Span;
	//newspan->objsize = 16;
	//void* ptr = malloc(16 * 8);
	//void* cur = ptr;
	////切分对象
	//for (size_t i = 0; i < 7; i++)
	//{
	//	//把切的对象给链接起来
	//	void* next = (char*)cur + 16;
	//	NEXT_OBJ(cur) = next;
	//	cur = next;
	//}

	//NEXT_OBJ(cur) = nullptr;
	//newspan->_list = ptr;

	//把span插入到当前这个链表中
	spanlist.PushFront(newspan);

	return newspan;

}

//从中心缓存获取一定数量的对象返回给thread cache
size_t CentralCache::FetchRangeObj(void*& start, void*& end, size_t n, size_t byte_size)
{
	//加锁

	size_t index = Size::Index(byte_size);//计算所给大小对应哪一层的spanlist
	SpanList& spanlist = _spanlist[index];
	Span* span = GetOneSpan(spanlist, byte_size);

	//从span中获取一定范围内的对象
	size_t batchsize = 0;//统计获取对象的个数
	void* prev = nullptr;
	void*cur = span->_list;
	for (size_t i = 0; i < n; i++)
	{
		prev = cur;
		cur = NEXT_OBJ(cur);
		batchsize++;
		if (cur == nullptr)//如果想要获取的对象个数n大于实际有的对象，就要判断，以防越界
			break;
	}
	start = span->_list;
	end = prev;
	span->_list = cur;//拿走对象后，让span的_list指向新的第一个对象
	span->usecount += batchsize;
	
	if (span->_list == nullptr)//如果span为空，就把它尾插到最后，保持非空的span在前面
	{
		spanlist.Erase(span);//只是移除，不是彻底删除
		spanlist.PushBack(span);
	}

	return batchsize;

}