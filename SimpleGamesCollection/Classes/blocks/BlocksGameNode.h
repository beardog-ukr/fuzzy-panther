#pragma once

#include <memory>
#include <vector>
//#include <utility>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class FigureInfo;
class FigureFactory;
enum MoveType : int;
class WallInfo;

class BlocksGameNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static BlocksGameNode* create(std::shared_ptr<SixCatsLogger> c6);

  static bool loadCached();
  static void unloadCached();

  float prepareNextIteration();
  bool gameIsOver() const;

  void doRotateFigure();
  void doMoveFigureLeft();
  void doMoveFigureRight();
  void doDropFigure();

protected:
  BlocksGameNode();
  virtual ~BlocksGameNode();

  bool initSelf();
  bool initFactory();
  bool initWall();

  void debugInitWall();

  MoveType nextMoveType;

  void startGame();

  FigureInfo* figure;
  std::unique_ptr<WallInfo> wall;

  bool gameOver;

  std::unique_ptr<FigureFactory> figureFactory;
};

}
