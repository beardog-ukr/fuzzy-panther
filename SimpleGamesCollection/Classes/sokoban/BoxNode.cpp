#include "sokoban/BoxNode.h"
using namespace sokoban;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

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

Vec2 BoxNode::calculatePosition(const int x, const int y) const {
  Vec2 result = {.x = tileSize/2, .y = tileSize/2};
  if ((x>=4) ||(x<0) ||(y>=4) ||(y<0) ) {
    return result;
  }
  //
  result.x = result.x + x*tileSize;
  result.y = result.y + y*tileSize;

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
  const string fn = "sokoban/beetle5a_sn05.png";
  if (!initWithFile(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;    //
  }

  const Size itemSize = getContentSize();
  tileSize = itemSize.width;

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

