#pragma once

#include "cocos2d.h"

// #include <memory>
// #include "SixCatsLogger.h"
#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class AsteroidNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  static AsteroidNode* create(std::shared_ptr<SixCatsLogger> c6);

  void setAngle(const double angle);
  void start();

protected:
  AsteroidNode();
  virtual ~AsteroidNode();

  bool init();

  void goToAnotherPoint();

  cocos2d::Vec2 calculatePointA(const cocos2d::Vec2& currentPos, const double rotationAngle) const;
  cocos2d::Vec2 calculatePointAupright(const cocos2d::Vec2& currentPos,
                                       const double rotationAngle) const;
  cocos2d::Vec2 calculatePointAdownright(const cocos2d::Vec2& currentPos,
                                         const double rotationAngle) const;
  cocos2d::Vec2 calculatePointAdownleft(const cocos2d::Vec2& currentPos,
                                        const double rotationAngle) const;
  cocos2d::Vec2 calculatePointAupleft(const cocos2d::Vec2& currentPos,
                                      const double rotationAngle) const;

  cocos2d::Vec2 calculatePointB(const cocos2d::Vec2& currentPos, const double rotationAngle) const;

  //double angle;
  cocos2d::Vec2 pointA;
  cocos2d::Vec2 pointB;
  bool goToPointA;
  // cocos2d::Vec2 nextPoint;

  double upMargin;
  double downMargin;
  double leftMargin;
  double rightMargin;

  cocos2d::Sprite* baseSprite;
};

}