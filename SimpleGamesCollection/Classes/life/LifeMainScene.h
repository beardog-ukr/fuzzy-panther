#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace life {

class GameNode;

class LifeMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  LifeMainScene();
  virtual ~LifeMainScene();

  bool init();
  bool initKeyboardProcessing();
  bool initGameNode();
  bool initMouseProcessing();

  void moveCamera(const int xDiff, const int yDiff);

  GameNode* gameNode;
  cocos2d::Size mapSize;

  bool cameraIsMoving;
  int mouseButtonPressed;
  cocos2d::EventMouse::MouseButton currentActiveMouseButton;
  void onMouseDown(cocos2d::Event *event);
  void onMouseUp(cocos2d::Event *event);
  void onMouseMove(cocos2d::Event *event);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
};

}

