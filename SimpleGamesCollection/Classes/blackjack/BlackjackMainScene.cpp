#include "blackjack/BlackjackMainScene.h"
#include "blackjack/BlackboardNode.h"
#include "blackjack/BlackjackCommonUtils.h"
#include "blackjack/GameTableNode.h"
#include "blackjack/GameStateKeeper.h"
#include "main_menu/BackgroundNode.h"
using namespace blackjack;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackjackMainScene::BlackjackMainScene() {
  ignoreHitStandButtons = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BlackjackMainScene::~BlackjackMainScene() {
  GameStateKeeper::unloadCardsFromCache();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
void BlackjackMainScene::continueGameUsual() {
  // C6_D1(c6, "Continue as usual");
  ignoreHitStandButtons = false;
  blackboardNode->setPlayerCount(gameStateKeeper->getPlayerCount());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::continueGameBust() {
  blackboardNode->setPlayerCount(gameStateKeeper->getPlayerCount());

  list<string> additionalDealerCards = gameStateKeeper->prepareFullDealer();
  for (const string cn: additionalDealerCards) {
    gameTableNode->dealToDealer(cn);
  }

  float delayValue = gameTableNode->revealFirst();

  DelayTime* dt = DelayTime::create(delayValue);
  CallFunc* cf = CallFunc::create(CC_CALLBACK_0(BlackjackMainScene::finishGameBust, this));
  Sequence* seq = Sequence::create(dt, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Scene* BlackjackMainScene::createScene(std::shared_ptr<SixCatsLogger> inC6) {

  BlackjackMainScene *pRet = new(std::nothrow) BlackjackMainScene();
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

void BlackjackMainScene::finishGameBust() {
  C6_D1(c6, "Continue as usual");
  const int dc = gameStateKeeper->getDealerCount();
  blackboardNode->setDealerCount(dc);

  const int pc = gameStateKeeper->getPlayerCount();
  blackboardNode->showGameResult(dc, pc);

  hitButton->setVisible(false);
  standButton->setVisible(false);
  replayButton->setVisible(true);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::init() {
  //////////////////////////////
  // 1. super init first
  if ( !Scene::init() ) {
    return false;
  }

  if (!initBackground()) {
    return false;
  }

  if (!initBoard()) {
    return false;
  }

  if (!initMenu()) {
    return false;
  }

  if (!initGameStateKeeper()) {
    C6_D1(c6, "Failed to create game state keeper");
    return false;
  }

  if (!initGameTable()) {
    C6_D1(c6, "Failed to create game table");
    return false;
  }

  if (!initKeyboardProcessing()) {
    C6_D1(c6, "Failed to init kb processing");
    return false;
  }

  startGame();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initBackground() {
  const string bFn = "menu/seamless-1657428_640.jpg";
  BackgroundNode* backgroundNode = BackgroundNode::create(getContentSize(), bFn, c6);
  if (backgroundNode == nullptr) {
    return false;
  }

  backgroundNode->setAnchorPoint(Vec2(0,0));
  backgroundNode->setPosition(0,0);
  addChild(backgroundNode, ZO_scene_background);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initBoard() {
  const Size cs = getContentSize();
  blackboardNode = BlackboardNode::create(c6);

  blackboardNode->setAnchorPoint(Vec2(1, 1));
  blackboardNode->setPosition(cs.width, cs.height);

  blackboardNode->setScale( (cs.width/3) / blackboardNode->getContentSize().width );

  addChild(blackboardNode, ZO_scene_elements);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initGameTable() {
  const Size cs = getContentSize();
  Size gtes;
  gtes.width = (cs.width/3)*2;
  gtes.height = (cs.height/3)*2;

  gameTableNode = GameTableNode::create(gtes, c6);

  gameTableNode->setAnchorPoint(Vec2(0.5,0.5));
  gameTableNode->setPosition(cs.width/3, (cs.height/3)*2);
  addChild(gameTableNode, ZO_cards);

  return true;  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initGameStateKeeper() {
  gameStateKeeper = make_unique<GameStateKeeper>();
  gameStateKeeper->setLogger(c6);

  if (!GameStateKeeper::loadCardsToCache()) {
    C6_D1(c6, "Failed to load: cards to cache");
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initKeyboardProcessing() {
  // keyboard processing
  EventListenerKeyboard* sceneKeyboardListener = EventListenerKeyboard::create();
  sceneKeyboardListener->onKeyPressed = CC_CALLBACK_2(BlackjackMainScene::onKeyPressedScene, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(sceneKeyboardListener, this);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool BlackjackMainScene::initMenu() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  Menu* menu = Menu::create();

  const int itemsCount = 2;
  string captions[itemsCount] = {"Hit", "Stand"};
  ccMenuCallback mcbs[itemsCount] = {CC_CALLBACK_1(BlackjackMainScene::mcHit, this),
                                     CC_CALLBACK_1(BlackjackMainScene::mcStand, this)
  };

  const Size currentWindowSize = getContentSize();
  Vec2 positions[itemsCount] = {
    {.x = currentWindowSize.width/8, .y = currentWindowSize.height/6},
    {.x = (currentWindowSize.width/8)*3, .y = currentWindowSize.height/6}
  };

  Node* resultButtons[itemsCount];

  for (int i = 0; i< itemsCount; i++) {
    MenuItemImage* item = MenuItemImage::create(
      "blackjack/menu_panel_short_main.png", "blackjack/menu_panel_short_sec.png", mcbs[i]);

    const Size itemSize = item->getContentSize();
    item->setAnchorPoint(Vec2(0.5,0.5));
    item->setPosition(positions[i]);

    Label* label = Label::createWithTTF(captions[i], "fonts/Marker Felt.ttf", 32);
    label->setTextColor(Color4B(160,82,45,255));
    label->setAnchorPoint(Vec2(0.5,0.5));
    label->setPosition(itemSize.width/2, itemSize.height/2);
    item->addChild(label);

    menu->addChild(item);

    resultButtons[i] = item;
  }

  hitButton = resultButtons[0];
  standButton = resultButtons[1];

  menu->addChild(prepareMainMenuButton());
  menu->addChild(prepareReplayButton());

  menu->setPosition(Vec2::ZERO);

  addChild(menu, ZO_scene_elements);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* BlackjackMainScene::prepareMainMenuButton() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  MenuItemImage* bmItem = MenuItemImage::create();
  bmItem->setNormalSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_main.png"));
  bmItem->setSelectedSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_sec.png"));
  bmItem->setCallback(CC_CALLBACK_1(BlackjackMainScene::mcBackToMain, this));

  const Size currentWindowSize = getContentSize();
  bmItem->setPosition( {.x = (currentWindowSize.width/4)*3, .y = currentWindowSize.height/6});

  const Size itemSize = bmItem->getContentSize();
  bmItem->setAnchorPoint(Vec2(0.5,0.5));

  Label* label = Label::createWithTTF("Back to main menu", "fonts/Marker Felt.ttf", 32);
  label->setTextColor(Color4B(160,82,45,255));
  label->setAnchorPoint(Vec2(0.5,0.5));
  label->setPosition(itemSize.width/2, itemSize.height/2);
  bmItem->addChild(label);

  return bmItem;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Node* BlackjackMainScene::prepareReplayButton() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();
  MenuItemImage* item = MenuItemImage::create();
  item->setNormalSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_main.png"));
  item->setSelectedSpriteFrame(sfc->getSpriteFrameByName("common_ui/panel_sec.png"));
  item->setCallback(CC_CALLBACK_1(BlackjackMainScene::mcReplay, this));

  // MenuItemImage* item = MenuItemImage::create(
  //   "blackjack/menu_panel_short_main.png", "blackjack/menu_panel_short_sec.png",
  //   CC_CALLBACK_1(BlackjackMainScene::mcReplay, this));

  const Size currentWindowSize = getContentSize();
  item->setPosition( {.x = (currentWindowSize.width/8)*2, .y = currentWindowSize.height/6});

  const Size itemSize = item->getContentSize();
  item->setAnchorPoint(Vec2(0.5,0.5));

  Label* label = Label::createWithTTF("Replay", "fonts/Marker Felt.ttf", 32);
  label->setTextColor(Color4B(160,82,45,255));
  label->setAnchorPoint(Vec2(0.5,0.5));
  label->setPosition(itemSize.width/2, itemSize.height/2);
  item->addChild(label);

  item->setVisible(false);
  replayButton = item;

  return item;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::onKeyPressedScene(EventKeyboard::KeyCode keyCode, Event *) {
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

void BlackjackMainScene::mcBackToMain(cocos2d::Ref *pSender) {
  Director::getInstance()->popScene();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::mcHit(cocos2d::Ref *pSender) {
  if (ignoreHitStandButtons) {
    C6_T1(c6, "Ignore mcHit");
    return;
  }

  ignoreHitStandButtons = true;

  string cardName = gameStateKeeper->hit();
  const float delayValue = gameTableNode->dealToPlayer(cardName);

  DelayTime* dt = DelayTime::create(delayValue);


  CallFunc* cf;
  if (gameStateKeeper->playerGotBust()) {
    C6_D1(c6, "player failed");
    cf = CallFunc::create(CC_CALLBACK_0(BlackjackMainScene::continueGameBust, this));
  }
  else {
    cf = CallFunc::create(CC_CALLBACK_0(BlackjackMainScene::continueGameUsual, this));
  }

  Sequence* seq = Sequence::create(dt, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::mcReplay(cocos2d::Ref *pSender) {
  hitButton->setVisible(true);
  standButton->setVisible(true);
  replayButton->setVisible(false);

  gameTableNode->reset();
  blackboardNode->reset();
  gameStateKeeper->reset();

  ignoreHitStandButtons = false;

  startGame();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::mcStand(cocos2d::Ref *pSender) {
  C6_D1(c6, "here");
  list<string> additionalDealerCards = gameStateKeeper->prepareFullDealer();
  for (const string cn: additionalDealerCards) {
    gameTableNode->dealToDealer(cn);
  }

  float delayValue = gameTableNode->revealFirst();

  DelayTime* dt = DelayTime::create(delayValue);
  CallFunc* cf = CallFunc::create(CC_CALLBACK_0(BlackjackMainScene::finishGameBust, this));
  Sequence* seq = Sequence::create(dt, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void BlackjackMainScene::startGame() {
  list<string> initialCards = gameStateKeeper->prepareInitialDeal();

  if (initialCards.size()<4) {
    C6_C1(c6, "bad logic");
    return;
  }

  gameTableNode->dealToDealer(initialCards.front());
  initialCards.pop_front();
  gameTableNode->dealToDealer(initialCards.front());
  initialCards.pop_front();

  gameTableNode->dealToPlayer(initialCards.front());
  initialCards.pop_front();
  gameTableNode->dealToPlayer(initialCards.front());
  initialCards.pop_front();

  blackboardNode->setPlayerCount(gameStateKeeper->getPlayerCount());
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
