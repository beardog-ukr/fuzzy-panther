#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace sokoban {

class GameNode;

class SokobanMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  SokobanMainScene();
  virtual ~SokobanMainScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initGameNode();
  bool initGameOverMenu();
  bool initGameOverImage();

  void processGameOver(const bool gameResult);

  cocos2d::Menu* gameOverMenu;
  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  GameNode* gameNode;

  bool gameOver;

  cocos2d::Sprite* lastGameOverImage;
  cocos2d::Sprite* failureImage;
  cocos2d::Sprite* victoryImage;

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

