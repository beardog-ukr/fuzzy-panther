#include "blocks/FigureInfo.h"
#include "blocks/BrickInfo.h"
#include "blocks/CommonUtils.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace blocks;

static const float iterationDuration = 1.0;
static const float deletionDuration = 4.0;

//static const string mapImagesPlistFN = "bird/maps.plist";

static const int tileSize = 64;
static const int gameColumnsCount = 10;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

WallInfo::WallInfo() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

WallInfo::~WallInfo() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WallInfo::consume(const std::list<BrickInfo> &anotherBricks) {
  for(const BrickInfo& anotherBrick: anotherBricks) {
    bricks.push_back(anotherBrick);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WallInfo::intersects(const std::list<BrickInfo>& anotherBricks) {
  for(const BrickInfo& anotherBrick: anotherBricks) {
    for(const BrickInfo& hereBrick: bricks) {
      if ((hereBrick.gameX == anotherBrick.gameX)&&
          (hereBrick.gameY == anotherBrick.gameY)) {
        return true;
      }
    }
  }

  return false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WallInfo::addInitialBrick(const BrickInfo& initialBrick) {
  bricks.push_back(initialBrick);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WallInfo::addSelfToNode(cocos2d::Node* newParentNode) {
  for(BrickInfo& brick : bricks) {
    if (brick.node == nullptr) {
      C6_D1(c6, "Trying to create node");
      brick.node = createNode(brick);
    }
  }

  for(const BrickInfo brick : bricks) {
    if (brick.node != nullptr) {
      C6_D4(c6, "Adding node at ", brick.node->getPosition().x, ":", brick.node->getPosition().y);
      newParentNode->addChild(brick.node, ZO_bricks);
    }
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* WallInfo::createNode(const BrickInfo& brickInfo) {
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

float WallInfo::removeFullRows() {
  const int rowCount = 18;
  const int columnCount = 4; //really 10

  int rowCounters[rowCount];
  for (int i = 0; i<rowCount; i++) {
    rowCounters[i] = 0;
  }

  for (const BrickInfo& brick: bricks) {
    rowCounters[brick.gameY]++;
  }

  int rowRemovalGuide[rowCount];
  int acc = 0;
  for (int i = 0; i<rowCount; i++) {
    if (rowCounters[i] == columnCount) {
      rowRemovalGuide[i] = -1;
      acc++;
    }
    else {
      rowRemovalGuide[i] = acc;
    }
  }

  if (acc==0) { //then nothing to delete
    return 0;
  }


  for (int i = 0; i<rowCount; i++) {
    C6_D4(c6, "Removal guide #", i, " is ", rowRemovalGuide[i]);
  }

  list<Node*> nodesToGetOut;
  auto bi = bricks.begin();
  while (bi != bricks.end()) {
    if (rowRemovalGuide[bi->gameY] == -1) {
      nodesToGetOut.push_back(bi->node);
      bi = bricks.erase(bi);
    }
    else {
      if (rowRemovalGuide[bi->gameY] > 0) {
        MoveBy* mba = MoveBy::create(deletionDuration/2,
                                     Vec2(0, -1*rowRemovalGuide[bi->gameY]*tileSize));
        Sequence* seq = Sequence::create(DelayTime::create(deletionDuration/2), mba, nullptr);
        bi->node->runAction(seq);

        bi->gameY = bi->gameY - rowRemovalGuide[bi->gameY];
      }

      bi++;
    }
  }

  for (Node* node:nodesToGetOut) {
    FadeOut*foa = FadeOut::create(deletionDuration/2);
    Sequence* seq = Sequence::create(foa, RemoveSelf::create(), nullptr);
    node->runAction(seq);
  }

  return deletionDuration;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureInfo::FigureInfo() {
//  baseRow = 0;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureInfo::~FigureInfo() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureInfo::addInitialBrick(const BrickInfo& initialBrick) {
  bricks.push_back(initialBrick);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureInfo::addSelfToNode(cocos2d::Node* newParentNode) {
  for(BrickInfo& brick : bricks) {
    if (brick.node == nullptr) {
      C6_D1(c6, "Trying to create node");
      brick.node = createNode(brick);
    }
  }

  for(const BrickInfo brick : bricks) {
    if (brick.node != nullptr) {
      C6_D4(c6, "Adding node at ", brick.node->getPosition().x, ":", brick.node->getPosition().y);
      newParentNode->addChild(brick.node, ZO_bricks);
    }
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

list<BrickInfo> FigureInfo::getBricks() {
  return bricks;
}

//bool FigureInfo::intersects(const std::list<BrickInfo>& anotherBricks) {
//  for(const BrickInfo& anotherBrick: anotherBricks) {
//    for(const BrickInfo& hereBrick: bricks) {
//      if ((hereBrick.gameX == anotherBrick.gameX)&&
//          (hereBrick.gameY == anotherBrick.gameY)) {
//        return true;
//      }
//    }
//  }

//  return false;
//}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//void FigureInfo::consume(FigureInfo* anotherFigure) {
//  for(const BrickInfo& anotherBrick: anotherFigure->bricks) {
//    bricks.push_back(anotherBrick);
//  }

//  delete anotherFigure;
//  //
//}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FigureInfo::canDoMove(const MoveType mt, const std::unique_ptr<WallInfo>& wall) {
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
    // no default section here
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

bool FigureInfo::verifyBricksSides(const std::list<BrickInfo>& movedBricks) {
  for (const BrickInfo& brick: movedBricks) {
    if ((brick.gameX <0) || (brick.gameX>=gameColumnsCount) || (brick.gameY <0) ) {
      return false;
    }
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float FigureInfo::doMove(const MoveType mt) {
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
    // no default section here
  }

  for (BrickInfo& brick: bricks) {
    brick.gameX = brick.gameX + xDiff;
    brick.gameY = brick.gameY + yDiff;

    Vec2 newPos;//= {.x = brick.gameX*tileSize, .y = brick.gameY*tileSize};
    newPos.x = brick.gameX*tileSize;
    newPos.y = brick.gameY*tileSize;
    MoveTo* mta = MoveTo::create(iterationDuration, newPos);
    brick.node->runAction(mta);
  }

  return iterationDuration;
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

