#ifndef _SAFEQUEUE_H__
#define _SAFEQUEUE_H__

#include <Windows.h>
#include <optional>
#include <deque>

template <typename T>
class SafeQueue
{
public:
    SafeQueue();
    ~SafeQueue();

    void Push(const T& data);
    std::optional<T> Pop();  
    std::optional<T> Front();
    std::optional<T> Back(); 
    bool IsEmpty();

private:
    std::deque<T> m_deque;
    CRITICAL_SECTION m_criticalSection;
};

template <typename T>
SafeQueue<T>::SafeQueue()
{
    InitializeCriticalSection(&m_criticalSection);
}

template <typename T>
SafeQueue<T>::~SafeQueue()
{
    EnterCriticalSection(&m_criticalSection);
    m_deque.clear();
    LeaveCriticalSection(&m_criticalSection);
    DeleteCriticalSection(&m_criticalSection);
}

template <typename T>
void SafeQueue<T>::Push(const T& data)
{
    EnterCriticalSection(&m_criticalSection);
    m_deque.push_back(data);
    LeaveCriticalSection(&m_criticalSection);
}

template <typename T>
std::optional<T> SafeQueue<T>::Pop()
{
    EnterCriticalSection(&m_criticalSection);
    if (m_deque.empty())
    {
        LeaveCriticalSection(&m_criticalSection);
        return {};
    }
    T v = m_deque.front();
    m_deque.pop_front();
    LeaveCriticalSection(&m_criticalSection);
    return v;
}

template <typename T>
std::optional<T> SafeQueue<T>::Front()
{
    EnterCriticalSection(&m_criticalSection);
    if (m_deque.empty())
    {
        LeaveCriticalSection(&m_criticalSection);
        return {};
    }
    T v = m_deque.front();
    LeaveCriticalSection(&m_criticalSection);
    return v;
}

template <typename T>
std::optional<T> SafeQueue<T>::Back()
{
    EnterCriticalSection(&m_criticalSection);
    if (m_deque.empty())
    {
        LeaveCriticalSection(&m_criticalSection);
        return {};
    }
    T v = m_deque.back();
    LeaveCriticalSection(&m_criticalSection);
    return v;
}

template <typename T>
bool SafeQueue<T>::IsEmpty()
{
    EnterCriticalSection(&m_criticalSection);
    bool empty = m_deque.empty();
    LeaveCriticalSection(&m_criticalSection);
    return empty;
}

#endif // _SAFEQUEUE_H__
