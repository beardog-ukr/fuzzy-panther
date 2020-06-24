#include "flowers/GameStateKeeper.h"

// #include "fifteen/BoardCombinationsFactory.h"
#include "flowers/TileType.h"
#include "flowers/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

using namespace flowers;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static int gamePosToMatrixIndex(const int gameX, const int gameY) {
  int result = gameX + ((kFlowersGameHeight-1) -gameY)*kFlowersGameWidth;
  if ((result <0)||(result>=(kFlowersGameWidth*kFlowersGameHeight))) {
    return 0;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::GameStateKeeper() {
  initMap();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameStateKeeper::~GameStateKeeper() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameStateKeeper::checkIfAllRevealed() {
  bool allTilesAreVisible = true;
  for (int i = 0; i<(kFlowersGameWidth*kFlowersGameHeight); i++) {
    if (minesPresence[i]) {
      continue;
    }

    if (tilesVisibility[i] == false) {
      C6_D3(c6, "Tile at ", i, " is not visible");
      allTilesAreVisible = false;
      break;
    }
  }

  return allTilesAreVisible;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

string GameStateKeeper::generateTileNodeName(const int x, const int y) {
  ostringstream ss;
  ss << "tn_" << x << "_" << y;

  return ss.str();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::generateRandomMap(int* const map) {
  int minesCountMax = 20;

  for (int i = 0; i<kFlowersGameWidth*kFlowersGameHeight; i++) {
    map[i] = 0;
  }

  int currentMinesCount = 0;
  while(currentMinesCount<= minesCountMax) {
    int row = RandomHelper::random_int((int)0, kFlowersGameWidth-1);
    int column = RandomHelper::random_int((int)0, kFlowersGameHeight-1);

    int idx = gamePosToMatrixIndex(row, column);
    if (map[idx] ==0) {
      map[idx] = 1;
      currentMinesCount++;
    }
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameStateKeeper::getGameIsOver() const {
  return gameIsOver;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameStateKeeper::getGameResult() const {
  return (!gameWasFailed);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int GameStateKeeper::getTileCounter(const int x, const int y) const {
  const int idx = gamePosToMatrixIndex(x, y);
  int result = minesCount[idx];
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

TileType GameStateKeeper::getTileStatus(const int x, const int y)const {
  int idx = gamePosToMatrixIndex(x, y);

//  TileType result = kStonesTileType;
//  if (tilesVisibility[idx]) {
//    result = kGrassTileType;
//  }

  return tilesVisual[idx];
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::initMap() {
  gameIsOver = false;
  gameWasFailed = false;

//  int minesMap[kFlowersGameWidth*kFlowersGameHeight] = {
//    0,0,1,0,0, 1,0,0,0,0, 0,0,0,0,0,
//    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
//    0,0,1,0,0, 1,0,0,0,0, 0,0,0,0,0,
//    1,0,1,0,0, 1,0,0,0,0, 0,0,0,0,0,
//    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,

//    0,0,1,0,0, 0,0,0,0,0, 0,0,0,0,0,
//    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
//    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
//    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0,
//    0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0
//  };
  int minesMap[kFlowersGameWidth*kFlowersGameHeight];
  generateRandomMap(minesMap);

  for(int i = 0; i<kFlowersGameWidth; i++) {
    for(int j = 0; j<kFlowersGameHeight; j++) {
      int mIdx = gamePosToMatrixIndex(i,j);
      minesPresence[mIdx] = (minesMap[mIdx] != 0);
    }
  }

  for(int i = 0; i<kFlowersGameWidth; i++) {
    for(int j = 0; j<kFlowersGameHeight; j++) {
      int cntr = 0;

      int diffX[8] = {-1, 0, 1,-1, 1,-1, 0, 1};
      int diffY[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
      for (int di = 0; di<8; di++) {
        int x = i + diffX[di];
        int y = j + diffY[di];
        if ((x>=0)&&(x<kFlowersGameWidth)&&(y>=0)&&(y<kFlowersGameHeight)) {
          if (minesPresence[gamePosToMatrixIndex(x,y)]) {
            cntr++;
          }
        }
      }

      minesCount[gamePosToMatrixIndex(i,j)] = cntr;
    }
  }


  for (int i = 0; i<(kFlowersGameWidth*kFlowersGameHeight); i++) {
    tilesVisibility[i] = false;
  }

  for (int i = 0; i<(kFlowersGameWidth*kFlowersGameHeight); i++) {
    tilesVisual[i] = kStonesTileType;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

list<pair<int, int> > GameStateKeeper::processClick(const int row, const int column) {
  list<pair<int, int> > result;

  const int idx = gamePosToMatrixIndex(row, column);
  if (minesPresence[idx]) {
    gameIsOver = true;
    gameWasFailed = true;
    return result;
  }

  bool needToShowSomeTiles = false;

  //do nothing if tile is already visible
  if (tilesVisibility[idx]) {
    return result;
  }
  else {
    tilesVisibility[idx] = true;
    tilesVisual[idx] = kGrassTileType;
    result.push_back(make_pair(row, column));

    if (minesCount[idx] == 0) {
      needToShowSomeTiles = true;
    }
  }

  while (needToShowSomeTiles) {
    needToShowSomeTiles = false;

    for(int i = 0; i<kFlowersGameWidth; i++) {
      for(int j = 0; j<kFlowersGameHeight; j++) {
        int cellIdx = gamePosToMatrixIndex(i,j);
        if (minesCount[cellIdx] != 0) {
          continue;
        }
        if (tilesVisibility[cellIdx] == false) {
          continue;
        }

        int diffX[8] = {-1, 0, 1,-1, 1,-1, 0, 1};
        int diffY[8] = {-1,-1,-1, 0, 0, 1, 1, 1};
        for (int di = 0; di<8; di++) {
          int x = i + diffX[di];
          int y = j + diffY[di];
          if ((x>=0)&&(x<kFlowersGameWidth)&&(y>=0)&&(y<kFlowersGameHeight)) {
            const int npIdx = gamePosToMatrixIndex(x,y);   // neibor pos index
            if (!tilesVisibility[npIdx]) {
              tilesVisibility[npIdx] = true;
              tilesVisual[npIdx] = kGrassTileType;
              result.push_back(make_pair(x, y));

              if(minesCount[npIdx]==0) {
                needToShowSomeTiles = true;
              }
            }
          }
        }
      }
    }
  }

  if (checkIfAllRevealed()) {
    C6_D1(c6, "all tiles are visible, game over");
    gameIsOver = true;
    gameWasFailed = false;
    return result;
  }

  reevaluateTilesVisual(result);

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::reevaluateTilesVisual(list<pair<int, int> >& changedNodes) {
  C6_D1(c6, "here");

  // check for upper grass
  for(int i = 0; i<kFlowersGameWidth; i++) {
    for(int j = 0; j<kFlowersGameHeight-1; j++) {
      int idx = gamePosToMatrixIndex(i,j);
      int upperIdx = gamePosToMatrixIndex(i,j+1);

      bool c1 = (tilesVisibility[idx]==false) && tilesVisibility[upperIdx];
      if (c1) {
        if (tilesVisual[upperIdx] == kGrassTileType) {
          tilesVisual[upperIdx] = kGrassUpTileType;
          changedNodes.push_back(make_pair(i,j+1));
        }
      }

      // for tiles previously marked as "grass up", but now lower tile became visible
      bool c2 = tilesVisibility[idx] && tilesVisibility[upperIdx];
      if (c2) {


        if (tilesVisual[upperIdx] == kGrassUpTileType) {
          C6_D4(c6, "Need to set visual at ", i, ":", j+1);
          tilesVisual[upperIdx] = kGrassTileType;
          changedNodes.push_back(make_pair(i,j+1));
        }
      }
    }
  }

  // check for stone up from grass
  for(int i = 0; i<kFlowersGameWidth; i++) {
    for(int j = 0; j<kFlowersGameHeight-1; j++) {
      int idx = gamePosToMatrixIndex(i,j);
      int upperIdx = gamePosToMatrixIndex(i,j+1);

      bool c1 = (tilesVisibility[upperIdx]==false) && tilesVisibility[idx];
      if (c1) {
        if (tilesVisual[upperIdx] == kStonesTileType) {
          tilesVisual[upperIdx] = kStonesUpTileType;
          changedNodes.push_back(make_pair(i,j+1));
        }
      }

//      // for tiles previously marked as "grass up", but now lower tile became visible
//      bool c2 = tilesVisibility[idx] && tilesVisibility[upperIdx];
//      if (c2) {


//        if (tilesVisual[upperIdx] == kGrassUpTileType) {
//          C6_D4(c6, "Need to set visual at ", i, ":", j+1);
//          tilesVisual[upperIdx] = kGrassTileType;
//          changedNodes.push_back(make_pair(i,j+1));
//        }
//      }
    }
  }



}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameStateKeeper::reset() {
  gameIsOver = false;
  gameWasFailed = false;
  initMap();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .



