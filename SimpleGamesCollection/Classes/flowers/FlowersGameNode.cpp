#include "flowers/FlowersGameNode.h"
#include "flowers/GameStateKeeper.h"
#include "flowers/TileNode.h"
#include "flowers/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace flowers;

static const int kTileSize = 64;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FlowersGameNode::FlowersGameNode() {
//
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FlowersGameNode::~FlowersGameNode() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FlowersGameNode* FlowersGameNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  FlowersGameNode *result = new (std::nothrow) FlowersGameNode();

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

bool FlowersGameNode::getGameResult() const {
  return gameStateKeeper->getGameResult();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FlowersGameNode::initGameStateKeeper() {
  gameStateKeeper = make_unique<GameStateKeeper>();
  gameStateKeeper->setLogger(c6);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FlowersGameNode::initSelf() {
  const string bgfn = "flowers/flowers_background.png";
  if (!initWithFile(bgfn)) {
    C6_C2(c6, "Failed to init with file ", bgfn);
    return false;    //
  }

  if (!initGameStateKeeper()) {
    return false;
  }

  if (!initTileNodes()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FlowersGameNode::initTileNodes() {
  // list<int>


  for (int i = 0; i<kFlowersGameWidth; i++) {
    for (int j = 0; j<kFLowersGameHeight; j++) {
      TileNode* tileNode = TileNode::create(c6);
      tileNode->setName(GameStateKeeper::generateTileNodeName(i,j));

      tileNode->setAnchorPoint(Vec2(0, 0));
      tileNode->setPosition(i*kTileSize, j*kTileSize);


      addChild(tileNode, kTilesZOrder);

    }
    //
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FlowersGameNode::processClick(const float clickX, const float clickY,
                                   const cocos2d::EventMouse::MouseButton button) {
  float tmpf;
  modf(kFlowersGameWidth*clickX, &tmpf);
  int row = (int)round(tmpf);

  modf(kFLowersGameHeight*clickY, &tmpf);
  int column = (int)round(tmpf);

  C6_D4(c6, "Click at position: ", row, ":", column);

  if (button == EventMouse::MouseButton::BUTTON_RIGHT) {
    C6_T2(c6, "That was right click: ", static_cast<int>(button));
    processRightClick(row, column);
    return false;
  }
  else if (button == EventMouse::MouseButton::BUTTON_LEFT) {
    return processLeftClick(row, column);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FlowersGameNode::processLeftClick(const int row, const int column) {
  list<pair<int, int> > changedPositions = gameStateKeeper->processClick(row, column);
  C6_D3(c6, "Need to update ", changedPositions.size(), " tiles");

  for(const pair<int, int>& chp: changedPositions) {
    string name = gameStateKeeper->generateTileNodeName(chp.first, chp.second);
    TileNode* node = dynamic_cast<TileNode*>(getChildByName(name));
    if (node == nullptr) {
      C6_D2(c6, "Failed to find child with name: ", name);
      return true;
    }

    node->setMineCounter(gameStateKeeper->getTileCounter(chp.first, chp.second));
    TileType tt = gameStateKeeper->getTileStatus(chp.first, chp.second);
    C6_T6(c6, "Tile Type for ", row, ":", column, " is ", (int)tt);
    node->setTileType(tt);
  }


  //finally
  if (gameStateKeeper->getGameIsOver() == true) {
    if (gameStateKeeper->getGameResult()) {
      C6_D1(c6, "Victory");
    }
    else {
      C6_D1(c6, "Player failed");
//      show mine at this place
    }
    return true;
  }

  return false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FlowersGameNode::processRightClick(const int row, const int column) {

  string name = gameStateKeeper->generateTileNodeName(row, column);
  TileNode* node = dynamic_cast<TileNode*>(getChildByName(name));
  if (node == nullptr) {
    C6_D2(c6, "Failed to find child with name: ", name);
    return;
  }

  node->switchRedFlag();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FlowersGameNode::resetForNewGame() {

  for (int i = 0; i<kFlowersGameWidth; i++) {
    for (int j = 0; j<kFLowersGameHeight; j++) {
      string nn = gameStateKeeper->generateTileNodeName(i,j);
      TileNode* node = dynamic_cast<TileNode*>(getChildByName(nn));
      if (node == nullptr) {
        continue;
      }
      node->reset();
    }
  }

  gameStateKeeper->reset();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

