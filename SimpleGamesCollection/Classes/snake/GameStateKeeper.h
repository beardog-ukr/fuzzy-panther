#pragma once

#include <list>
//#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace snake {


enum DirectionType : int;
class SnakeElementInfo;

enum GameFieldSize {
  kSnakeGameWidth = 24,
  kSnakeGameHeight = 14
};

enum MoveType : int {
  kMoveNorth,
  kMoveEast,
  kMoveSouth,
  kMoveWest,
};

enum MoveResultType : int {
  kGameFailure,
  kIdleConsume,
  kSimpleMove,
};

class GameStateKeeper : virtual public SixCatsLoggerLoggable {
public:
  GameStateKeeper();
  virtual ~GameStateKeeper();

  MoveResultType performNextMove();

  std::list<SnakeElementInfo> prepareSnakeInfo() const;

  SnakeElementInfo getBarrelInfo() const;

  void setNextMove(const MoveType move);

  void reset();

protected:
  std::list<SnakeElementInfo> elements;

  MoveResultType currentMoveResult;
  void performIdleConsume(const SnakeElementInfo& newHeadInfo);
  void performSimpleMove(const SnakeElementInfo& newHeadInfo);

  MoveType nextMove;

  int barrelX;
  int barrelY;

  int debugGameoverCountdown;

  void putBarrelToRandom();
};

}

