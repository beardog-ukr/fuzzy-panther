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
  virtual bool init();

  // implement the "static create()" method manually
  CREATE_FUNC(AsteroidsScene);

protected:
  AsteroidsScene();
  virtual ~AsteroidsScene();

  bool initBackground();
  bool initGameNode();
  bool initKeyboardProcessing();

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

  std::unique_ptr<asteroids::GameManager> gameManager;
};

}

