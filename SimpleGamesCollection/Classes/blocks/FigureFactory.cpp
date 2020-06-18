#include "blocks/FigureFactory.h"
#include "blocks/BrickInfo.h"
#include "blocks/FigureInfo.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace blocks;

map<FigureType, FigureType> rotationVariants = {
  {kQuad, kQuad},
  {kZR1, kZR2 },  {kZR2, kZR1},
  {kZMirroredR1, kZMirroredR2 },
  {kZMirroredR2, kZMirroredR1},
  {kTR1, kTR2}, {kTR2, kTR3}, {kTR3, kTR4}, {kTR4, kTR1},
  {kLR1, kLR2}, {kLR2, kLR3}, {kLR3, kLR4}, {kLR4, kLR1},

  {kLMirroredR1, kLMirroredR2}, {kLMirroredR2, kLMirroredR3},
  {kLMirroredR3, kLMirroredR4}, {kLMirroredR4, kLMirroredR1},
  {kIR1, kIR2}, {kIR2, kIR1},
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureFactory::FigureFactory() {
//  prepareRotationVariants()
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureFactory::~FigureFactory() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureInfo* FigureFactory::composeNewFigure(const int initialRow) {
  FigureInfo* result = new FigureInfo();
  result->setLogger(c6);

  const FigureType baseFigureType = getNextRandomType();
  list<pair<int, int> > brickData = getNewFigureTemplate(baseFigureType);

  const int initialColumn = 3;

//  int rotationsAmount = 1;
  result->setRotationsAmount(1);

  for (const auto& bd: brickData) {
    BrickInfo positionedBrick;
    positionedBrick.node = nullptr;
    positionedBrick.gameX = bd.first + initialColumn;
    positionedBrick.gameY = bd.second + initialRow;
    result->addInitialBrick(0, positionedBrick);
  }

  FigureType figureType2 = baseFigureType;
  auto searchResult = rotationVariants.find(baseFigureType);
  if (searchResult != rotationVariants.end()) {
    figureType2 = searchResult->second;
  }

  if (baseFigureType == figureType2) {
    return result;
  }

  result->setRotationsAmount(2);
  brickData = getNewFigureTemplate(figureType2);
  for (const auto& bd: brickData) {
    BrickInfo positionedBrick;
    positionedBrick.node = nullptr;
    positionedBrick.gameX = bd.first + initialColumn;
    positionedBrick.gameY = bd.second + initialRow;
    result->addInitialBrick(1, positionedBrick);
  }

  FigureType figureType3 = figureType2;
  searchResult = rotationVariants.find(figureType2);
  if (searchResult != rotationVariants.end()) {
    figureType3 = searchResult->second;
  }

  if ((baseFigureType == figureType3)||(figureType2 == figureType3)) {
    return result;
  }

  result->setRotationsAmount(3);
  brickData = getNewFigureTemplate(figureType3);
  for (const auto& bd: brickData) {
    BrickInfo positionedBrick;
    positionedBrick.node = nullptr;
    positionedBrick.gameX = bd.first + initialColumn;
    positionedBrick.gameY = bd.second + initialRow;
    result->addInitialBrick(2, positionedBrick);
  }

  FigureType figureType4 = figureType3;
  searchResult = rotationVariants.find(figureType3);
  if (searchResult != rotationVariants.end()) {
    figureType4 = searchResult->second;
  }

  if ((baseFigureType == figureType4)
      ||(figureType2 == figureType4)||(figureType3 == figureType4)) {
    return result;
  }

  result->setRotationsAmount(4);
  brickData = getNewFigureTemplate(figureType4);
  for (const auto& bd: brickData) {
    BrickInfo positionedBrick;
    positionedBrick.node = nullptr;
    positionedBrick.gameX = bd.first + initialColumn;
    positionedBrick.gameY = bd.second + initialRow;
    result->addInitialBrick(3, positionedBrick);
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

std::list<std::pair<int, int> > FigureFactory::getNewFigureTemplate(const FigureType type) {
  list<pair<int, int> > result;

  switch (type) {
  case kQuad:
    result.push_back(make_pair(1,1));
    result.push_back(make_pair(2,1));
    result.push_back(make_pair(1,2));
    result.push_back(make_pair(2,2));
    break;

  case kZR1:
    result.push_back(make_pair(1,0));
    result.push_back(make_pair(2,0));
    result.push_back(make_pair(0,1));
    result.push_back(make_pair(1,1));
    break;

  case kZR2:
    result.push_back(make_pair(1,0));
    result.push_back(make_pair(1,1));
    result.push_back(make_pair(2,1));
    result.push_back(make_pair(2,2));
    break;

  case kZMirroredR1:
    result.push_back(make_pair(0,0));
    result.push_back(make_pair(1,0));
    result.push_back(make_pair(1,1));
    result.push_back(make_pair(2,1));
    break;

  case kZMirroredR2:
    result.push_back(make_pair(1,0));
    result.push_back(make_pair(0,1));
    result.push_back(make_pair(1,1));
    result.push_back(make_pair(0,2));
    break;

  case kTR1:
    result.push_back(make_pair(0,0)); //2x
    result.push_back(make_pair(0,1)); //1xx
    result.push_back(make_pair(1,1)); //0x
    result.push_back(make_pair(0,2)); // 012
    break;

  case kTR2:
    result.push_back(make_pair(0,0));
    result.push_back(make_pair(1,0));
    result.push_back(make_pair(2,0));
    result.push_back(make_pair(1,1));
    break;

  case kTR3:
    result.push_back(make_pair(2,0)); //2  x
    result.push_back(make_pair(1,1)); //1 xx
    result.push_back(make_pair(2,1)); //0  x
    result.push_back(make_pair(2,2)); // 012
    break;

  case kTR4:
    result.push_back(make_pair(1,1)); //2xxx
    result.push_back(make_pair(0,2)); //1 x
    result.push_back(make_pair(1,2)); //0
    result.push_back(make_pair(2,2)); // 012
    break;

  case kLR1:
    result.push_back(make_pair(0,0)); //2x
    result.push_back(make_pair(1,0)); //1x
    result.push_back(make_pair(0,1)); //0xx
    result.push_back(make_pair(0,2)); // 012
    break;

  case kLR2:
    result.push_back(make_pair(0,0)); //2
    result.push_back(make_pair(1,0)); //1  x
    result.push_back(make_pair(2,0)); //0xxx
    result.push_back(make_pair(2,1)); // 012
    break;

  case kLR3:
    result.push_back(make_pair(2,0)); //2 xx
    result.push_back(make_pair(2,1)); //1  x
    result.push_back(make_pair(1,2)); //0  x
    result.push_back(make_pair(2,2)); // 012
    break;

  case kLR4:
    result.push_back(make_pair(0,1)); //2xxx
    result.push_back(make_pair(0,2)); //1x
    result.push_back(make_pair(1,2)); //0
    result.push_back(make_pair(2,2)); // 012
    break;

  // ---
  case kLMirroredR1:
    result.push_back(make_pair(1,0)); //2  x
    result.push_back(make_pair(2,0)); //1  x
    result.push_back(make_pair(2,1)); //0 xx
    result.push_back(make_pair(2,2)); // 012
    break;

  case kLMirroredR2:
    result.push_back(make_pair(2,1)); //2xxx
    result.push_back(make_pair(0,2)); //1  x
    result.push_back(make_pair(1,2)); //0
    result.push_back(make_pair(2,2)); // 012
    break;

  case kLMirroredR3:
    result.push_back(make_pair(0,0)); //2xx
    result.push_back(make_pair(0,1)); //1x
    result.push_back(make_pair(0,2)); //0x
    result.push_back(make_pair(1,2)); // 012
    break;

  case kLMirroredR4:
    result.push_back(make_pair(0,0)); //2
    result.push_back(make_pair(1,0)); //1x
    result.push_back(make_pair(2,0)); //0xxx
    result.push_back(make_pair(0,1)); // 012
    break;

  // ---
  case kIR1:                          // x
    result.push_back(make_pair(0,0)); //2x
    result.push_back(make_pair(0,1)); //1x
    result.push_back(make_pair(0,2)); //0x
    result.push_back(make_pair(0,3)); // 012
    break;

  case kIR2:
    result.push_back(make_pair(0,0)); //2
    result.push_back(make_pair(1,0)); //1x
    result.push_back(make_pair(2,0)); //0xxx
    result.push_back(make_pair(3,0)); // 012
    break;


  default:
    break;
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FigureType FigureFactory::getNextRandomType() {
  FigureType result = kQuad;

  if (randomSeq.empty()) {
    generateRandomSeq();
  }
  if (!randomSeq.empty()) {
    result = randomSeq.front();
    randomSeq.pop_front();
  }

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FigureFactory::generateRandomSeq() {
  const int availableTypesCount = 19;
  FigureType availableTypes[] = {
    kQuad,
    kZR1, kZR2, kZMirroredR1,  kZMirroredR2,
    kTR1, kTR2, kTR3, kTR4,
    kLR1, kLR2, kLR3, kLR4,
    kLMirroredR1, kLMirroredR2, kLMirroredR3, kLMirroredR4,
    kIR1, kIR2,
  };

  for(int i = 0; i<40; i++ ) {
    int rv = RandomHelper::random_int((int)0, availableTypesCount-1);
    randomSeq.push_back(availableTypes[rv]);

    C6_D4(c6, "Added ", availableTypes[rv], " for ", rv);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
