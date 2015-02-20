
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//
// XSDK
// Copyright (c) 2015 Schneider Electric
//
// Use, modification, and distribution is subject to the Boost Software License,
// Version 1.0 (See accompanying file LICENSE).
//
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include "XSDK/XStackStopWatch.h"

using namespace XSDK;

XStackStopWatch::XStackStopWatch(XString message, XDuration minTimeElapsed, LOG_LEVEL level) :
_stopWatch(true),
_message(message),
_minTimeElapsed(minTimeElapsed),
_level(level)
{
}

XStackStopWatch::~XStackStopWatch() throw()
{
    XDuration timeElapsed = _stopWatch.TimeElapsed();
    if ( timeElapsed < _minTimeElapsed )
        return;
    try
    {//Not do or do not there is no try.....
        switch ( _level )
        {
        case LOGLEVEL_CRITICAL:
            X_LOG_CRITICAL("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        case LOGLEVEL_ERROR:
            X_LOG_ERROR("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        case LOGLEVEL_WARNING:
            X_LOG_WARNING("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        case LOGLEVEL_NOTICE:
            X_LOG_NOTICE("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        case LOGLEVEL_INFO:
            X_LOG_INFO("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        case LOGLEVEL_TRACE:
            X_LOG_TRACE("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        case LOGLEVEL_DEBUG:
        default:
            X_LOG_DEBUG("%s, elapsed time[%s]",_message.c_str(),timeElapsed.ToString().c_str());
            break;
        }
    }catch(std::exception&)
    {
    }
}

