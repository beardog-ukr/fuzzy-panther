#pragma once

#include <memory>
#include <list>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class BrickInfo;
enum MoveType : int;

class WallInfo;

class FigureInfo final : virtual public SixCatsLoggerLoggable {
public:
  FigureInfo();
  ~FigureInfo();

//  void setBaseRow(const int newBaseRow);

  void addInitialBrick(const BrickInfo& initialBrick);

  void addSelfToNode(cocos2d::Node* newParentNode);

  bool canDoMove(const MoveType mt, const std::unique_ptr<WallInfo> &wall);
  float doMove(const MoveType mt);

  std::list<BrickInfo> getBricks();

protected:
  cocos2d::Node* createNode(const BrickInfo& brickInfo);

  bool verifyBricksSides(const std::list<BrickInfo>& movedBricks);
  std::list<BrickInfo> prepareMovedBricks(const MoveType mt);

  std::list<BrickInfo> bricks;
};

class WallInfo final : virtual public SixCatsLoggerLoggable {
public:
  WallInfo();
  ~WallInfo();

  bool intersects(const std::list<BrickInfo>& anotherBricks);

  float removeFullRows();
  void consume(const std::list<BrickInfo>& anotherBricks);

  void addSelfToNode(cocos2d::Node* newParentNode);
  void addInitialBrick(const BrickInfo& initialBrick);

protected:
  cocos2d::Node* createNode(const BrickInfo& brickInfo);

  std::list<BrickInfo> bricks;
};


}
