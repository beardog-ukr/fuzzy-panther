#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class AsteroidNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable
{
public:
  enum RockType {
    RT_small = 0,
    RT_medium,
    RT_big
  };
  static AsteroidNode* create(const double angle, const RockType type,
                              std::shared_ptr<SixCatsLogger> c6);

  void start();

protected:
  AsteroidNode(const double angle, const RockType type);
  virtual ~AsteroidNode();

  bool init();
  bool initBaseSprite();
  bool initPhysicsBody();


  void setAngle(const double angle);
  void resetMove();

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

  double basicAngle;
  RockType type;

  cocos2d::Vec2 pointA;
  cocos2d::Vec2 pointB;
  // bool goToPointA;

  double upMargin;
  double downMargin;
  double leftMargin;
  double rightMargin;

  cocos2d::Sprite* baseSprite;
};

}