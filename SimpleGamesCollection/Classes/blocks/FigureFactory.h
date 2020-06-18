#pragma once

//#include "cocos2d.h"
#include <list>
#include <utility> // std::pair
#include <map>

#include "SixCatsLoggerLoggable.h"

namespace blocks {

class FigureInfo;
enum FigureType : int;

class FigureFactory final : virtual public SixCatsLoggerLoggable {
public:
  FigureFactory();
  ~FigureFactory();

  FigureInfo* composeNewFigure(const int initialRow);

protected:
  std::list<std::pair<int, int> > getNewFigureTemplate(const FigureType type );

  void generateRandomSeq();
  FigureType getNextRandomType();
  std::list<FigureType> randomSeq;

//  void prepareRotationVariants();
//  std::map<FigureType, FigureType> rotationVariants;
};

}
