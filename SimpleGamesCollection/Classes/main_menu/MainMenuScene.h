#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

class MainMenuScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  virtual bool init();

  // a selector callback
  void menuCloseCallback(cocos2d::Ref* pSender);

  // implement the "static create()" method manually
  CREATE_FUNC(MainMenuScene);

protected:
  bool initBackground();
  bool initCloseButton();

};


