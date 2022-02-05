#pragma once

#ifdef WIN32

#ifdef PNG_CORE_EXPORT
#define PNG_CORE_API __declspec(dllexport)
#else
#define PNG_CORE_API __declspec(dllimport)
#endif  // PNG_CORE_EXPORT

#else

#ifdef PNG_CORE_EXPORT
#define PNG_CORE_API __attribute__((visibility("default")))
#else
#define PNG_CORE_API
#endif  // PNG_CORE_EXPORT

#endif // WIN32
