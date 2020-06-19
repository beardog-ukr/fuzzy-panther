#include "MainMenuScene.h"

#include "asteroids/AsteroidsScene.h"
#include "bird/BirdMainScene.h"
#include "blackjack/BlackjackMainScene.h"
#include "blocks/BlocksMainScene.h"
#include "eyes/EyesMainScene.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

#include "BackgroundNode.h"

USING_NS_CC;
using namespace std;

enum MenuCodeGame {
  kAsteroids = 10,
  kBird,
  kBlackjack,
  kBlocks,
  kEyes,
  kFifteen,
  kFlowers,
  kLife,
  kRepeat,
  kSnake,
  kSokoban
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

static const string plistFilename = "menu/menu.plist";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MainMenuScene::MainMenuScene() {
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

MainMenuScene::~MainMenuScene() {
  //
  C6_D1(c6, "here");
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(plistFilename);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// on "init" you need to initialize your instance
bool MainMenuScene::init() {
  if ( !Scene::init() ) {
    return false;
  }

  mainMenu = nullptr;
  currentSideMenu = nullptr;
  settingsMenu = nullptr;
  newGameMenu = nullptr;
  c6 = std::make_shared<SixCatsLogger>(SixCatsLogger::Debug);

  if (!initSpriteCache()) {
    return false;
  }

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
  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();

  Menu* menu = Menu::create();

  const int itemsCount = 3;
  string captions[itemsCount] = {"New Game", "Settings", "Exit"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(MainMenuScene::mcNewGame, this),
                                     CC_CALLBACK_1(MainMenuScene::mcSettings, this),
                                     CC_CALLBACK_1(MainMenuScene::mcExit, this)
  };

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create();
    item->setNormalSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_main.png"));
    item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_sec.png"));
    item->setCallback(mcbs[i]);

    // item->
    // "menu/panel_Example2.png", "menu/panel_Example1.png", mcbs[i]);

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
  const Size cs = getContentSize();

  newGameMenu = Node::create();
  newGameMenu->setAnchorPoint(Vec2(0.5, 0.5));
//  newGameMenu->setAnchorPoint(Vec2(0, 0));

  Size menuNodeSize = {.width = cs.width/2, .height = cs.height};
  newGameMenu->setContentSize(menuNodeSize);

  SpriteFrameCache* sfc = SpriteFrameCache::getInstance();
  Menu* menu = Menu::create();

  menu->setContentSize(Size(cs.width/2, cs.height));

  const int itemsCount = 10;
  string captions[itemsCount] = {"Asteroids", "Bird", "Blackjack", "Blocks", "Eyes",
                                 "Fifteen", "Flowers", "Repeat", "Snake", "Sokoban"
  };
  ccMenuCallback mcbs[itemsCount] = {
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kAsteroids),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kBird),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kBlackjack),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kBlocks),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kEyes),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kFifteen),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kFlowers),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kRepeat),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kSnake),
    CC_CALLBACK_1(MainMenuScene::mcSwitchToGame, this, kSokoban)
  };

  list<MenuItemImage*> ngMenuItems;

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create();
    item->setNormalSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_medium_main.png"));
    item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_medium_sec.png"));
    item->setCallback(mcbs[i]);
    // "menu/panel_Example2.png", "menu/panel_Example1.png", mcbs[i]);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    const Size itemSize = item->getContentSize();
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
    ngMenuItems.push_back(item);
  }

  menu->setPosition(Vec2::ZERO);
  newGameMenu->addChild(menu);



  float leftPos = menuNodeSize.width/4;//
  float rightPos = (menuNodeSize.width/4)*3;//
  float currentXPos = leftPos;


  float yPosStep = menuNodeSize.height / ((itemsCount/2) +1);
  float currentYPos = menuNodeSize.height - yPosStep;
  for (MenuItemImage* menuItem: ngMenuItems) {
    menuItem->setPosition(currentXPos, currentYPos);
    if (currentXPos==leftPos) {
      currentXPos = rightPos;
    }
    else {
      currentXPos = leftPos;
      currentYPos = currentYPos - yPosStep;
    }
  }


  addChild(newGameMenu, ZO_Side_Menu);


  newGameMenu->setPosition(Vec2(cs.width-cs.width/4, cs.height + cs.height/2));

//  menu->alignItemsVerticallyWithPadding(1);

//  newGameMenu = menu;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainMenuScene::initSettingsMenu() {
  const char filename[] = "menu/construction.png";

  Sprite* sprite = Sprite::create(filename);
  if (sprite == nullptr) {
    C6_C2(c6, "Error while loading file : ", filename);
    return false;
  }

  settingsMenu = sprite;

  const Size cs = getContentSize();
  settingsMenu->setPosition(Vec2(cs.width-cs.width/4, cs.height + cs.height/2));

  addChild(settingsMenu, ZO_Side_Menu);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool MainMenuScene::initSpriteCache() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(plistFilename);
  if (!sfc->isSpriteFramesWithFileLoaded(plistFilename)) {
    C6_C2(c6, "Error while loading: ", plistFilename);
    return false;
  }

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



  Scene* newScene = nullptr;
  if (menuCode == kAsteroids) {
    newScene = asteroids::AsteroidsScene::createScene();
  }
  else if (menuCode == kBird) {
    newScene = bird::BirdMainScene::createScene(c6);
  }
  else if (menuCode == kBlackjack) {
    newScene = blackjack::BlackjackMainScene::createScene(c6);
  }
  else if (menuCode == kBlocks) {
    newScene = blocks::BlocksMainScene::createScene(c6);
  }
  else if (menuCode == kEyes) {
    newScene = eyes::EyesMainScene::createScene(c6);
  }
  else {
    C6_C2(c6, "Bad Call ", menuCode);
  }

  if (newScene!= nullptr) {
    Director::getInstance()->pushScene(newScene);
  }
  else {
    C6_C1(c6, "Failed to create new scene");
  }
}
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
