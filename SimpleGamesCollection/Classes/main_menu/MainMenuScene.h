#pragma once

#include "cocos2d.h"
#include "SixCatsLoggerLoggable.h"

class MainMenuScene : public cocos2d::Scene,
  virtual public SixCatsLoggerLoggable
{
public:
  virtual bool init();

  // implement the "static create()" method manually
  CREATE_FUNC(MainMenuScene);

protected:
  MainMenuScene();
  virtual ~MainMenuScene();

  bool initBackground();
  bool initMainMenu();
  bool initSettingsMenu();
  bool initSpriteCache();
  bool initNewGameMenu();

  void mcExit(cocos2d::Ref *pSender);
  void mcNewGame(cocos2d::Ref *pSender);
  void mcSettings(cocos2d::Ref *pSender);
  void mcSwitchToGame(cocos2d::Ref *pSender, const int menuCode);

  cocos2d::Menu* mainMenu;

  cocos2d::Sprite* settingsMenu; // TODO: implement real menu instead of sprite stub
  cocos2d::Node* newGameMenu;
  cocos2d::Node* currentSideMenu;
  void switchSideMenu(cocos2d::Node* newMenu);
};


