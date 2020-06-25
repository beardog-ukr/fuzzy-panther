#pragma once

#include <vector>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace life {

enum ChangeType : int;

class CellNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:

  static CellNode* create(std::shared_ptr<SixCatsLogger> c6);

  static const int kCellSide;
  static const float kCellSideF;

  static bool loadSpriteCache(std::shared_ptr<SixCatsLogger> c6);
  static void unloadSpriteCache();

  void markAlive();
  void markKill();

  void applyChanges();

  void addNeighborNode(CellNode* node);

  void prepareNextChange();

  bool checkIsAlive() const;

protected:
  CellNode();
  virtual ~CellNode();

  std::string generateRandomFrameName() const;

  void doDie();
  void doResurrect();

  cocos2d::Sprite* markNode;
  cocos2d::Sprite* creatureNode;
  cocos2d::Action* creatureAction;

  ChangeType nextChange;
  bool nextChangeIsProtected;

  CellNode* neighborNodes[8];
  int neighborsCount;

  bool isAlive;

  bool initMarkNode();
  bool initSelf();
};

}
