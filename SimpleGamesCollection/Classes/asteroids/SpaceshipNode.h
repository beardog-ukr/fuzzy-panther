#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class SpaceshipNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  static SpaceshipNode* create(std::shared_ptr<SixCatsLogger> c6);

  void setAngle(const double angle);

  void rotateClockwice();
  void rotateCounterClockwice();

protected:
  SpaceshipNode();
  virtual ~SpaceshipNode();

  void doRotate(const int value);

  bool init();

  cocos2d::Sprite* baseSprite;
};

}