#pragma once

#include <memory>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class GameManager;
// #include "GameManager.h"

class AsteroidsScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  static cocos2d::Scene* createScene();
  virtual bool init();

  // implement the "static create()" method manually
  CREATE_FUNC(AsteroidsScene);

protected:
  AsteroidsScene();
  virtual ~AsteroidsScene();

  // void addSplashAt(const cocos2d::Vec2& pointA, const cocos2d::Vec2& pointB);

  bool initBackground();
  bool initGameNode();
  bool initKeyboardProcessing();
  bool initSpriteCache();
  // bool initDebugAsteroids();

  bool onContactBegin(cocos2d::PhysicsContact& contact);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

  std::unique_ptr<asteroids::GameManager> gameManager;
};

}

