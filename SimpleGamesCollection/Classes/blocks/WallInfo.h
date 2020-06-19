#pragma once

#include <list>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class BrickInfo;

class WallInfo final : virtual public SixCatsLoggerLoggable {
public:
  WallInfo();
  ~WallInfo();

  bool intersects(const std::list<BrickInfo>& anotherBricks);

  float removeFullRows();
  void consume(const std::list<BrickInfo>& anotherBricks);

  void addSelfToNode(cocos2d::Node* newParentNode);
  void addInitialBrick(const BrickInfo& initialBrick);

  int getHeight() const;

protected:
  cocos2d::Node* createNode(const BrickInfo& brickInfo);

  std::list<BrickInfo> bricks;
};

}
