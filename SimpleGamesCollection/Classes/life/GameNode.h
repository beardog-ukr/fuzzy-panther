#pragma once

#include <vector>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace life {

class CellNode;

class GameNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static GameNode* create(std::shared_ptr<SixCatsLogger> c6);

  // returns true if game is over
  bool processClick(const float clickX, const float clickY,
                    const cocos2d::EventMouse::MouseButton button);

  void applyChanges();

protected:
  GameNode();
  virtual ~GameNode();

  bool initSelf();
  bool initCellNodes();

  std::vector<CellNode*> cellNodes;
};

}
