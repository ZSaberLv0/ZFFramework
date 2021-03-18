/**
 * @file zfstl_string.h
 * @brief stl wrapper, note it's internal use only
 */

#ifndef _ZFI_zfstl_string_h_
#define _ZFI_zfstl_string_h_

#include "zfstl_define.h"
#include <string>

// ============================================================
/** @brief stl wrapper */
#ifndef zfstlstring
    #define zfstlstring std::string
#endif

/** @brief stl wrapper */
#ifndef zfstlbasicstring
    #define zfstlbasicstring std::basic_string
#endif

// ============================================================
/** @brief zfchar version of stl string */
#ifndef zfstlstringZ
    #define zfstlstringZ zfstlbasicstring<zfchar>
#endif

#endif // #ifndef _ZFI_zfstl_string_h_

