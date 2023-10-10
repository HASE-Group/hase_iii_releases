#pragma once

struct _SHMutex;

class CHMutex
{
private:
    _SHMutex* m_pMutex;
public:
    CHMutex();
    ~CHMutex();
    void Lock();
    void Unlock();
};


