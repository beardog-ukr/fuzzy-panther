#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace flowers {

class FlowersGameNode;

class FlowersMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  FlowersMainScene();
  virtual ~FlowersMainScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initGameNode();
  bool initGameOverMenu();
  bool initGameOverImage();
  bool initMouseProcessing();

  FlowersGameNode* gameNode;
  float gnNorthBorder;
  float gnSouthBorder;
  float gnEastBorder;
  float gnWestBorder;
//  float gnSize;

  cocos2d::Menu* gameOverMenu;
  void mcBackToMain(cocos2d::Ref *pSender);
  void mcTryAgain(cocos2d::Ref *pSender);

  bool gameOver;

  cocos2d::Sprite* lastGameOverImage;
  cocos2d::Sprite* failureImage;
  cocos2d::Sprite* victoryImage;

  void onMouseDown(cocos2d::Event *event);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

