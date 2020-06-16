#pragma once

#include <memory>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

namespace blackjack {

class BlackboardNode;
class GameTableNode;
class GameStateKeeper;

class BlackjackMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable {
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  BlackjackMainScene();
  virtual ~BlackjackMainScene();

  bool init();
  bool initBackground();
  bool initBoard();
  bool initKeyboardProcessing();
  bool initMenu();
  bool initGameTable();
  bool initGameStateKeeper();

  void startGame();

  void continueGameUsual();
  void continueGameBust();
  void finishGameBust();

  BlackboardNode* blackboardNode;
  GameTableNode* gameTableNode;

  cocos2d::Node* prepareMainMenuButton();
  cocos2d::Node* prepareReplayButton();

  void mcBackToMain(cocos2d::Ref *pSender);
  void mcHit(cocos2d::Ref *pSender);
  void mcStand(cocos2d::Ref *pSender);
  void mcReplay(cocos2d::Ref *pSender);

  Node* replayButton;
  Node* hitButton;
  Node* standButton;


  bool ignoreHitStandButtons;

  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

  std::unique_ptr<GameStateKeeper> gameStateKeeper;
};

}

