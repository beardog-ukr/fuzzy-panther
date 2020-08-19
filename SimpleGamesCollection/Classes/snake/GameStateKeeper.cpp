#include "snake/GameStateKeeper.h"
#include "snake/BodyPartsFactory.h"

#include "snake/DirectionType.h"
// #include "flowers/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

using namespace snake;


static const int kMapXLimit = 14;
static const int kMapYLimit = 8;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//static int gamePosToMatrixIndex(const int gameX, const int gameY) {
//  int result = gameX + ((kSnakeGameHeight-1) -gameY)*kSnakeGameWidth;
//  if ((result <0)||(result>=(kSnakeGameWidth*kSnakeGameHeight))) {
//    return 0;
//  }

//  return result;
//}

//static inline int calculateNextX()

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::GameStateKeeper() {
//  initMap();
  debugGameoverCountdown = 4;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::~GameStateKeeper() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SnakeElementInfo GameStateKeeper::getBarrelInfo() const {
  SnakeElementInfo result;
  result.gameX = barrelX;
  result.gameY = barrelY;
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::performIdleConsume(const SnakeElementInfo& newHeadInfo) {
  elements.push_front(newHeadInfo);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MoveResultType GameStateKeeper::performNextMove() {
  debugGameoverCountdown--;
  if (debugGameoverCountdown <=0) {
    C6_D1(c6, "Debug game over happened.");
    return kGameFailure;
  }

  if (elements.size()==0) {
    return kGameFailure;    //impossible
  }

  SnakeElementInfo currentHeadInfo = elements.front();

  int diffX = 0;
  int diffY = 0;
  switch(nextMove ) {
  case kMoveEast:
    diffX = 1;
    break;
  case kMoveWest:
    diffX = -1;
    break;
  case kMoveNorth:
    diffY = 1;
    break;
  case kMoveSouth:
    diffY = -1;
    break;
    // no default section here
  }

  SnakeElementInfo newHeadInfo;
  newHeadInfo.previousX = currentHeadInfo.gameX;
  newHeadInfo.gameX = currentHeadInfo.gameX + diffX;
  if (newHeadInfo.gameX<0) {
    newHeadInfo.gameX = kMapXLimit-1;
  }
  else if(newHeadInfo.gameX>=kMapXLimit) {
    newHeadInfo.gameX = 0;
  }

  newHeadInfo.previousY = currentHeadInfo.gameY;
  newHeadInfo.gameY = currentHeadInfo.gameY + diffY;
  if (newHeadInfo.gameY<0) {
    newHeadInfo.gameY = kMapYLimit-1;
  }
  else if(newHeadInfo.gameY>=kMapYLimit) {
    newHeadInfo.gameY = 0;
  }


  // --- check if bites
  for (const SnakeElementInfo& element: elements) {
    if ((element.gameX == newHeadInfo.gameX)&&(element.gameY == newHeadInfo.gameY)) {
      currentMoveResult = kGameFailure;
      return currentMoveResult;
    }
  }

  // ---
  if ((newHeadInfo.gameX == barrelX)&&(newHeadInfo.gameY==barrelY)) {
    performIdleConsume(newHeadInfo);
    currentMoveResult = kIdleConsume;
  }
  else {
    performSimpleMove(newHeadInfo);
    currentMoveResult = kSimpleMove;
  }

  return currentMoveResult;

//  newElements.push_back(newHeadInfo);
//  C6_T4(c6, "Head: ", newHeadInfo.gameX, ":", newHeadInfo.gameY);
//  C6_D4(c6, "head pp: ", newHeadInfo.previousX, ":", newHeadInfo.previousY);

//  SnakeElementInfo previousElement = newHeadInfo;

//  elements.pop_front();
//  for(const SnakeElementInfo& element:elements) {
//    SnakeElementInfo newElement;
//    newElement.previousX = element.gameX;
//    newElement.previousY = element.gameY;

//    newElement.gameX = previousElement.previousX;
//    newElement.gameY = previousElement.previousY;

//    previousElement = newElement;

//    newElements.push_back(newElement);
//    C6_D4(c6, "new element: ", newElement.gameX, ":", newElement.gameY);
//    C6_D4(c6, "new element pp: ", newElement.previousX, ":", newElement.previousY);
//  }

//  elements.clear();
//  elements = newElements;

//  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::performSimpleMove(const SnakeElementInfo& newHeadInfo) {
  list<SnakeElementInfo> newElements;
  newElements.push_back(newHeadInfo);
  C6_T4(c6, "Head: ", newHeadInfo.gameX, ":", newHeadInfo.gameY);
  C6_D4(c6, "head pp: ", newHeadInfo.previousX, ":", newHeadInfo.previousY);

  SnakeElementInfo previousElement = newHeadInfo;

  elements.pop_front();
  for(const SnakeElementInfo& element:elements) {
    SnakeElementInfo newElement;
    newElement.previousX = element.gameX;
    newElement.previousY = element.gameY;

    newElement.gameX = previousElement.previousX;
    newElement.gameY = previousElement.previousY;

    previousElement = newElement;

    newElements.push_back(newElement);
    C6_D4(c6, "new element: ", newElement.gameX, ":", newElement.gameY);
    C6_D4(c6, "new element pp: ", newElement.previousX, ":", newElement.previousY);
  }

  elements.clear();
  elements = newElements;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

list<SnakeElementInfo> GameStateKeeper::prepareSnakeInfo() const {
  list<SnakeElementInfo> result;

  for (const SnakeElementInfo& sei:elements) {
    result.push_back(sei);
  }

  if (currentMoveResult == kIdleConsume) {
    result.pop_front();
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::putBarrelToRandom() {
  int newBarrelX;
  int newBarrelY;

  bool newPlaceIsOk = false;
  while (!newPlaceIsOk) {
    newBarrelX = RandomHelper::random_int((int)0, kMapXLimit-1);
    newBarrelY = RandomHelper::random_int((int)0, kMapYLimit-1);

    newPlaceIsOk = true;

    for (const auto& element: elements) {
      if ((element.gameX ==newBarrelX) && (element.gameY == newBarrelY)) {
        newPlaceIsOk = false;
        break;
      }
    }

//  TODO: also check if new barrel pos is not equal to current barrel pos?
  }

  barrelX = newBarrelX;
  barrelY = newBarrelY;
  C6_D4(c6, "Barrel pos is: ", newBarrelX, ":", newBarrelY);

//  barrelX = 9;
//  barrelY = 6;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::reset() {
  nextMove = kMoveEast;

  elements.clear();

  SnakeElementInfo e1 = {.previousX = 0, .previousY = 0, .gameX = 6, .gameY = 6};
  SnakeElementInfo e2 = {.previousX = 0, .previousY = 0, .gameX = 5, .gameY = 6};
  SnakeElementInfo e3 = {.previousX = 0, .previousY = 0, .gameX = 4, .gameY = 6};

  elements.push_back(e1);
  elements.push_back(e2);
  elements.push_back(e3);

  //--- barrel
  putBarrelToRandom();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::setNextMove(const MoveType move) {
  nextMove = move;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .



