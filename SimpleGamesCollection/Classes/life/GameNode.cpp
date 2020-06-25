#include "life/GameNode.h"
#include "life/CellNode.h"
#include "life/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace life;

static const int kTileSize = 64;
const string kFlowersPlistFileName = "flowers/flowers.plist";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static int matrixWidth;
static int matrixHeight;

static int gamePosToMatrixIndex(const int gameX, const int gameY) {
  int result = gameX + ((matrixHeight-1) -gameY)*matrixWidth;
  if ((result <0)||(result>=(matrixWidth*matrixHeight))) {
    return 0;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::GameNode() {
//
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::~GameNode() {
  CellNode::unloadSpriteCache();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::applyChanges() {
  for (int i = 0; i<matrixHeight*matrixWidth; i++) {
    if (cellNodes[i]) {
      cellNodes[i]->prepareNextChange();
    }
  }

  for (int i = 0; i<matrixHeight*matrixWidth; i++) {
    if (cellNodes[i]) {
      cellNodes[i]->applyChanges();
    }
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode* GameNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  GameNode *result = new (std::nothrow) GameNode();

  if (result == nullptr) {
    return result;
  }

  result->setLogger(inc6);

  if (!result->initSelf()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initCellNodes() {
  const Size cs = getContentSize();

  matrixWidth = ceil(cs.width / (CellNode::kCellSideF));
  matrixHeight = ceil(cs.height / (CellNode::kCellSideF));

  cellNodes.resize(matrixHeight*matrixWidth);

  for(int i = 0; i<matrixHeight*matrixWidth; i++) {
    CellNode* cn = CellNode::create(c6);
    cn->setAnchorPoint(Vec2(0,0));

    cellNodes[i] = cn;
  }

  for (int i = 0; i<matrixWidth; i++) {
    for (int j = 0; j<matrixHeight; j++) {
      const Vec2 pos = {.x = ((float)i)*CellNode::kCellSide,
                        .y = ((float)j)*CellNode::kCellSide};

      const int idx = gamePosToMatrixIndex(i,j);
      cellNodes[idx]->setPosition(pos);
      addChild(cellNodes[idx], kCellsZOrder);

      int diffX[8] = {-1, 0, 1,-1, 1,-1, 0, 1};
      int diffY[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
      for (int di = 0; di<8; di++) {
        int x = i + diffX[di];
        int y = j + diffY[di];
        if ((x>=0)&&(x<matrixWidth)&&(y>=0)&&(y<matrixHeight)) {
          cellNodes[idx]->addNeighborNode(cellNodes[gamePosToMatrixIndex(x,y)]);
        }
      }
    }

  }


  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initSelf() {
  const string bgfn = "life/life_test_background.png";
  if (!initWithFile(bgfn)) {
    C6_C2(c6, "Failed to init with file ", bgfn);
    return false;    //
  }

  if (!CellNode::loadSpriteCache(c6)) {
    return false;
  }

  if (!initCellNodes()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::processClick(const float clickX, const float clickY,
                            const cocos2d::EventMouse::MouseButton button) {
  float tmpf;
  modf(matrixWidth*clickX, &tmpf);
  int row = (int)round(tmpf);

  modf(matrixHeight*clickY, &tmpf);
  int column = (int)round(tmpf);

  C6_F4(c6, "Click at position: ", row, ":", column);

  int idx = gamePosToMatrixIndex(row, column);

  if (button == EventMouse::MouseButton::BUTTON_RIGHT) {
    cellNodes[idx]->markKill();
  }
  else if (button == EventMouse::MouseButton::BUTTON_LEFT) {
    cellNodes[idx]->markAlive();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
