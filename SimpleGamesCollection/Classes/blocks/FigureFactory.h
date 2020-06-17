#pragma once

//#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blocks {

class FigureInfo;

class FigureFactory final : virtual public SixCatsLoggerLoggable {
public:
  FigureFactory();
  ~FigureFactory();

  FigureInfo* composeNewFigure(const int initialRow);
};

}
