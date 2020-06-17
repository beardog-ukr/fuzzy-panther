#include "blocks/BlocksGameNode.h"
#include "blocks/CommonUtils.h"
#include "blocks/BrickInfo.h"
#include "blocks/FigureFactory.h"
#include "blocks/FigureInfo.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace blocks;

static const float deletionDuration = 1.0;

//static const string mapImagesPlistFN = "bird/maps.plist";

//static const int tileSize = 64;
//static const int gameColumnsCount = 10;


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlocksGameNode::BlocksGameNode() {
  figure = nullptr;
  nextMoveType = MT_Down;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlocksGameNode::~BlocksGameNode() {
  delete figure;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlocksGameNode* BlocksGameNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  BlocksGameNode *result = new (std::nothrow) BlocksGameNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }

  if (!result->initSelf()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksGameNode::debugInitWall() {
  const int bricksCount = 6;
  BrickInfo debugBricks[bricksCount] =  {
    { .gameX = 0, .gameY = 0, .node = nullptr},
    { .gameX = 1, .gameY = 0, .node = nullptr},
//    { .gameX = 2, .gameY = 0, .node = nullptr},
    { .gameX = 3, .gameY = 0, .node = nullptr},

    { .gameX = 0, .gameY = 1, .node = nullptr},
//    { .gameX = 1, .gameY = 1, .node = nullptr},
    { .gameX = 2, .gameY = 1, .node = nullptr},
    { .gameX = 3, .gameY = 1, .node = nullptr},

//    { .gameX = 0, .gameY = 2, .node = nullptr},
//    { .gameX = 1, .gameY = 2, .node = nullptr},
//    { .gameX = 2, .gameY = 2, .node = nullptr},
//    { .gameX = 3, .gameY = 2, .node = nullptr},

//    { .gameX = 3, .gameY = 3, .node = nullptr},
//    { .gameX = 2, .gameY = 3, .node = nullptr}
  };

  const int initialColumn = 3;

  for (int i = 0; i<bricksCount; i++) {
    wall->addInitialBrick(debugBricks[i]);
  }
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksGameNode::doRotateFigure() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksGameNode::doMoveFigureLeft() {
  nextMoveType = MT_Left;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksGameNode::doMoveFigureRight() {
  nextMoveType = MT_Right;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlocksGameNode::doDropFigure() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksGameNode::initFactory() {
  figureFactory = make_unique<FigureFactory>();
  figureFactory->setLogger(c6);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksGameNode::initSelf() {
  const string bgfn = "blocks/blocks_background.png";
  if (!initWithFile(bgfn)) {
    C6_C2(c6, "Failed to init with file ", bgfn);
    return false;    //
  }

  if (!initFactory()) {
    return false;
  }

  if (!initWall()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlocksGameNode::initWall() {
  wall = make_unique<WallInfo>();
  wall->setLogger(c6);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float BlocksGameNode::prepareNextIteration() {
  if (figure == nullptr) {
    figure = figureFactory->composeNewFigure(10);
    figure->addSelfToNode(this);
  }

//  blocks::MoveType nextMoveType = MT_Down;

  float result = 0;

  if (figure->canDoMove(nextMoveType, wall)) {
    result = figure->doMove(nextMoveType);
  }
  else {
    wall->consume(figure->getBricks());
    delete figure;
    figure = nullptr;

    result = wall->removeFullRows();
  }

  nextMoveType = MT_Down;

  //debug stub:
//  float result = 0;
//  debugInitWall();
//  wall->addSelfToNode(this);
//  float tmpf = wall->removeFullRows();


  //TODO: check if wall has full rows,
  // and remove that rows here ???

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//bool MapSectionNode::loadCached() {
//  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

//  sfc->addSpriteFramesWithFile(mapImagesPlistFN);
//  if (!sfc->isSpriteFramesWithFileLoaded(mapImagesPlistFN)) {
//    return false;
//  }
//  //
//  return true;
//}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//void MapSectionNode::unloadCached() {
//  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(mapImagesPlistFN);
//}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

