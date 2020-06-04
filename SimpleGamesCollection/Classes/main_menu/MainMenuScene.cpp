#include "MainMenuScene.h"

#include "asteroids/AsteroidsScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "BackgroundNode.h"

USING_NS_CC;
using namespace std;

enum MenuCodeGame {
  MCG_Asteroids = 10,
  MCG_Bird
};

enum MenuCodeMain {
  MCM_New_game = 10,
  MCM_Settings,
  MCM_Exit
};

enum z_orders {
  ZO_Background = 10,
  ZO_Main_Menu,
  ZO_Side_Menu
};

static const float menuMoveDuration = 1.0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool MainMenuScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  mainMenu = nullptr;
  currentSideMenu = nullptr;
  settingsMenu = nullptr;
  newGameMenu = nullptr;
  c6 = std::make_shared<SixCatsLogger>(SixCatsLogger::Debug);

  if (!initBackground()) {
    return false;
  }

  if (!initMainMenu()) {
    return false;
  }

  if (!initSettingsMenu()) {
    return false;
  }

  if (!initNewGameMenu()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainMenuScene::initBackground() {
  const string filename = "menu/seamless-1657428_640.jpg";

  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), filename, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, ZO_Background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainMenuScene::initMainMenu() {
  Menu* menu = Menu::create();

  const int itemsCount = 3;
  string captions[itemsCount] = {"New Game", "Settings", "Exit"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(MainMenuScene::mcNewGame, this),
                                     CC_CALLBACK_1(MainMenuScene::mcSettings, this),
                                     CC_CALLBACK_1(MainMenuScene::mcExit, this)
  };

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create(
      "menu/panel_Example2.png", "menu/panel_Example1.png", mcbs[i]);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    const Size itemSize = item->getContentSize();
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }

  const Size cs = getContentSize();

  menu->setPosition(cs.width/2,cs.height/2);
  addChild(menu, ZO_Side_Menu);

  menu->alignItemsVertically();

  mainMenu = menu;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainMenuScene::initNewGameMenu() {
  Menu* menu = Menu::create();
  const Size cs = getContentSize();
  menu->setContentSize(Size(cs.width/2, cs.height));

  const int itemsCount = 5;
  string captions[itemsCount] = {"Asteroids", "Bird", "Blackjack", "Blocks", "Eyes"
                                 //, "Fifteen", "Flowers"
  };
  ccMenuCallback mcbs[itemsCount] = {
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Asteroids),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Bird),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Asteroids),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Asteroids),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Asteroids),
    // CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Asteroids),
    // CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, MCG_Asteroids)
  };

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create(
      "menu/panel_Example2.png", "menu/panel_Example1.png", mcbs[i]);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    const Size itemSize = item->getContentSize();
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }


  menu->setPosition(Vec2(cs.width-cs.width/4, cs.height + cs.height/2));
  addChild(menu, ZO_Side_Menu);

  menu->alignItemsVerticallyWithPadding(1);

  newGameMenu = menu;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainMenuScene::initSettingsMenu() {
  const char filename[] = "menu/construction.png";

  Sprite* sprite = Sprite::create(filename);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading: ", filename);
    return false;
  }

  settingsMenu = sprite;

  const Size cs = getContentSize();
  settingsMenu->setPosition(Vec2(cs.width-cs.width/4, cs.height + cs.height/2));

  addChild(settingsMenu, ZO_Side_Menu);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MainMenuScene::mcExit(Ref *pSender) {
  Director::getInstance()->end();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MainMenuScene::mcNewGame(Ref *pSender) {
  switchSideMenu(newGameMenu);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MainMenuScene::mcSettings(Ref *pSender) {
  C6_T1(c6, "here ");
  switchSideMenu(settingsMenu);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MainMenuScene::switchSideMenu(cocos2d::Node* newMenu) {
  if (newMenu == currentSideMenu) {
    return;
  }

  const Size cs = getContentSize();

  if (currentSideMenu == nullptr) {
    MoveTo* mt = MoveTo::create(menuMoveDuration, Vec2(cs.width/4, cs.height/2));
    mainMenu->runAction(mt);
  }
  else {
    //move current side menu to the right of the screen
    MoveTo* mt = MoveTo::create(menuMoveDuration, Vec2(cs.width + cs.width/4, cs.height/2));
    currentSideMenu->runAction(mt);
  }

  newMenu->stopAllActions();// to stop running if menu was called twice
  newMenu->setPosition(Vec2(cs.width-cs.width/4, cs.height + cs.height/2));
  MoveTo* mt = MoveTo::create(menuMoveDuration,
                              Vec2(cs.width-cs.width/4, cs.height/2));
  newMenu->runAction(mt);
  currentSideMenu = newMenu;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void MainMenuScene::mcSwitchToGame(cocos2d::Ref *pSender, const int menuCode) {
  C6_D2(c6, "here ", menuCode);

  Scene* newScene = AsteroidsScene::create();

  Director::getInstance()->pushScene(newScene);
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
