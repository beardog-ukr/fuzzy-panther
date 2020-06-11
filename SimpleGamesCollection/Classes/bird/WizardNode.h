#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace bird {

class WizardNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static WizardNode* create(std::shared_ptr<SixCatsLogger> c6);

protected:
  WizardNode();
  virtual ~WizardNode();

  virtual bool init();
  bool initPhysicsBody();
};

}