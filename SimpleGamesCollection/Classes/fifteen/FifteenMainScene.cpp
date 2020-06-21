#include "fifteen/FifteenMainScene.h"

#include "fifteen/BoardCombinationsFactory.h"
#include "fifteen/FifteenGameNode.h"
#include "fifteen/ZOrderValues.h"
#include "main_menu/BackgroundNode.h"

using namespace fifteen;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FifteenMainScene::FifteenMainScene() {
  gameSolved = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

FifteenMainScene::~FifteenMainScene() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* FifteenMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  FifteenMainScene *pRet = new(std::nothrow) FifteenMainScene();
  if (pRet == nullptr) {
    return nullptr;
  }

  pRet->setLogger(inC6);

  if (pRet && pRet->init()) {
    pRet->autorelease();
    return pRet;
  }
  else {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initGameNode()) {
    return false;
  }

  if (!initGameOverMenu()) {
    return false;
  }

  if (!initGameOverImage()) {
    return false;
  }

  if (!initMouseProcessing()) {
    return false;
  }

  if (!initKeyboardProcessing()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::initBackground() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, kSceneBackgroundZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::initGameNode() {
  gameNode = FifteenGameNode::create(c6);

  gameNode->setAnchorPoint(Vec2(0.5, 0.5));

  const Size cs = getContentSize();
  float desiredGameNodeHeight = cs.height*0.9;

  // game node height is same as width, it's square
  gnWestBorder = cs.width/2 - desiredGameNodeHeight/2;
  gnEastBorder = cs.width/2 + desiredGameNodeHeight/2;
  gnSouthBorder = cs.height/2 - desiredGameNodeHeight/2;
  gnNorthBorder = cs.height/2 + desiredGameNodeHeight/2;

  gnSize = desiredGameNodeHeight;

  const float gnScale = desiredGameNodeHeight / gameNode->getContentSize().height;
  gameNode->setScale(gnScale);

  gameNode->setPosition(cs.width/2, cs.height/2);
  addChild(gameNode, kGameBackgroundZOrder);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::initGameOverImage() {
  const string fn = "fifteen/victoryImage.png";
  victoryImage = Sprite::create(fn);
  if (victoryImage == nullptr) {
    return false;
  }

  const Size cs = getContentSize();
  victoryImage->setPosition(cs.width/2, cs.height/2);

  addChild(victoryImage, kGameOverMenuZOrder);

  victoryImage->setVisible(false);

  return true;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::initGameOverMenu() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  Menu* menu = Menu::create();

  const int itemsCount = 2;
  string captions[itemsCount] = {"Back to main menu", "Try again"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(FifteenMainScene::mcBackToMain, this),
                                     CC_CALLBACK_1(FifteenMainScene::mcTryAgain, this)
  };

  const Size currentWindowSize = getContentSize();

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create();
    item->setNormalSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_main.png"));
    item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_sec.png"));
    item->setCallback(mcbs[i]);

    const Size itemSize = item->getContentSize();
    C6_D4(c6, "One item size is ", itemSize.width, "x", itemSize.height);
    Vec2 ip;
    ip.x = itemSize.width + itemSize.height/2;
    ip.x = currentWindowSize.width - (i+1)*ip.x;
    ip.x = ip.x + itemSize.width/2;
    ip.y = itemSize.height;

    C6_D4(c6, "Will put menu item at ", ip.x, ":", ip.y);

    item->setAnchorPoint(Vec2(0.5,0.5));
    item->setPosition(ip);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);
  }

  menu->setPosition(Vec2::ZERO);

  addChild(menu, kGameOverMenuZOrder);

  menu->setVisible(false);

  gameOverMenu = menu;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(FifteenMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool FifteenMainScene::initMouseProcessing() {
  EventListenerMouse* sceneMouseListener = EventListenerMouse::create();
  sceneMouseListener->onMouseDown = CC_CALLBACK_1(FifteenMainScene::onMouseDown, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneMouseListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FifteenMainScene::mcBackToMain(cocos2d::Ref *pSender) {
  Director::getInstance()->popToRootScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FifteenMainScene::mcTryAgain(cocos2d::Ref *pSender) {
  gameNode->resetForNewGame();
  gameOverMenu->setVisible(false);
  victoryImage->setVisible(false);
  gameSolved = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FifteenMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
  C6_D3(c6, "Key '", (int)keyCode, "' was pressed");

  if (EventKeyboard::KeyCode::KEY_BACKSPACE == keyCode) {
    Director::getInstance()->popScene();
  }
  else if (EventKeyboard::KeyCode::KEY_X == keyCode) {
    c6->d(__c6_MN__, "Need to get out.");

    // Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void FifteenMainScene::onMouseDown(Event *event) {
  if (gameSolved) {
    return;
  }

  EventMouse* e = (EventMouse*)event;

  float xPos = e->getCursorX();
  float yPos = e->getCursorY();
  C6_D4(c6,"Mouse Down detected, Position ", xPos, ":", yPos);

  if ((xPos < gnWestBorder) ||(xPos > gnEastBorder) ||
      (yPos > gnNorthBorder) || (yPos<gnSouthBorder)) {
    C6_D1(c6,"Ignore, it's outside widget");
    return;
  }

  float tmpf;
  modf (4*((xPos - gnWestBorder)/gnSize), &tmpf);
  int gameTileX = (int) tmpf;

  modf (4*((yPos - gnSouthBorder)/gnSize), &tmpf);
  int gameTileY = (int) tmpf;

  C6_D4(c6,"Tile is supposed to be ", gameTileX, ":", gameTileY);
  gameSolved = gameNode->processClick(gameTileX, gameTileY);
  if (gameSolved) {
    gameOverMenu->setVisible(true);
    victoryImage->setVisible(true);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
