#ifndef THREAD_MANAGER_INCLUDED__H
#define THREAD_MANAGER_INCLUDED__H

class ThreadManager
{
public:
	ThreadManager(int threadCount);
	~ThreadManager();

	void CreateThreads();

private:
	int m_threadCount;

};
#endif