
//=============================================================================
// Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XBlockingQueue.h$
// $Revision:$
// $Date:$
// $Author: bwilson$
//=============================================================================

#ifndef XSDK_XBLOCKING_QUEUE_H
#define XSDK_XBLOCKING_QUEUE_H

#include "XSDK/XCondition.h"
#include "XSDK/XGuard.h"
#include <list>

namespace XSDK
{

template<class T>
class XBlockingQueue
{
public:
    typedef std::list<T> QueueContainer;

    XBlockingQueue(void) :
        _queue(),
        _condLock(),
        _cond(_condLock),
        _waiting(true)
    {
    }

    virtual ~XBlockingQueue(void)
    {
    }

    void Push(const T & element)
    {
        XSDK::XGuard lock(_condLock);
        _queue.push_back(element);
        _waiting = false;
        _cond.Signal();
    }

    bool NonBlockingPeek(void)
    {
        XSDK::XGuard lock(_condLock);
        return !_queue.empty();
    }

    bool BlockingPeek(void)
    {
        XSDK::XGuard lock(_condLock);
        if(_queue.empty())
        {
            _waiting = true;
            while(_waiting)
                _cond.Wait();
        }
        return !_queue.empty();
    }

    T Pop(void)
    {
        XSDK::XGuard lock(_condLock);
        if(_queue.empty())
        {
            _waiting = true;
            while(_waiting)
                _cond.Wait();
        }
        T data = _queue.front();
        _queue.pop_front();
        return data;
    }

    void Cancel(void)
    {
        XSDK::XGuard lock(_condLock);
        _waiting = false;
        _cond.Signal();
    }

private:
    QueueContainer _queue;
    XSDK::XMutex _condLock;
    XSDK::XCondition _cond;
    bool _waiting;
};

}

#endif
