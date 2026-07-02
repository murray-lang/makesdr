#include "../include/thread/Mutex.h"
#include <chrono>

Mutex::Mutex()
{
}

Mutex::~Mutex()
{
}

void Mutex::lock()
{
    m_mutex.lock();
}

bool Mutex::tryLock()
{
    return m_mutex.try_lock();
}

// bool Mutex::tryLockFor(uint32_t timeoutMs)
// {
//     return m_mutex.try_lock_for(std::chrono::milliseconds(timeoutMs));
// }

void Mutex::unlock()
{
    m_mutex.unlock();
}

// LockGuard implementation
LockGuard::LockGuard(Mutex& mutex)
    : m_mutex(mutex)
{
    m_mutex.lock();
}

LockGuard::~LockGuard()
{
    m_mutex.unlock();
}
