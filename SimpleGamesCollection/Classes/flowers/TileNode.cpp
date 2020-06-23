#include "flowers/TileNode.h"
#include "flowers/ZOrderValues.h"
#include "flowers/TileType.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace flowers;

static const int kTileSize = 64;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileNode::TileNode() {
  // gameSolved = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileNode::~TileNode() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileNode* TileNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  TileNode *result = new (std::nothrow) TileNode();

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

bool TileNode::initMineCounter() {
  Label* label = Label::createWithTTF("0", "fonts/Marker Felt.ttf", 32);
  label->setTextColor(Color4B(160,82,45,255));
  label->setAnchorPoint(Vec2(0.5,0.5));
  const Size cs = getContentSize();
  label->setPosition(cs.width/2, cs.height/2);
  addChild(label, kCounterZOrder);


  label->setVisible(false);

  mineCounter = label;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::reset() {
  if (mineCounter) {
    mineCounter->setVisible(false);
  }
  if (redFlag) {
    redFlag->setVisible(false);
  }

  setTileType(kStonesTileType);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TileNode::initRedFlag() {
  const string fn = "red_flag.png";
  redFlag = Sprite::createWithSpriteFrameName(fn);
  if (redFlag == nullptr) {
    return false;
  }

  redFlag->setPosition(Vec2(kTileSize/2, kTileSize/2));
  addChild(redFlag, kRedFlagZOrder);

  redFlag->setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool TileNode::initSelf() {
  const string fn = "stone.png";
  if (!initWithSpriteFrameName(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;    //
  }

  mineCounter = nullptr;
  redFlag = nullptr;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::setMineCounter(const int value) {
  if (value ==0) {
    return;
  }

  if (mineCounter==nullptr) {
    if (!initMineCounter()) {
      return;
    }
  }

  mineCounter->setVisible(true);

  ostringstream ss;
  ss << value;
  mineCounter->setString(ss.str());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::setTileType(const TileType newTileType) {
  string sfn = ""; // sprite frame name
  switch (newTileType) {
  case kGrassTileType:
    sfn = "grass.png";
    break;
  case kStonesTileType:
    sfn = "stone.png";
    break;
  default:
    C6_C2(c6, "Unknown tile type ", (int) newTileType);
    return;
  }

  setSpriteFrame(sfn);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void TileNode::switchRedFlag() {
  if (redFlag==nullptr) {
    if (!initRedFlag()) {
      return;
    }
  }

  redFlag->setVisible(!redFlag->isVisible());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
