/* Copyright (C) 2018-2020 Nick Fiege, Robert Rudolph - All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

// STD Includes
#include <string>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <memory>

// ROS
//#include <ros/ros.h>

// Others
#include <chrono>

// internal includes

/*! GLOBAL CONFIGURATION DEFINITIONS */
/*! ***********************************************/
#define STS_IO_TIME_TYPE 2                  //1 micro, 2 msec, 3 sec, else nano
//#define STS_IO_USE_ROS_TIME
#define STS_IO_USE_CHRONO_TIME
#define STS_IO_USE_NODE_NAME
#define STS_IO_USE_MESSAGE_DELIM_TYPE 1    //0 nothing, 1 arrow, 2 colon
/*! ***********************************************/

/*! LEGACY SUPPORT MACROS */
/*! ***********************************************/
#define STS_INFO            sts_core::StsIO::PRINTF_INFO
#define STS_NOTIFY          sts_core::StsIO::PRINTF_NOTIFY
#define STS_WARNING         sts_core::StsIO::PRINTF_WARNING
#define STS_ERROR           sts_core::StsIO::PRINTF_ERROR

#define STS_THROTTLE_INFO   sts_core::StsIO::PRINTF_THROTTLE_INFO
/*! ***********************************************/



//int c = std::sprintf(buf,fmt,to_c(args)...);

#if false
#define STS_BASE_NODE_PRINTF_IMPL   \
        static char buf[255];       \
        int c = std::sprintf(buf,fmt,to_c(args)...); \
        std::string str(buf);
#endif
#if true
//https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
#define STS_BASE_NODE_PRINTF_IMPL \
    std::size_t size = snprintf( nullptr, 0, fmt, to_c(args) ... ) + 1; /* Extra space for '\0' */ \
    std::unique_ptr<char[]> buf( new char[ size ] ); \
    std::snprintf( buf.get(), size, fmt, to_c(args) ... ); \
    std::string str (buf.get(), buf.get() + size -1 ); /* We don't want the '\0' inside */
#endif


#define STS_IO_PRINT_ARGS const char* fmt, Args&&... args


namespace sts_core {


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-security"
/*! Support class for the node to handle IOs */
class StsIO
{
private:
    static std::string nodename_;
    static const std::string& nodename();
public:
    static void setNodename(const std::string& s);
public:
    /*! Own printing Functions (only print on Screen) */
    static void PRINT_INFO(const std::string& str);
    static void PRINT_NOTIFY(const std::string& str);
    static void PRINT_WARNING(const std::string& str);
    static void PRINT_ERROR(const std::string& str);

    /*! Own Throttled Functions (only print on Screen) */
    static bool throttle_;
    static std::chrono::time_point<std::chrono::system_clock> start_;
    static void PRINT_THROTTLE_INFO(double s, const std::string& str);

    /*! Own printing (printf) wrapper Functions */
    // template wrapper for printf
    template<class T> static inline auto to_c(T&& arg) -> decltype(std::forward<T>(arg));
    static inline char const* to_c(std::string const& s);
    static inline char const* to_c(std::string& s);
    //...
    template<class... Args> static void PRINTF_INFO(STS_IO_PRINT_ARGS);
    template<class... Args> static void PRINTF_NOTIFY(STS_IO_PRINT_ARGS);
    template<class... Args> static void PRINTF_WARNING(STS_IO_PRINT_ARGS);
    template<class... Args> static void PRINTF_ERROR(STS_IO_PRINT_ARGS);
    template<class... Args> static void PRINTF_THROTTLE_INFO(double s, STS_IO_PRINT_ARGS);

private:
    static void PRINT(const char* str, const std::string& symbol, const std::string& encode);
    static void PRINT(const std::string& str, const std::string& symbol, const std::string& encode);


};

/// ******************************
/// Template and inline functions
/// ******************************

template<class T> inline auto StsIO::to_c(T&& arg) -> decltype(std::forward<T>(arg))
{
    return std::forward<T>(arg);
}

inline char const* StsIO::to_c(std::string const& s)
{
    return s.c_str();
}

inline char const* StsIO::to_c(std::string& s)
{
    return s.c_str();
}

template<class... Args> void StsIO::PRINTF_INFO(STS_IO_PRINT_ARGS)
{
    STS_BASE_NODE_PRINTF_IMPL
    StsIO::PRINT_INFO(str);
}

template<class... Args> void StsIO::PRINTF_NOTIFY(STS_IO_PRINT_ARGS)
{
    STS_BASE_NODE_PRINTF_IMPL
    StsIO::PRINT_NOTIFY(str);
}

template<class... Args> void StsIO::PRINTF_WARNING(STS_IO_PRINT_ARGS)
{
    STS_BASE_NODE_PRINTF_IMPL
    StsIO::PRINT_WARNING(str);
}

template<class... Args> void StsIO::PRINTF_ERROR(STS_IO_PRINT_ARGS)
{
    STS_BASE_NODE_PRINTF_IMPL
    StsIO::PRINT_ERROR(str);
}

template<class... Args> void StsIO::PRINTF_THROTTLE_INFO(double s, STS_IO_PRINT_ARGS)
{
    if(!throttle_)
    {
        StsIO::start_ = std::chrono::system_clock::now();
        StsIO::throttle_ = true;
    }
    std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-StsIO::start_);
    if(elapsed.count() >= s)
    {
        StsIO::PRINTF_INFO(fmt, to_c(args)...);
        StsIO::throttle_ = false;
        StsIO::start_ = std::chrono::system_clock::now();
    }
}

#pragma GCC diagnostic pop


} /* namespace */
