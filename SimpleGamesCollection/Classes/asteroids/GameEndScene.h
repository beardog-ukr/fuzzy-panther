#pragma once

#include <memory>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class GameEndScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  static cocos2d::Scene* createScene(const bool victory, std::shared_ptr<SixCatsLogger> c6);


  // implement the "static create()" method manually
  // CREATE_FUNC(GameEndScene);

protected:
  GameEndScene(const bool victory);
  virtual ~GameEndScene();

  // void addSplashAt(const cocos2d::Vec2& pointA, const cocos2d::Vec2& pointB);

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initMenu();
  bool initMenuBackground();

  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  // bool onContactBegin(cocos2d::PhysicsContact& contact);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

  bool victory;
};

}

