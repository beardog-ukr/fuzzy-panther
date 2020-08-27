#include "sokoban/GameNode.h"
#include "sokoban/ActorNode.h"
#include "sokoban/BoxNode.h"
#include "sokoban/TargetNode.h"
#include "sokoban/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace sokoban;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

static const string kPlistFileName = "sokoban/movables.plist";
static const string kAnimationsPlistFileName = "sokoban/animations.plist";

const float kIterationDuration = 0.8;

static const struct {
  int box;
  int obstacle;
  int person;
  int target;
} kMapMetaCode = {
  .box = 10,
  .obstacle = 20,
  .person = 30,
  .target = 40
};

static const struct {
  string box;
  string obstacle;
  string person;
  string target;
} kMapMetaCodeName = {
  .box = "meta_box",
  .obstacle = "meta_obstacle",
  .person = "meta_person",
  .target = "meta_target"
};

const string kOpenTargetSpriteName = "sokoban/target/open";
const string kFilledTargetSpriteName = "sokoban/target/filled";

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::GameNode() {
  actor = nullptr;//
  actionInProcess = false;
  hasBufferedSomething = false;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::~GameNode() {
//
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode* GameNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  GameNode *result = new (std::nothrow) GameNode();

  if (result == nullptr) {
    return nullptr;
  }

  result->setLogger(inc6);

  if (!result->initSelf()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//void GameNode::doActorAttack(const int diffX, const int diffY) {
//  //
//}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::doMoveBox(const int boxX, const int boxY, const int diffX, const int diffY) {
  int newBoxX = boxX + diffX;
  int newBoxY = boxY+ diffY;

  for(const auto& obstacle: obstaclesInfo) {
    if ((obstacle.first == newBoxX)&&(obstacle.second == newBoxY)) {
      C6_D4(c6, "Obstacle for box at ", obstacle.first, ":", obstacle.second);
      return false;
    }
  }

  for(const auto& boxInfo: boxesInfo) {
    if ((boxInfo.gameX == newBoxX)&&(boxInfo.gameY == newBoxY)) {
      C6_D4(c6, "Another box at ", newBoxX, ":", newBoxY);
      return false;
    }
  }

  for(auto& boxInfo: boxesInfo) {
    if ((boxInfo.gameX == boxX)&&(boxInfo.gameY == boxY)) {
      boxInfo.gameX = newBoxX;
      boxInfo.gameY = newBoxY;

      boxInfo.node->doChangePositionTo(newBoxX, newBoxY);
      break;
    }
  }

  reevaluateTargets();

  return true;//box moved successfully
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initActorNode() {
  actor = ActorNode::create(kIterationDuration, c6);
  if (actor == nullptr) {
    return false;
  }

  mapNode->addChild(actor, kActorZOrder);

  actor->setGamePosition(personInfo.first, personInfo.second);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initBoxNodes() {
  for(auto& boxInfo: boxesInfo) {
    boxInfo.node = BoxNode::create(c6);
    if (boxInfo.node == nullptr) {
      return false;
    }
    boxInfo.node->setGamePosition(boxInfo.gameX, boxInfo.gameY);
    mapNode->addChild(boxInfo.node, kActorZOrder);
  }

  return true;
}


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initMapNode() {
  const char mapFilename[] = "sokoban/map01.tmx";

//  TMXTiledMap*
  mapNode = TMXTiledMap::create(mapFilename);
  if (mapNode == nullptr) {
    C6_D2(c6, "Error while loading: ", mapFilename);
    return false;
  }
  mapNode->setAnchorPoint(Vec2(0.5,0.5));
  addChild(mapNode, kGameBackgroundZOrder);


  //TODO: check boundary cases:
  // targets amount equal to boxes
  // one person
  // boxes, person, targets non-zero
  // all coordinates in range (already) and do not intersect

  // --- boxes
  std::list<std::pair<int, int> > tmpBoxesInfo;
  if (!loadMetaInfo(mapNode, kMapMetaCodeName.box, kMapMetaCode.box, tmpBoxesInfo)) {
    C6_D1(c6, "Failed to load boxes starting points");
    return false;
  }
  for (const auto& tmpbi: tmpBoxesInfo) {
    BoxInfo binfo;
    binfo.gameX = tmpbi.first;
    binfo.gameY = tmpbi.second;
    binfo.node = nullptr;
    boxesInfo.push_back(binfo);
  }

  // --- obstacles
  if (!loadMetaInfo(mapNode, kMapMetaCodeName.obstacle, kMapMetaCode.obstacle, obstaclesInfo)) {
    C6_D1(c6, "Failed to load obstacles");
    return false;
  }

  // --- person starting point
  std::list<std::pair<int, int> > tmpPersonInfo;
  if (!loadMetaInfo(mapNode, kMapMetaCodeName.person, kMapMetaCode.person, tmpPersonInfo)) {
    C6_D1(c6, "Failed to load person info");
    return false;
  }
  if (tmpPersonInfo.size() !=1) {
    C6_C1(c6, "No person info found");
    return false;
  }
  personInfo.first = tmpPersonInfo.front().first;
  personInfo.second = tmpPersonInfo.front().second;

  // --- targets
  std::list<std::pair<int, int> > tmpTargetsInfo;
  if (!loadMetaInfo(mapNode, kMapMetaCodeName.target, kMapMetaCode.target, tmpTargetsInfo)) {
    C6_D1(c6, "Failed to load targets");
    return false;
  }
  for (const auto& tmpti: tmpTargetsInfo) {
    TargetInfo tInfo;
    tInfo.gameX = tmpti.first;
    tInfo.gameY = tmpti.second;
    tInfo.node = nullptr;
    targetsInfo.push_back(tInfo);
  }

  // finally
  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initSelf() {
  if (!initMapNode()) {
    return false;
  }

  if (!initActorNode()) {
    return false;
  }

  if (!initBoxNodes()) {
    return false;
  }

  if (!initTargetNodes()) {
    return false;
  }


  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initTargetNodes() {
  for(auto& targetInfo: targetsInfo) {
    targetInfo.node = TargetNode::create(c6);
    if (targetInfo.node == nullptr) {
      return false;
    }

    targetInfo.node->setGamePosition(targetInfo.gameX, targetInfo.gameY);

    mapNode->addChild(targetInfo.node, kTargetZOrder);
  }

  reevaluateTargets();

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::loadMetaInfo(TMXTiledMap* const mapNode,
                            const string &layerName, const int metaCode,
                            list<pair<int, int> > &data) {

  data.clear();

  TMXLayer *const metaLayer = mapNode->getLayer(layerName);
  if (metaLayer == nullptr) {
    C6_D2(c6, "Failed to find layer ", layerName);
    return false;
  }

  const Size mapSize = mapNode->getMapSize();
  const int obstaclesMapWidth = mapSize.width;
  const int obstaclesMapHeight = mapSize.height;

  for (int tileX = 0; tileX < mapSize.width; tileX++) {
    for (int tileY = 0; tileY < mapSize.height; tileY++) {
      const int tileGid = metaLayer->getTileGIDAt(Vec2(tileX, tileY));
      const Value prop = mapNode->getPropertiesForGID(tileGid);

      if (prop.isNull()) {
        continue;
      }

      const ValueMap vm   = prop.asValueMap();
      const auto frez = vm.find("MetaCode");

      if (frez == vm.end()) {
        continue;
      }

      const int metaCodeValue = frez->second.asInt();
      if (metaCodeValue == metaCode ) {
        pair<int, int> dataElement;
        dataElement.first = tileX;
        dataElement.second = obstaclesMapHeight - tileY - 1;
        data.push_back(dataElement);
        C6_D6(c6, "found meta code ", metaCode, " at ", dataElement.first, ":", dataElement.second);
      }
    }
  }

  metaLayer->setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::loadSpriteCache(std::shared_ptr<SixCatsLogger> c6) {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(kPlistFileName);
  if (!sfc->isSpriteFramesWithFileLoaded(kPlistFileName)) {
    C6_C2(c6, "Failed to find ", kPlistFileName);
    return false;
  }

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(kAnimationsPlistFileName);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processKey(cocos2d::EventKeyboard::KeyCode keyCode) {
  C6_D2(c6, "here for ", (int)keyCode);

  if (actionInProcess) {
    C6_D1(c6, "Action ignored and buffered");
    hasBufferedSomething = true;
    bufferedKeyCode = keyCode;
    return;
  }

  int diffX = 0;
  int diffY = 0;

  if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
    diffY = -1;
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
    diffY = 1;
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
    diffX = -1;
  }
  else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
    diffX = 1;
  }

  if ((diffX!=0)||(diffY!=0)) {
    processMoveRequest(diffX, diffY);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processAttackEnd(float ) {
  actionInProcess = false;

  reevaluateTargets();

  if (hasBufferedSomething) {
    hasBufferedSomething = false;
    processKey(bufferedKeyCode);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processActionEnd(float ) {
  actionInProcess = false;

  for(auto& boxInfo: boxesInfo) {
    boxInfo.node->doAlignAgainst(personInfo.first, personInfo.second);
  }

  if (hasBufferedSomething) {
    hasBufferedSomething = false;
    processKey(bufferedKeyCode);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processMoveRequest(const int diffX, const int diffY) {
  pair<int, int> newPos;
  newPos.first = personInfo.first + diffX;
  newPos.second = personInfo.second + diffY;

  C6_D4(c6, "NewPos is ", newPos.first, ":", newPos.second);

  bool needToPush = false;
  for(const auto& boxInfo: boxesInfo) {
    if ((boxInfo.gameX == newPos.first)&&(boxInfo.gameY == newPos.second)) {
      C6_D4(c6, "Box at ", newPos.first, ":", newPos.second);
      needToPush = true;
      break;
    }
  }

  if (needToPush) {
    if (doMoveBox(newPos.first, newPos.second, diffX, diffY)) {
      actor->doAttackTo(newPos.first, newPos.second);
//      doActorAttack(diffX, diffY);
    }

    actionInProcess = true;
    schedule(CC_SCHEDULE_SELECTOR(GameNode::processAttackEnd), kIterationDuration, 0, 0);
    return;
  }

  bool prohibitMove = false;
  for(const auto& obstacle: obstaclesInfo) {
    if ((obstacle.first == newPos.first)&&(obstacle.second == newPos.second)) {
      C6_D4(c6, "Obstacle at ", newPos.first, ":", newPos.second);
      prohibitMove = true;
      break;
    }
  }

  if (prohibitMove) {
    return;
  }

  personInfo = newPos;
  actor->doChangePositionTo(personInfo.first, personInfo.second);

  actionInProcess = true;
  schedule(CC_SCHEDULE_SELECTOR(GameNode::processActionEnd), kIterationDuration, 0, 0);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::reevaluateTargets() {
  for(auto& targetInfo: targetsInfo) {
    bool stateIsFilled = false;
    for (const auto& boxInfo: boxesInfo) {
      if ((targetInfo.gameX==boxInfo.gameX)&&(targetInfo.gameY == boxInfo.gameY)) {
        stateIsFilled = true;
        break;
      }
    }

    targetInfo.node->setFilled(stateIsFilled);
  }
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::unloadSpriteCache() {
  ActorNode::unloadAnimations();
  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(kPlistFileName);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
