#pragma once

#include "cocos2d.h"

#include <memory>
#include "SixCatsLogger.h"
#include "SixCatsLoggerLoggable.h"

class BackgroundNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  static BackgroundNode* create(const cocos2d::Size& expectedSize,
                                std::shared_ptr<SixCatsLogger> c6);


protected:
  BackgroundNode(const cocos2d::Size& expectedSize);
  virtual ~BackgroundNode();

  bool init();

  cocos2d::Size expectedSize;

};