#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace fifteen {

class FifteenGameNode;

class FifteenMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  FifteenMainScene();
  virtual ~FifteenMainScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initGameNode();
  bool initGameOverMenu();
  bool initGameOverImage();
  bool initMouseProcessing();

  FifteenGameNode* gameNode;
  float gnNorthBorder;
  float gnSouthBorder;
  float gnEastBorder;
  float gnWestBorder;
  float gnSize;

  cocos2d::Menu* gameOverMenu;
  cocos2d::Sprite* victoryImage;
  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  bool gameSolved;

  void onMouseDown(cocos2d::Event *event);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

