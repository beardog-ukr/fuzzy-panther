#pragma once

#include <memory>
#include <vector>

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace repeat {

// enum MoveType : int;
//class BoardCombinationsFactory;
class DigitNode;
// class GameStateKeeper;

class GameNode : public cocos2d::Sprite, virtual public SixCatsLoggerLoggable {
public:
  static GameNode* create(std::shared_ptr<SixCatsLogger> c6);

  static bool loadSpriteCache(std::shared_ptr<SixCatsLogger> c6);
  static void unloadSpriteCache();

  typedef std::function<void (bool)> SceneCallback;
  void startNewGame(SceneCallback sceneCallback);
  bool getGameResult() const;
  void processKey(cocos2d::EventKeyboard::KeyCode keyCode);

protected:
  GameNode();
  virtual ~GameNode();

  static const int kKeysCount;
  std::vector<DigitNode*> digitNodes;

  bool gameFailed;

  std::vector<int> gameSeq;
  int seqElementsCount;
  int currentSeqElemIdx;
  int currentIteration;
  int iterationsCount;
  void prepareGameSeq();

  cocos2d::Sprite* whiteRocket;
  void launchWhiteRocket();

  void showNextDigit();
  void startWaiting();
  bool isWaitingForKey;
  void processWaitingOver();

  SceneCallback sceneCallback;

  void launchShow();
  void doNextIteration();

  bool initSelf();
  bool initDigitNodes();
  bool initWhiteRocket();
};

}
