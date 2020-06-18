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

  void addInitialBrick(const int rotationIndex, const BrickInfo& initialBrick);
  void setRotationsAmount(const int rotationsAmount);

  void addSelfToNode(cocos2d::Node* newParentNode);

  bool canDoMove(const MoveType mt, const std::unique_ptr<WallInfo> &wall);
  float doMove(const MoveType mt);

  bool canDoRotate(const std::unique_ptr<WallInfo> &wall);
  float doRotate();

  void accumulateMoveDown();
  float useAccumulatedMoveDown();

  std::list<BrickInfo> getBricks();

protected:
  cocos2d::Node* createNode(const BrickInfo& brickInfo);

  bool verifyBricksSides(const std::list<BrickInfo>& movedBricks);
  std::list<BrickInfo> prepareMovedBricks(const MoveType mt);
  std::list<BrickInfo> prepareRotatedBricks(const int newRotationIndex);

  std::list<BrickInfo> bricks;

  int accumulatedXdiff;
  int accumulatedYdiff;

  cocos2d::Node* parentNode;
  int rotationIndex;
  int rotationsAmount;
  std::list<BrickInfo> rotationTemplates[4];
};

}
