#pragma once

// if auto-detection does not work it can be
// commented out and the required
// macros can be just defined manually like this:

/*#ifndef PLAYGROUND_PLATFORM_IOS
#define PLAYGROUND_PLATFORM_IOS
#endif*/

#if defined(__APPLE__) // Apple OSX and iOS (Darwin)
    #include <TargetConditionals.h>

    #if TARGET_OS_MAC == 1 // still both OSX or iOS

        #if TARGET_OS_IOS // iOS found
            #define PLAYGROUND_PLATFORM_IOS // Apple iOS

        #elif TARGET_OS_SIMULATOR // simulator found
            #define PLAYGROUND_PLATFORM_IOS // Apple iOS

        #elif TARGET_OS_OSX
            #define PLAYGROUND_PLATFORM_OSX // Apple OSX
        #else
            // define nothing
        #endif

    #endif // TARGET_OS_MAC
#endif // __APPLE__

