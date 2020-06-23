#pragma once

#include <memory>
#include <vector>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace flowers {

// enum MoveType : int;
//class BoardCombinationsFactory;
class TileNode;
class GameStateKeeper;

class FlowersGameNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static FlowersGameNode* create(std::shared_ptr<SixCatsLogger> c6);

  // returns true if game is over
  bool processClick(const float clickX, const float clickY,
                    const cocos2d::EventMouse::MouseButton button);

  bool getGameResult() const;

  void resetForNewGame();

protected:
  FlowersGameNode();
  virtual ~FlowersGameNode();

  bool initSelf();
  bool initGameStateKeeper();
  bool initTileNodes();

  bool processLeftClick(const int x, const int y);
  void processRightClick(const int x, const int y);

//  bool gameSolved;

  std::unique_ptr<GameStateKeeper> gameStateKeeper;

};

}
