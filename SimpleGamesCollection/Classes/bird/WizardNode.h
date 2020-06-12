#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace bird {

class WizardNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static WizardNode* create(std::shared_ptr<SixCatsLogger> c6);

  void doGoUp();
  void doDie(cocos2d::CallFunc *cf );
  void setLowPoint(const cocos2d::Vec2& lp);

  void start();

protected:
  WizardNode();
  virtual ~WizardNode();

  void doGoDown();

  virtual bool init();
  bool initPhysicsBody();

  cocos2d::ActionInterval* prepareUpAction() const;
  cocos2d::ActionInterval* prepareDownAction() const;


  cocos2d::Vec2 lowPoint;
};

}