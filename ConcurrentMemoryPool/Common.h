#pragma once

#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>

using std::cout;
using std::endl;

const size_t MAX_BYTES = 64 * 1024; //thread cache申请的最大内存
const size_t NLISTS = MAX_BYTES/8;//自由链表中有这么多个自由链表


inline static void*& NEXT_OBJ(void* obj)//获取当前对象的下一个对象
{
	return *((void**)obj);
}

class FreeList
{
public:
	void PushRange(void* start, void* end, size_t n)//插入批量的对象
	{
		NEXT_OBJ(end) = _list;
		start = _list;
		_size += n;
	}

	void PopRange()
	{

	}

	void Push(void* obj)//插入一个对象
	{
		NEXT_OBJ(obj) = _list;
		_list = obj;
		_size++;
	}
	void* Pop()//弹出一个对象
	{
		/*void* next = *((void**)_list);
		void* cur = _list;
		_list = next;*/

		void* obj = _list;
		_list = NEXT_OBJ(obj);
		_size--;

		return obj;//返回被弹出的对象
	}
	bool Empty()
	{
		return _list == nullptr;
	}
private:
	void* _list=nullptr;//对象里面有一个指针，指向后面的对象块
	size_t _size=0;//记录自由链表里面有多少个对象

};

class Size
{
public:
	inline static size_t Index(size_t size)//利用一个size计算，这个size在自由链表的哪个位置
	{	
		return _Index(size, 3);
	}

	inline static size_t Roundup(size_t size)//对齐到8字节的整数倍
	{
		return _Roundup(size, 3);//这里之所以要封装对齐数，是因为考虑到对齐数越小，内存碎片问题越轻
	}
private:
	inline static size_t _Index(size_t size, size_t align)//利用一个size计算，这个size在自由链表的哪个位置
	{
		//9-16(+7后)  --->16-23
		//return ((size+7)>>3)-1;
		size_t _align = 1 << align;
		return ((size + _align - 1) >> align) - 1;
	}

	inline static size_t _Roundup(size_t size, size_t align)//对齐到8字节的整数倍
	{

		//return (size+7)&~7;
		size_t _align = 1 << align;
		return (size + _align - 1)&~_align - 1;//size+7再按位与上7取反，刚好每次都把低三位置为0，就可以达到每次都对齐到8的整数倍处
	}
};

typedef size_t PageID;

struct Span//公有的，给别人访问的一般就不用设计成class了
{
	PageID _pageid;//页号
	size_t _npage;//页数
	
	Span* _next = nullptr;
	Span* _prev = nullptr;

	void* _list = nullptr;//链接对象的自由链表(_list指向第一个对象)
	size_t objsize = 0;//一个对象的大小

	size_t usecount = 0;//对象的使用计数

};

//双向带头循环的span链表
class SpanList
{
public:
	SpanList()//构造函数
	{
		_head = new Span;
		_head->_next = _head;
		_head->_prev = _head;
	}

	Span* Begin()
	{
		return _head->_next;
	}
	Span* End()
	{
		return _head;
	}

	void PushBack(Span* newspan)
	{
		Insert(End(), newspan);
	}

	void PushFront(Span* newspan)
	{
		Insert(Begin(), newspan);
	}

	void PopBack()
	{
		Erase(_head->_prev);
	}

	void PopFront()
	{
		Erase(_head->_next);
	}

	void Insert(Span* cur, Span* newspan)//在cur的前面插入一个newspan
	{
		//prev newspan cur
		Span* prev = cur->_prev;

		newspan->_prev = prev;
		prev->_next = newspan;
		newspan->_next = cur;
	}

	void Erase(Span* cur)
	{
		//prev cur  next
		Span* prev = cur->_prev;
		Span* next = cur->_next;

		prev->_next = next;
		next->_prev = prev;
	}

	~SpanList()
	{
		Span* cur = _head->_next;
		while (cur != _head)
		{
			Span* next = cur->_next;
			delete cur;
			cur = next;
		}
		delete _head;
		_head = nullptr;
	}

	SpanList(const SpanList&) = delete;//没有拷贝的需求，但是不写，编译器会自动生成浅拷贝，所以就直接把拷贝构造设置为delete函数
	SpanList& operator=(const SpanList&) = delete;//赋值也一样

private:
	Span* _head;
};
