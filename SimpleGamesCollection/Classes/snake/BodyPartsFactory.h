#pragma once

#include "cocos2d.h"

#include <vector>
#include "SixCatsLoggerLoggable.h"

namespace snake {

enum DirectionType : int;

class SnakeElementInfo {
public:
  int previousX;
  int previousY;
  int gameX;
  int gameY;
};

class BodyPartsFactory : virtual public SixCatsLoggerLoggable {
public:
  BodyPartsFactory();
  virtual ~BodyPartsFactory();

  cocos2d::Sprite* createMovingPart(const SnakeElementInfo& info);
  cocos2d::Sprite* createIdlePart(const SnakeElementInfo& info);

  cocos2d::Sprite* createBarrel(const int x, const int y) const;
  cocos2d::Sprite* createBarrelExplode(const int x, const int y) const;

//  cocos2d::Sprite* addBodyPart(const SnakeElementInfo& info, cocos2d::Node* parent,
//                               const int zOrder);
protected:

//  std::string getAnimationNameForType(const DirectionType type) const;
};

}
