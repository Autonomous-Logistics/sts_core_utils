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

#include <sts_core_utils/sts_core/sts_io/sts_io.h>


using namespace sts_core;


std::string StsIO::nodename_ = "";
bool StsIO::throttle_ = false;
std::chrono::time_point<std::chrono::system_clock> StsIO::start_ = std::chrono::system_clock::now();

/* Own printing Functions (only print on Screen */
void  StsIO::PRINT_INFO(const std::string& str)
{
    StsIO::PRINT(str, ".", "\033[21;37m");
}

void StsIO::PRINT_NOTIFY(const std::string& str)
{
    StsIO::PRINT(str, "*", "\033[1;32m");
}

void StsIO::PRINT_WARNING(const std::string& str)
{
    StsIO::PRINT(str, "!", "\033[21;33m");
}
void StsIO::PRINT_ERROR(const std::string& str)
{
    StsIO::PRINT(str, "#", "\033[1;31m");
}

void StsIO::PRINT_THROTTLE_INFO(double s, const std::string& str)
{
    if(!throttle_) {StsIO::start_ = std::chrono::system_clock::now(); StsIO::throttle_ = true;}
    std::chrono::duration<double> elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-StsIO::start_);
    if(elapsed.count() >= s)
    {
        StsIO::PRINT_INFO(str);
        StsIO::throttle_ = false;
        StsIO::start_ = std::chrono::system_clock::now();
    }
}

void StsIO::PRINT(const char* str, const std::string& symbol, const std::string& encode)
{
    std::string str_(str);
    StsIO::PRINT(str_, symbol, encode);
}
void StsIO::PRINT(const std::string& str, const std::string& symbol, const std::string& encode)
{
#ifdef STS_IO_USE_ROS_TIME
    ros::Time t = ros::Time::now();
#endif
#ifdef STS_IO_USE_CHRONO_TIME
    std::chrono::time_point<std::chrono::system_clock> now =
        std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
#endif
    //color and text format
    //https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
    //https://en.wikipedia.org/wiki/ANSI_escape_code#graphics
    std::cout << encode
              << "[" << symbol << "] "
#ifdef STS_IO_USE_ROS_TIME
 #if STS_IO_TIME_TYPE == 1
              <<  t.toNSec() / 1000
 #elif STS_IO_TIME_TYPE == 2
              <<  t.toNSec() / 1000000
 #elif STS_IO_TIME_TYPE == 3
              << std::fixed << t.toSec()
 #else
              <<  t.toNSec()
 #endif
              << "'"
#endif
#ifdef STS_IO_USE_CHRONO_TIME
 #if STS_IO_TIME_TYPE == 1
           << std::chrono::duration_cast<std::chrono::microseconds>(duration).count()
 #elif STS_IO_TIME_TYPE == 2
           << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count()
 #elif STS_IO_TIME_TYPE == 3
           << std::chrono::duration_cast<std::chrono::seconds>(duration).count()
 #else
           << std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count()
 #endif
           << "'"
#endif
#ifdef STS_IO_USE_NODE_NAME
              << "@" << StsIO::nodename()
#endif
#if STS_IO_USE_MESSAGE_DELIM_TYPE == 0
              << ""
#elif STS_IO_USE_MESSAGE_DELIM_TYPE == 1
              << " -> "
#elif STS_IO_USE_MESSAGE_DELIM_TYPE == 2
              << ": "
#else
              << " "
#endif
              << str << "\033[0m\n";
}

const std::string& StsIO::nodename()
{
    return nodename_;
}

void StsIO::setNodename(const std::string& s)
{
    nodename_ = s;
}





