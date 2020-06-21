#pragma once

#include <vector>
#include "SixCatsLoggerLoggable.h"

namespace fifteen {

class BoardCombinationsFactory : virtual public SixCatsLoggerLoggable {
public:
  BoardCombinationsFactory();
  virtual ~BoardCombinationsFactory();


  std::vector<int> getRandomCombination();
protected:
  bool checkIfSolvable(const std::vector<int>& vec) const;

  int getInversionsCount(const std::vector<int>& vec) const;
};

}