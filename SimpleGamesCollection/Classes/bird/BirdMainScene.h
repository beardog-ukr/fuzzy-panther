#pragma once

#include <memory>
#include <vector>
#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"


namespace bird {

class MapSectionNode;
class WizardNode;

class BirdMainScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  static cocos2d::Scene* createScene(std::shared_ptr<SixCatsLogger> c6);

protected:
  BirdMainScene();
  virtual ~BirdMainScene();

  bool init();
  bool initBackground();
  bool initBackgroundBorder();
  bool initBackgroundGame();

  bool initKeyboardProcessing();
  bool initModules();

  WizardNode* wizardNode;
  bool initWizard();

  MapSectionNode* addNewMapSection(const cocos2d::Vec2& position);
  void startGame();
  void doOneTick();

  std::vector<cocos2d::Vec2> mapSectionPositions;
  std::vector<MapSectionNode*> mapSections;

  std::vector<int> obstacleLevelGeneratorArr;
  int nextObstacleLevelGeneratorIdx;
  int getNextObstacleLevel();

  cocos2d::Sprite* mapBackgroundSprite;
  void startMoveGameBackground();
  void resetGameBackgroundPosition();

  void stopBeforeGameOver();
  void resetAfterGameOver(const bool gameResult);

  bool onContactBegin(cocos2d::PhysicsContact& contact);
  void onKeyPressedScene(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *);

  int sectionsPassedCounter;
};

}

