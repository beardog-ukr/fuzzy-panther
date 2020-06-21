#pragma once

#include <memory>
#include <vector>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace fifteen {

// enum MoveType : int;
class BoardCombinationsFactory;
class TokenNode;

class FifteenGameNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static FifteenGameNode* create(std::shared_ptr<SixCatsLogger> c6);

  bool processClick(const int gameX, const int gameY);

  void resetForNewGame();

protected:
  FifteenGameNode();
  virtual ~FifteenGameNode();

  bool initSelf();
  bool initTokens();
  bool initGameOverMenu();

  bool checkIfSolved() const;
  bool gameSolved;

  int emptyPlaceX;
  int emptyPlaceY;

  std::vector<TokenNode*> tokenNodes;

  std::unique_ptr<BoardCombinationsFactory> combinationsFactory;
};

}
