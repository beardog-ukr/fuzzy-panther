#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace snake {

class GameNode;

class SnakeMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  SnakeMainScene();
  virtual ~SnakeMainScene();

  bool init();
  bool initBackground();
  bool initBackgroundBorder();
  bool initKeyboardProcessing();
  bool initGameNode();
  bool initGameOverMenu();
  bool initGameOverImage();

  cocos2d::Menu* gameOverMenu;
  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  GameNode* gameNode;

  void processGameOver(const bool gameEndedVictory);

  cocos2d::Sprite* lastGameOverImage;
  cocos2d::Sprite* failureImage;
  cocos2d::Sprite* victoryImage;

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

