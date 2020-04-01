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

// ROS
#include <ros/ros.h>

// Others

// internal includes
#include <sts_core_utils/sts_core/sts_io/sts_io.h>

namespace sts_core {
namespace sts_base {

enum State { NOK = 0, OK = 1 };


class StsCoreNodeData
{
public:
    StsCoreNodeData();
    //! ROS node started fine.
    State status_ = State::NOK;
    //! ROS node name.
    std::string nodename_;
    //! ROS node handle.
    ros::NodeHandle* nodeHandlePtr_;
};

/*! Main class for the node to handle the ROS interfacing. */
class StsCoreNode
{
    /* ***************************************************************************************************
     *                                      S T A T I C S
     * **************************************************************************************************/

    /* ***************************************************************************************************
     *                           L E G A C Y    S U P P O R T
     * **************************************************************************************************/
public:
    void PRINT_INFO(const std::string& str);
    void PRINT_NOTIFY(const std::string& str);
    void PRINT_WARNING(const std::string& str);
    void PRINT_ERROR(const std::string& str);

    /* ***************************************************************************************************
     *                              C O R E          M E T H O D S
     * **************************************************************************************************/
public:
    /*! Constructor.
    * @param nodeHandle the ROS node handle. */
    StsCoreNode();
    StsCoreNode(ros::NodeHandle* nodeHandlePtr);

    /*! Destructor. */
    virtual ~StsCoreNode();

public:
protected:
    bool initialize(ros::NodeHandle* nodeHandlePtr);

    /* ***************************************************************************************************
     *                           S E T T E R          M E T H O D S
     * **************************************************************************************************/
public:
    void setStatus(sts_core::sts_base::State s);
    void setNOK();
    void setOK();

protected:
    void setNodeHandle(ros::NodeHandle* nodeHandlePtr);

    /* ***************************************************************************************************
     *                           G E T T E R          M E T H O D S
     * **************************************************************************************************/
public:
    std::string getNodeName();
    bool getStatus();
    bool isOk();
    ros::NodeHandle* getNodeHandle();

    /* ***************************************************************************************************
     *                                  A T T R I B U T E S
     * **************************************************************************************************/
protected:
    StsCoreNodeData* data_;
};


} /* namespace */
} /* namespace */



