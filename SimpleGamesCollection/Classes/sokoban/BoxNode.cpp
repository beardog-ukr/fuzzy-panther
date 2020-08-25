#include "sokoban/BoxNode.h"
using namespace sokoban;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

const int kTileSize = 48;

static const struct {
  string north;
  string south;
  string east;
  string west;
} kDefaultFrameNames = {
  .north = "sokoban/beetle/beetle5a_sn01",
  .south = "sokoban/beetle/beetle5a_ns01",
  .east = "sokoban/beetle/beetle5a_we01",
  .west = "sokoban/beetle/beetle5a_ew01"
};

//const string kDefaultFrameName = "sokoban/beetle/beetle5a_sn01";

const float iterationDuration = 0.7;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BoxNode::BoxNode() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BoxNode::~BoxNode() {
  C6_F1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BoxNode::doAlignAgainst(const int actorX, const int actorY) {
  string newDFN = "";//kDefaultFrameNames.north;

  do {
    if (actorX == gameX) {
      if (actorY > gameY) {
        newDFN = kDefaultFrameNames.north;
      }
      else {
        newDFN = kDefaultFrameNames.south;
      }
      break;
    }

    if (actorY == gameY) {
      if (actorX > gameX) {
        newDFN = kDefaultFrameNames.east;
      }
      else {
        newDFN = kDefaultFrameNames.west;
      }
      break;
    }

  } while(false);

  if ("" != newDFN) {
    setSpriteFrame(newDFN);
//    currentDFN = newDFN;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 BoxNode::calculatePosition(const int x, const int y) const {
  Vec2 result = {.x = kTileSize/2, .y = kTileSize/2};
  //
  result.x = result.x + x*kTileSize;
  result.y = result.y + y*kTileSize;

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BoxNode* BoxNode::create(shared_ptr<SixCatsLogger> c6) {

  BoxNode *pRet = new(std::nothrow) BoxNode();
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->initSelf()) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BoxNode::doChangePositionTo(const int newGameX, const int newGameY) {
  Vec2 newPosition = calculatePosition(newGameX, newGameY);

  RotateBy* ra = RotateBy::create(iterationDuration, 360);
  MoveTo* ma = MoveTo::create(iterationDuration, newPosition);

  runAction(ra);
  runAction(ma);

  gameX = newGameX;
  gameY = newGameY;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BoxNode::initSelf() {
//  const string fn = "sokoban/beetle5a_sn05.png";
  if (!initWithSpriteFrameName(kDefaultFrameNames.north)) {
    C6_C2(c6, "Failed to init with file ", kDefaultFrameNames.north);
    return false;    //
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BoxNode::setGamePosition(const int newGameX, const int newGameY) {
  gameX = newGameX;
  gameY = newGameY;

  setPosition(calculatePosition(gameX, gameY));
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

