#pragma once

#include <memory>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace eyes {

class EyesMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  EyesMainScene();
  virtual ~EyesMainScene();

  bool init();
  bool initBackground();
  bool initKeyboardProcessing();
  bool initMouseProcessing();
  bool initSpaceShips();

  cocos2d::Sprite* leftShip;
  cocos2d::Vec2 leftShipPos;
  cocos2d::Sprite* rightShip;
  cocos2d::Vec2 rightShipPos;

  double calculateAngle(const float xPos, const float yPos,
                        const cocos2d::Vec2& shipPos) const;

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);
  void onMouseMove(cocos2d::Event *event);
};

}

