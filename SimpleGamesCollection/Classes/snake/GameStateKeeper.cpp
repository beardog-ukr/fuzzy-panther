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

GameStateKeeper::GameStateKeeper() {
  debugGameoverCountdown = 4;
  movesArchive.resize(400);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::~GameStateKeeper() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SnakeElementInfo GameStateKeeper::getBarrelInfo() const {
  SnakeElementInfo result;
  result.targetX = barrelX;
  result.targetY = barrelY;
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::performIdleConsume(const int headX, const int headY) {
  C6_D1(c6, "Here.");

  // ---
  for(SnakeElementInfo& ei: elements) {
    ei.moveTypeIdx = ei.moveTypeIdx + 1;
  }

  // --- add new head
  SnakeElementInfo headInfo = {.previousX = 0, .previousY = 0,
                               .targetX = headX, .targetY = headY, .moveTypeIdx = 0};
  elements.push_front(headInfo);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MoveResultType GameStateKeeper::performNextMove() {
//  debugGameoverCountdown--;
//  if (debugGameoverCountdown <=0) {
//    C6_D1(c6, "Debug game over happened.");
//    return kGameFailure;
//  }

  if (elements.size()==0) {
    return kGameFailure;    //impossible
  }

  // --- shift moves archive
  for(int i = movesArchiveCount; i>0; i--) {
    movesArchive[i] = movesArchive[i-1];
  }
  movesArchiveCount++;
  movesArchive[0] = nextMove;

  SnakeElementInfo currentHeadInfo;
  currentHeadInfo.moveTypeIdx = -1; //invalid value
  for(const SnakeElementInfo& ei: elements) {
    if (ei.moveTypeIdx ==0) {
      currentHeadInfo = ei;
      break;
    }
  }

  if (currentHeadInfo.moveTypeIdx != 0) {
    C6_T1(c6, "Unexpected failure.");
    return kGameFailure;   //impossible and unexpected
  }

  int newHeadX = currentHeadInfo.targetX;
  int newHeadY = currentHeadInfo.targetY;
  switch(nextMove ) {
  case kMoveEast:
    newHeadX = newHeadX +1;
    break;
  case kMoveWest:
    newHeadX = newHeadX -1;
    break;
  case kMoveNorth:
    newHeadY = newHeadY +1;
    break;
  case kMoveSouth:
    newHeadY = newHeadY -1;
    break;
    // no default section here
  }

  // --- check if bites itself
  for (const SnakeElementInfo& element: elements) {
    if ((element.targetX == newHeadX)&&(element.targetY == newHeadY)) {
      currentMoveResult = kGameFailure;
      return currentMoveResult;
    }
  }

  // ---
  if ((newHeadX == barrelX)&&(newHeadY==barrelY)) {
    performIdleConsume(newHeadX, newHeadY);
    currentMoveResult = kIdleConsume;
  }
  else {
    performSimpleMove();
    currentMoveResult = kSimpleMove;
  }

  return currentMoveResult;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::performSimpleMove() {
  list<SnakeElementInfo> newElements;

  for(const SnakeElementInfo& element:elements) {
    if ( (element.targetX>=kMapXLimit) || (element.targetX<=-1) ||
         (element.targetY>=kMapYLimit) || (element.targetY<=-1)) {
      C6_T4(c6, "skipped element from ", element.targetX, ":", element.targetY);
      continue;
    }

    SnakeElementInfo newElement;

    int diffX = 0;
    int diffY = 0;
    switch(movesArchive[element.moveTypeIdx] ) {
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

    newElement.moveTypeIdx = element.moveTypeIdx;

    newElement.previousX = element.targetX;
    newElement.previousY = element.targetY;

    newElement.targetX = element.targetX + diffX;
    newElement.targetY = element.targetY + diffY;

    c6->t(__c6_MN__, [element, newElement]() -> std::string {
      ostringstream ss;
      ss << "new element moves " << newElement.previousX << ":" << newElement.previousY
         << " -> " << newElement.targetX << ":" << newElement.targetY;
      return ss.str();
    });
    newElements.push_back(newElement);

    // --- extra processing
    bool needExtra = false;
    SnakeElementInfo extraElement = newElement;
    if(newElement.targetX==kMapXLimit) {
      extraElement.targetX = 0;
      extraElement.previousX = -1;
      needExtra = true;
    }
    if(newElement.targetX==-1) {
      extraElement.targetX = kMapXLimit-1;
      extraElement.previousX = kMapXLimit;
      needExtra = true;
    }
    if(newElement.targetY==kMapYLimit) {
      extraElement.targetY = 0;
      extraElement.previousY = -1;
      needExtra = true;
    }
    if(newElement.targetY==-1) {
      extraElement.targetY = kMapYLimit-1;
      extraElement.previousY = kMapYLimit;
      needExtra = true;
    }

    if (needExtra) {
      c6->t(__c6_MN__, [extraElement]() -> std::string {
        ostringstream ss;
        ss << "extra element moves " << extraElement.previousX << ":" << extraElement.previousY
           << " -> " << extraElement.targetX << ":" << extraElement.targetY;
        return ss.str();
      });

      newElements.push_back(extraElement);
    }
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
      if ((element.targetX ==newBarrelX) && (element.targetY == newBarrelY)) {
        newPlaceIsOk = false;
        break;
      }
    }

//  TODO: also check if new barrel pos is not equal to current barrel pos?
  }

  barrelX = newBarrelX;
  barrelY = newBarrelY;
  C6_D4(c6, "Barrel pos is: ", newBarrelX, ":", newBarrelY);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::reset() {
  nextMove = kMoveEast;

  elements.clear();

  SnakeElementInfo e1 = {.previousX = 0, .previousY = 0,
                         .targetX = 10, .targetY = 6, .moveTypeIdx = 0};

  SnakeElementInfo e2 = {.previousX = 0, .previousY = 0,
                         .targetX = 9, .targetY = 6, .moveTypeIdx = 1};

  SnakeElementInfo e3 = {.previousX = 0, .previousY = 0,
                         .targetX = 8, .targetY = 6, .moveTypeIdx = 2};

  movesArchive[0] = kMoveEast;
  movesArchive[1] = kMoveEast;
  movesArchive[2] = kMoveEast;
  movesArchiveCount = 3;

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



