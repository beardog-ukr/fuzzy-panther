#pragma once

#include "cocos2d.h"

#include <memory>
#include "SixCatsLogger.h"
#include "SixCatsLoggerLoggable.h"

class AsteroidsGameNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  static AsteroidsGameNode* create(std::shared_ptr<SixCatsLogger> c6);


protected:
  AsteroidsGameNode();
  virtual ~AsteroidsGameNode();

  bool init();
};