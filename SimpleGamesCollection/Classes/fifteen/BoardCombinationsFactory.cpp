#include "fifteen/BoardCombinationsFactory.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "cocos2d.h"
USING_NS_CC;
using namespace std;

using namespace fifteen;


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BoardCombinationsFactory::BoardCombinationsFactory() {
//
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BoardCombinationsFactory::~BoardCombinationsFactory() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BoardCombinationsFactory::checkIfSolvable(const std::vector<int>& comboArr) const {
  int inversionsCount = getInversionsCount(comboArr);
  C6_D2(c6, "Found inversions: ", inversionsCount);

  bool icIsEven = (inversionsCount % 2 == 0);

  bool emptyIsOnEven = false;
  int rowSwitch = 0;
  for(int i = comboArr.size()-1; i>=0; i--) {
    if (comboArr[i]==0) {
      break;
    }
    rowSwitch++;
    if (rowSwitch>=4) {
      rowSwitch = 0;
      emptyIsOnEven = !emptyIsOnEven;
    }
  }

  c6->d(__c6_MN__, [emptyIsOnEven]() -> std::string {
    ostringstream ss;
    if (emptyIsOnEven) {
      ss<< "Empty is on even";
    }
    else {
      ss<< "Empty is on odd";
    }
    return ss.str();
  });

  bool result = emptyIsOnEven && (!icIsEven);
  result = (!emptyIsOnEven) && icIsEven;

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int BoardCombinationsFactory::getInversionsCount(const std::vector<int>& comboArr) const {
  int invCount = 0;

  for(int i = 0; i<comboArr.size(); i++) {
    if (comboArr[i]==0) {
      continue;
    }

    for(int j = i+1; j<comboArr.size(); j++) {
      if (comboArr[j]==0) {
        continue;
      }

      if (comboArr[i]>comboArr[j]) {
        invCount++;
      }
    }
  }

  return invCount;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

vector<int> BoardCombinationsFactory::getRandomCombination() {
  const int cellsCount = 16;
  bool values[cellsCount];


  vector<int> result = { 2, 1, 3, 4, 5, 6, 7, 8, 9,10,11,12, 13,14,0, 15};
  int inversionsCount = getInversionsCount(result);
  C6_D2(c6, "Found inversions: sss ", inversionsCount);

  result.resize(cellsCount);
  bool isSolvable = false;
  while (isSolvable==false) {
    for(int i = 0; i<cellsCount; i++) {
      values[i] = false;
    }

    for(int i = 0; i<cellsCount; i++) {
      int rv = RandomHelper::random_int((int)0, cellsCount-1);
      int j = rv;
      while(values[j] !=false) {
        j++;
        if (j>=cellsCount) {
          j = 0;
        }
      }

      values[j] = true;
      result[i] = j;
    }

    isSolvable = checkIfSolvable(result);//
    c6->d(__c6_MN__, [result]() -> std::string {
      ostringstream ss;
      ss << "Generated seq: ";
      for (int rv : result) {
        ss << rv << " ";
      }
      return ss.str();
    });
    if (!isSolvable) {
      C6_D1(c6, "is not solvable, generate one more");
    }
  }



  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

