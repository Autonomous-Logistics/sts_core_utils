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

#include <sts_core_utils/sts_core/sts_base/sts_core_node.h>

using namespace sts_core::sts_base;

/// **************************
///         Constructors
/// **************************
StsCoreNode::StsCoreNode(ros::NodeHandle* nodeHandlePtr) : StsCoreNode()
{
    this->initialize(nodeHandlePtr);
}

StsCoreNode::StsCoreNode()
{
  data_ = new StsCoreNodeData();
}

/// **************************
///         Destructor
/// **************************
StsCoreNode::~StsCoreNode()
{
}

/// **************************
///         Methods
/// **************************

bool StsCoreNode::initialize(ros::NodeHandle* nodeHandlePtr)
{
    this->setNodeHandle(nodeHandlePtr);
    //grab namespace of handle by killing leading '/' of namespace
    std::string ns = this->getNodeHandle()->getNamespace();
    this->data_->nodename_ = ns.substr(1,ns.length()-1);
    this->data_->status_ = State::OK;
    //set this node for static IO class
    StsIO::setNodename(this->getNodeName());
    return true;
}

void StsCoreNode::PRINT_INFO(const std::string& str)
{
  STS_INFO("%s", str);
}

void StsCoreNode::PRINT_NOTIFY(const std::string& str)
{
  STS_NOTIFY("%s", str);
}

void StsCoreNode::PRINT_WARNING(const std::string& str)
{
  STS_WARNING("%s", str);
}

void StsCoreNode::PRINT_ERROR(const std::string& str)
{
  STS_ERROR("%s", str);
}

void StsCoreNode::setStatus(sts_core::sts_base::State s)
{
  this->data_->status_ = s;
}

void StsCoreNode::setNOK()
{
  this->setStatus(sts_core::sts_base::State::NOK);
}

void StsCoreNode::setOK()
{
  this->setStatus(sts_core::sts_base::State::OK);
}

void StsCoreNode::setNodeHandle(ros::NodeHandle* nodeHandlePtr)
{
  this->data_->nodeHandlePtr_ = nodeHandlePtr;
}

std::string StsCoreNode::getNodeName()
{
  return this->data_->nodename_;
}

bool StsCoreNode::getStatus()
{
  return this->data_->status_;
}

bool StsCoreNode::isOk()
{
  return (this->getStatus() != sts_core::sts_base::State::NOK);
}

ros::NodeHandle* StsCoreNode::getNodeHandle()
{
  return this->data_->nodeHandlePtr_;
}


/// **************************
///      StsCoreNodeData
/// **************************

StsCoreNodeData::StsCoreNodeData()
{
}



