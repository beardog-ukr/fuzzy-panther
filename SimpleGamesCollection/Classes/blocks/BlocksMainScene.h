#pragma once

#include <memory>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class BlocksGameNode;

class BlocksMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  BlocksMainScene();
  virtual ~BlocksMainScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initFailMenu();
  bool initGameNode();

  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  void startGame();
  void doNextTick();
  BlocksGameNode* gameNode;

  cocos2d::Menu* failMenu;

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

