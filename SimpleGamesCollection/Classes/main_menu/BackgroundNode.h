#pragma once

#include "cocos2d.h"

#include <memory>
#include "SixCatsLogger.h"
#include "SixCatsLoggerLoggable.h"

class BackgroundNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  static BackgroundNode* create(const cocos2d::Size& expectedSize, const std::string& filename,
                                std::shared_ptr<SixCatsLogger> c6);


protected:
  BackgroundNode();
  virtual ~BackgroundNode();

  bool init(const cocos2d::Size& expectedSize, const std::string& filename);
};