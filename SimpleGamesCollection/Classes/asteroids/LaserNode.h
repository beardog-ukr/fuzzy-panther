#pragma once

#include "cocos2d.h"

#include "SixCatsLoggerLoggable.h"

namespace asteroids {

class LaserNode : public cocos2d::Node,
  virtual public SixCatsLoggerLoggable {
public:
  static LaserNode* create(const double angle, std::shared_ptr<SixCatsLogger> c6);

  void start();

protected:
  LaserNode(const double angle);
  virtual ~LaserNode();

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

  double basicAngle;

  cocos2d::Vec2 pointA;

  double upMargin;
  double downMargin;
  double leftMargin;
  double rightMargin;

  cocos2d::Sprite* baseSprite;
};

}