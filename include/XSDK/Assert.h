//=============================================================================
// Copyright (c) 2010 Pelco. All rights reserved.
//
// This file contains trade secrets of Pelco.  No part may be reproduced or
// transmitted in any form by any means or for any purpose without the express
// written permission of Pelco.
//
// $File:$
// $Revision:$
// $Date:$
// $Author:$
//=============================================================================
#ifndef _XSDK_ASSERT_H_
#define _XSDK_ASSERT_H_

//==============================================================================
// Defines the runtime macros X_ASSERT and X_VERIFY.
//==============================================================================
#ifndef IS_KERNEL
    #include <assert.h>


    //=========================================================================
    /// @brief Halts execution in debug builds if the expression evaluates to
    ///        false.
    ///
    /// Example usage:
    ///
    /// @code
    /// void MyMethod(int* value)
    /// {
    ///     X_ASSERT(value != NULL);
    ///     ...
    /// }
    /// @endcode
    //=========================================================================
    #define X_ASSERT_NO_LOG   assert

    //===========================================================================
    /// @brief X_VERIFY acts similar to X_ASSERT except that the expression to
    ///        verify remains in the code in release builds.
    //===========================================================================
    #ifdef IS_DEBUG
        #define X_VERIFY      X_ASSERT_NO_LOG
    #else
        #define X_VERIFY(exp) (exp)   // Release
    #endif
#else
    // Kernel code
    #define X_ASSERT_NO_LOG(exp)
    #define X_VERIFY(exp) (exp)
#endif // IS_KERNEL

#endif // _XSDK_ASSERT_H_

