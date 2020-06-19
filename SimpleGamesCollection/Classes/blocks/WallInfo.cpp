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
      newParentNode->addChild(brick.node, kBricksZOrder);
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

int WallInfo::getHeight() const {
  int max = 0;

  for(const BrickInfo& brick: bricks) {
    if (brick.gameY > max) {
      max = brick.gameY;
    }
  }

  return max;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

float WallInfo::removeFullRows() {
  const int rowCount = 18;
  const int columnCount = 10; //really 10

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

  if (acc==0) { //then nothing to delete, return
    return 0;
  }

  // debug output for rowRemovalGuide
  c6->t([rowCount, rowRemovalGuide]() -> std::string {
    ostringstream ss;
    for (int i = 0; i<rowCount; i++) {
      ss << "Removal guide #" << i << " is " << rowRemovalGuide[i];
    }
    return ss.str();
  });


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
