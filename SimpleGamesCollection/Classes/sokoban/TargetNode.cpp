#include "sokoban/TargetNode.h"
using namespace sokoban;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

const int kTileSize = 48;

const string kFilledFrameName = "sokoban/target/filled";
const string kOpenFrameName = "sokoban/target/open";

const float iterationDuration = 0.7;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TargetNode::TargetNode() {
  currentFilledState = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TargetNode::~TargetNode() {
  C6_F1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 TargetNode::calculatePosition(const int x, const int y) const {
  Vec2 result = {.x = kTileSize/2, .y = kTileSize/2};
  //
  result.x = result.x + x*kTileSize;
  result.y = result.y + y*kTileSize;

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TargetNode* TargetNode::create(shared_ptr<SixCatsLogger> c6) {
  TargetNode *pRet = new(std::nothrow) TargetNode();
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

bool TargetNode::initSelf() {
  if (!initWithSpriteFrameName(kOpenFrameName)) {
    C6_C2(c6, "Failed to init with file ", kOpenFrameName);
    return false;    //
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TargetNode::setFilled(const bool filled) {
  if (currentFilledState == filled) {
    return;
  }

  string fs = kOpenFrameName;
  currentFilledState = true;
  if (filled == true) {
    fs = kFilledFrameName;
    currentFilledState = false;
  }

  setSpriteFrame(fs);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TargetNode::setGamePosition(const int newGameX, const int newGameY) {
  gameX = newGameX;
  gameY = newGameY;

  setPosition(calculatePosition(gameX, gameY));
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

