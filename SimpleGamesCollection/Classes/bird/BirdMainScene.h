#pragma once

#include <memory>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace bird {

class BirdMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  BirdMainScene();
  virtual ~BirdMainScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initModules();
  bool initWizard();

  cocos2d::Vec2 pointDelete;
  void addNewModule();

  // void mcBackToMain(cocos2d::Ref *pSender);
  // void mcTryAgain(cocos2d::Ref *pSender);

  // bool onContactBegin(cocos2d::PhysicsContact& contact);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

