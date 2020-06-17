#include "blocks/FigureFactory.h"
#include "blocks/BrickInfo.h"
#include "blocks/FigureInfo.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace blocks;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureFactory::FigureFactory() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureFactory::~FigureFactory() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureInfo* FigureFactory::composeNewFigure(const int initialRow) {
  FigureInfo* result = new FigureInfo();
  result->setLogger(c6);
//  result->setBaseRow(initialRow);

  const int quadBricksCount = 4;
  int quadBricksXPositions[quadBricksCount];
  int quadBricksYPositions[quadBricksCount];
  quadBricksXPositions[0] = 1;
  quadBricksYPositions[0] = 2;
  quadBricksXPositions[1] = 2;
  quadBricksYPositions[1] = 2;
  quadBricksXPositions[2] = 1;
  quadBricksYPositions[2] = 1;
  quadBricksXPositions[3] = 2;
  quadBricksYPositions[3] = 1;

//  = {
//    { .gameX = 1, .gameY = 2, .node = nullptr},  { .gameX = 2, .gameY = 2, .node = nullptr},
//    { .gameX = 1, .gameY = 1, .node = nullptr},  { .gameX = 2, .gameY = 1, .node = nullptr}
//  };

  const int initialColumn = 3;

  for (int i = 0; i<quadBricksCount; i++) {
    BrickInfo positionedBrick;
    positionedBrick.node = nullptr;
    positionedBrick.gameX = quadBricksXPositions[i] + initialColumn;
    positionedBrick.gameY = quadBricksYPositions[i] + initialRow;
    result->addInitialBrick(positionedBrick);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


