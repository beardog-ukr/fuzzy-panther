#include "fifteen/FifteenGameNode.h"

#include "fifteen/BoardCombinationsFactory.h"
#include "fifteen/TokenNode.h"
#include "fifteen/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace fifteen;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int gamePosToTokenArrIndex(const int gameX, const int gameY) {
  int result = gameX + ((4-1) -gameY)*4;
  if ((result <0)||(result>=16)) {
    return 0;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FifteenGameNode::FifteenGameNode() {
  gameSolved = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FifteenGameNode::~FifteenGameNode() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenGameNode::checkIfSolved() const {
  if (!((emptyPlaceX==3)&&(emptyPlaceY==0))) {
    return false;
  }

  int xi = 0;
  int yi = 3;
  for(int i = 0; i<15; i++) {
    int value = tokenNodes[i]->getValue();
    if (value != (i+1)) {
      C6_D4(c6, "Puzzle incomplete: ", value, " is on place ", i);
      return false;
    }

    xi++;
    if (xi >=4) {
      xi = 0;
      yi--;
    }
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FifteenGameNode* FifteenGameNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  FifteenGameNode *result = new (std::nothrow) FifteenGameNode();

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

bool FifteenGameNode::initSelf() {
  const string bgfn = "fifteen/fifteen_background.png";
  if (!initWithFile(bgfn)) {
    C6_C2(c6, "Failed to init with file ", bgfn);
    return false;    //
  }

  if (!initTokens()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenGameNode::initTokens() {
  // tokens.resize(16);
  combinationsFactory = make_unique<BoardCombinationsFactory>();
  combinationsFactory->setLogger(c6);

  vector<int> tokens = combinationsFactory->getRandomCombination();

  tokenNodes.resize(16);

  int xi = 0;
  int yi = 3;
  for(int i = 0; i<16; i++) {
    if (tokens[i] !=0) {
      TokenNode* tokenNode = TokenNode::create(tokens[i], c6);
      if (tokenNode == nullptr) {
        return false;
      }

      tokenNode->setGamePosition(xi, yi);
      addChild(tokenNode, kTokensZOrder);
      tokenNodes[i] = tokenNode;
    }
    else {
      emptyPlaceX = xi;
      emptyPlaceY = yi;
    }

    xi++;
    if (xi >=4) {
      xi = 0;
      yi--;
    }
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenGameNode::processClick(const int gameX, const int gameY) {
  if (gameSolved) {
    return true;
  }

  int xEmptyDiff = gameX - emptyPlaceX;
  int yEmptyDiff = gameY - emptyPlaceY;

  int tokenIdx = gamePosToTokenArrIndex(gameX, gameY);

  bool canBeProcessed = (xEmptyDiff==1)&&(yEmptyDiff==0);
  canBeProcessed = canBeProcessed || ((xEmptyDiff==-1)&&(yEmptyDiff==0));
  canBeProcessed  = canBeProcessed  || ((xEmptyDiff==0)&&(yEmptyDiff==1));
  canBeProcessed  = canBeProcessed || ((xEmptyDiff==0)&&(yEmptyDiff==-1));

  if (!canBeProcessed) {
    C6_D4(c6, "Ignoring :", gameX, ":", gameY);
    C6_D4(c6, "  Current empty :", emptyPlaceX, ":", emptyPlaceY);
    return false;
  }

  tokenNodes[tokenIdx]->doChangePositionTo(emptyPlaceX, emptyPlaceY);
  int oldEmptyIdx = gamePosToTokenArrIndex(emptyPlaceX, emptyPlaceY);

  emptyPlaceX = gameX;
  emptyPlaceY = gameY;

  tokenNodes[oldEmptyIdx] = tokenNodes[tokenIdx];
  tokenNodes[tokenIdx] = nullptr;

  gameSolved = checkIfSolved();

  return gameSolved;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FifteenGameNode::resetForNewGame() {
  for(int i = 0; i<16; i++) {
    if (tokenNodes[i]!=nullptr) {
      tokenNodes[i]->removeFromParentAndCleanup(true);
    }
  }

  initTokens();
  gameSolved = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

