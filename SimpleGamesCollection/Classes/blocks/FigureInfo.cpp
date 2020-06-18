#include "blocks/FigureInfo.h"
#include "blocks/BrickInfo.h"
#include "blocks/CommonUtils.h"
#include "blocks/WallInfo.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace blocks;

static const float iterationDuration = 0.7;
static const float deletionDuration = 3.0;

//static const string mapImagesPlistFN = "bird/maps.plist";

static const int tileSize = 64;
static const int gameColumnsCount = 10;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureInfo::FigureInfo() {
  rotationIndex = 0;

  accumulatedXdiff = 0;
  accumulatedYdiff = 0;

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureInfo::~FigureInfo() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureInfo::accumulateMoveDown() {

  accumulatedYdiff = accumulatedYdiff -1;

  for (BrickInfo& brick: bricks) {
//    brick.gameX = brick.gameX + xDiff;
    brick.gameY = brick.gameY -1;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureInfo::addInitialBrick(int rotationIndex, const BrickInfo& initialBrick) {
  if ((rotationIndex>=0)&&(rotationIndex<rotationsAmount)) {
    rotationTemplates[rotationIndex].push_back((initialBrick));
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureInfo::addSelfToNode(cocos2d::Node* newParentNode) {
  parentNode = newParentNode;

  if (bricks.empty()) {
    C6_D1(c6, "Need To create bricks first");
    for(BrickInfo templateBrick: rotationTemplates[0]) {
      bricks.push_back(templateBrick);
    }
  }

  for(BrickInfo& brick : bricks) {
    if (brick.node == nullptr) {
      C6_T1(c6, "Trying to create node");
      brick.node = createNode(brick);
    }
  }

  for(const BrickInfo brick : bricks) {
    if (brick.node != nullptr) {
      C6_T4(c6, "Adding node at ", brick.node->getPosition().x, ":", brick.node->getPosition().y);
      newParentNode->addChild(brick.node, kBricksZOrder);
    }
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FigureInfo::canDoMove(const MoveType mt, const std::unique_ptr<WallInfo>& wall) {
  if (mt == MT_Rotate) {
    return false;
  }

  list<BrickInfo> movedBricks = prepareMovedBricks(mt);

  if (!verifyBricksSides(movedBricks)) {
    return false;
  }

  if (wall->intersects(movedBricks)) {
    return false;
  }

  //else
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FigureInfo::canDoRotate(const std::unique_ptr<WallInfo> &wall) {
  int newRotationIndex = rotationIndex +1;
  if (newRotationIndex >= rotationsAmount) {
    newRotationIndex = 0;
  }

  if (newRotationIndex == rotationIndex) {
    C6_D2(c6, "can't rotate, same index ", rotationIndex);
    return false;
  }

  list<BrickInfo> rotatedBricks = prepareRotatedBricks(newRotationIndex);

  if (!verifyBricksSides(rotatedBricks)) {
    C6_D1(c6, "can't rotate, sides ");
    return false;
  }

  if (wall->intersects(rotatedBricks)) {
    C6_D1(c6, "can't rotate, wall ");
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* FigureInfo::createNode(const BrickInfo& brickInfo) {
  const string fn = "blocks/common_tile.png";
  Sprite* sprite = Sprite::create(fn);
  if(sprite == nullptr) {
    C6_D2(c6, "Failed to find ", fn);
    return nullptr;
  }

//  const int startingColumn = 3;

  sprite->setAnchorPoint(Vec2(0,0));
  sprite->setPosition( brickInfo.gameX*tileSize, brickInfo.gameY*tileSize);

  C6_D1(c6, "node created ");
  return sprite;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float FigureInfo::doMove(const MoveType mt) {
  if (mt == MT_Rotate) {
    return doRotate();
  }

  int xDiff = 0;
  int yDiff = 0;
  switch(mt) {
  case MT_Down:
    yDiff = -1;
    break;
  case MT_Left:
    xDiff = -1;
    break;
  case MT_Right:
    xDiff = 1;
    break;
  default:
    C6_D2(c6, "Bad logic ", (int)mt);
    return 0;
  }

  accumulatedXdiff += xDiff;
  accumulatedYdiff += yDiff;

  for (BrickInfo& brick: bricks) {
    brick.gameX = brick.gameX + xDiff;
    brick.gameY = brick.gameY + yDiff;

    Vec2 newPos;//
    newPos.x = brick.gameX*tileSize;
    newPos.y = brick.gameY*tileSize;
    MoveTo* mta = MoveTo::create(iterationDuration, newPos);
    brick.node->runAction(mta);
  }

  return iterationDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float FigureInfo::doRotate() {
  C6_D1(c6, "here");

  for(BrickInfo& brick: bricks) {
    if (brick.node != nullptr) {
      FadeOut* fout = FadeOut::create(iterationDuration/2);
      Sequence* seq = Sequence::create(fout, RemoveSelf::create(), nullptr );
      brick.node->runAction(seq);
    }
  }

  bricks.clear();

  int newRotationIndex = rotationIndex +1;
  if (newRotationIndex >= rotationsAmount) {
    newRotationIndex = 0;
  }

  bricks = prepareRotatedBricks(newRotationIndex);

  rotationIndex = newRotationIndex;

  addSelfToNode(parentNode);

  for(BrickInfo& brick: bricks) {
    if (brick.node != nullptr) {
      brick.node->setOpacity(0);
      FadeIn* fin = FadeIn::create(iterationDuration/2);
      brick.node->runAction(fin);
    }
  }

  return iterationDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

list<BrickInfo> FigureInfo::getBricks() {
  return bricks;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

std::list<BrickInfo> FigureInfo::prepareMovedBricks(const MoveType mt) {
  int xDiff = 0;
  int yDiff = 0;
  switch(mt) {
  case MT_Down:
    yDiff = -1;
    break;
  case MT_Left:
    xDiff = -1;
    break;
  case MT_Right:
    xDiff = 1;
    break;
  default:
    C6_D2(c6, "Bad logic ", (int)mt);
  }

  list<BrickInfo> result;

  for (const BrickInfo& brick: bricks) {
    BrickInfo movedBrick;
    movedBrick.node = brick.node;
    movedBrick.gameX = brick.gameX + xDiff;
    movedBrick.gameY = brick.gameY + yDiff;

    result.push_back(movedBrick);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

list<BrickInfo> FigureInfo::prepareRotatedBricks(const int newRotationIndex) {
  list<BrickInfo> result;

  for(const BrickInfo& templateBrick: rotationTemplates[newRotationIndex]) {
    BrickInfo rotatedBrick;
    rotatedBrick.node = nullptr;
    rotatedBrick.gameX = templateBrick.gameX + accumulatedXdiff;
    rotatedBrick.gameY = templateBrick.gameY + accumulatedYdiff;

    result.push_back(rotatedBrick);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureInfo::setRotationsAmount(const int newRotationsAmount) {
  rotationsAmount = newRotationsAmount;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FigureInfo::verifyBricksSides(const std::list<BrickInfo>& movedBricks) {
  for (const BrickInfo& brick: movedBricks) {
    if ((brick.gameX <0) || (brick.gameX>=gameColumnsCount) || (brick.gameY <0) ) {
      return false;
    }
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float FigureInfo::useAccumulatedMoveDown() {
  for (BrickInfo& brick: bricks) {

    Vec2 newPos;
    newPos.x = brick.gameX*tileSize;
    newPos.y = brick.gameY*tileSize;
    MoveTo* mta = MoveTo::create(iterationDuration, newPos);
    brick.node->runAction(mta);
  }

  return iterationDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

