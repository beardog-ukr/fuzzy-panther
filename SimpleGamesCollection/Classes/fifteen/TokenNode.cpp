#include "fifteen/TokenNode.h"
using namespace fifteen;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

const float iterationDuration = 0.7;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TokenNode::TokenNode(const int v) {
  value = v;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TokenNode::~TokenNode() {
  C6_F1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 TokenNode::calculatePosition(const int x, const int y) const {
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

TokenNode* TokenNode::create(const int number, shared_ptr<SixCatsLogger> c6) {

  TokenNode *pRet = new(std::nothrow) TokenNode(number);
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

void TokenNode::doChangePositionTo(const int newGameX, const int newGameY) {
  Vec2 newPosition = calculatePosition(newGameX, newGameY);

  RotateBy* ra = RotateBy::create(iterationDuration, 360);
  MoveTo* ma = MoveTo::create(iterationDuration, newPosition);

  runAction(ra);
  runAction(ma);

  gameX = newGameX;
  gameY = newGameY;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int TokenNode::getValue() const {
  return value;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TokenNode::initSelf() {
  const string fn = "fifteen/base_chip.png";
  if (!initWithFile(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;    //
  }

  const Size itemSize = getContentSize();
  tileSize = itemSize.width;

  if (!initLabelNumber()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TokenNode::initLabelNumber() {
  ostringstream ss;
  ss<< value;

  Label* label = Label::createWithTTF(ss.str(), "fonts/Marker Felt.ttf", 48);
  if (label == nullptr) {
    return false;
  }

  label->setTextColor(Color4B(180,20,20,255));
  label->setAnchorPoint(Vec2(0.5,0.5));
  label->setPosition(tileSize/2, tileSize/2);
  addChild(label);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TokenNode::setGamePosition(const int newGameX, const int newGameY) {
  gameX = newGameX;
  gameY = newGameY;

  setPosition(calculatePosition(gameX, gameY));
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

