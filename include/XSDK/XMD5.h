//=============================================================================
//Copyright (c) 2014 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File: XMD5.h $
// $Revision:$
// $Date:$
// $Author: Tony Di Croce $
//=============================================================================

#ifndef XMD5_h
#define XMD5_h

#include "XSDK/Types.h"
#include "XSDK/XString.h"

namespace XSDK
{

class XMD5
{
public:
    XMD5();
    virtual ~XMD5() throw();

    void Update( const uint8_t* src, size_t size );

    void Finalize();

    void Get( uint8_t* output ); // returns 16 byte MD5
    XString GetAsString();

private:
    uint8_t* _body( const uint8_t* data, size_t size );

    uint32_t _lo, _hi;
    uint32_t _a, _b, _c, _d;
    unsigned char _buffer[64];
    uint32_t _block[16];
    bool _finalized;
    uint8_t _result[16];
};

}

#endif
