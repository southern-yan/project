#include "ThreadCache.h"
#include "Common.h"
#include "ConcurrentAlloc.h"

//void SizeTest()
//{
//	cout<<Size::Index1(7)<<endl;
//	cout << Size::Index1(8)<<endl;
//
//	cout << Size::Index2(7) << endl;
//	cout << Size::Index2(8) << endl;
//
//	cout << Size::Roundup(10) << endl;
//	cout << Size::Roundup(16) << endl;
//}

void Alloc(size_t n)
{
	std::vector<void*> v;
	for (size_t i = 0; i < n; i++)
	{
		//cout << ConcurrentAlloc(10) << endl;
		v.push_back(ConcurrentAlloc(10));
	
	}

	for (size_t i = 0; i < n; i++)
	{
		ConcurrentFree(v[i],10);
		cout << v[i] << endl;
	}
	v.clear();
	//ÔÙÉêÇëÒ»ÂÖ
	cout << endl << endl;
	for (size_t i = 0; i < n; i++)
	{
		v.push_back(ConcurrentAlloc(10));

	}

	for (size_t i = 0; i < n; i++)
	{
		ConcurrentFree(v[i], 10);
		cout << v[i] << endl;
	}
	v.clear();
}

void TestThreadCache()
{
	std::thread t1(Alloc, 5);
	std::thread t2(Alloc, 5);
	std::thread t3(Alloc, 5);
	std::thread t4(Alloc, 5);

	t1.join();
	t2.join();
	t3.join();
	t4.join();

}

void TestCentralCache()
{
	std::vector<void*> v;
	for (size_t i = 0; i < 8; i++)
	{
		v.push_back(ConcurrentAlloc(10));
	}
	for (size_t i = 0; i < 8; i++)
	{
		//ConcurrentFree(v[i], 10);
		cout << v[i] << endl;
	}
}


int main()
{
	
	//SizeTest();
	//TestThreadCache();
	TestCentralCache();
	system("pause");
	return 0;
}