#include "sokoban/GameNode.h"
#include "sokoban/ZOrderValues.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace sokoban;

// static const string kPlistFileName = "repeat/repeat.plist";
// static const string kAnimationsPlistFileName = "repeat/animations.plist";

// static const float kTimeWaitOneDigit = 3.0;
// static const float kTimeShowOneDigit = 3.0;
// static const float kTimeHideWhiteRocket = 1.0;

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


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

GameNode::GameNode() {
//
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



// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::initSelf() {
  const char mapFilename[] = "sokoban/map01.tmx";

  TMXTiledMap* mapNode = TMXTiledMap::create(mapFilename);
  if (mapNode == nullptr) {
    C6_D2(c6, "Error while loading: ", mapFilename);
    return false;
  }
  mapNode->setAnchorPoint(Vec2(0.5,0.5));
  addChild(mapNode, kGameBackgroundZOrder);

  if (!loadMetaInfo(mapNode, kMapMetaCodeName.box, kMapMetaCode.box, boxesInfo)) {
    C6_D1(c6, "Failed to load boxes starting points");
    return false;
  }

  if (!loadMetaInfo(mapNode, kMapMetaCodeName.obstacle, kMapMetaCode.obstacle, obstaclesInfo)) {
    C6_D1(c6, "Failed to load obstacles");
    return false;
  }

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

  if (!loadMetaInfo(mapNode, kMapMetaCodeName.target, kMapMetaCode.target, targetsInfo)) {
    C6_D1(c6, "Failed to load targets");
    return false;
  }

  //TODO: check boundary cases:
  // targets amount equal to boxes
  // one person
  // boxes, person, targets non-zero
  // all coordinates in range (already) and do not intersect

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



//      switch (metaCode) {
//      case MMC_OBSTACLE:
//        obstaclesMap[obstaclesMapWidth*(obstaclesMapHeight - tileY - 1) + tileX] = true;
//        break;

//      case MMC_MAGE_START:
//        mageStartX   = tileX;
//        mageStartY   = mapSize.height - tileY - 1;
//        break;

//      case MMC_KNIGHT_START:
//        knightStartX   = tileX;
//        knightStartY   = mapSize.height - tileY - 1;

//        currentKnightX = knightStartX;
//        currentKnightY = knightStartY;
//        break;

//        // Note there is no suitable default action here
//        // default:
//      }


    }
  }

  metaLayer->setVisible(false);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool GameNode::loadSpriteCache(std::shared_ptr<SixCatsLogger> c6) {
  // SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  // sfc->addSpriteFramesWithFile(kPlistFileName);
  // if (!sfc->isSpriteFramesWithFileLoaded(kPlistFileName)) {
  //   C6_C2(c6, "Failed to find ", kPlistFileName);
  //   return false;
  // }

  // AnimationCache * const ac = AnimationCache::getInstance();
  // ac->addAnimationsWithFile(kAnimationsPlistFileName);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::processKey(cocos2d::EventKeyboard::KeyCode keyCode) {


  // //check if key is correct
  // bool guessed = false;
  // if (keyCode == EventKeyboard::KeyCode::KEY_1) {
  //   guessed = (gameSeq[currentSeqElemIdx] == 1);
  // }
  // else if (keyCode == EventKeyboard::KeyCode::KEY_2) {
  //   guessed = (gameSeq[currentSeqElemIdx] == 2);
  // }
  // else if (keyCode == EventKeyboard::KeyCode::KEY_3) {
  //   guessed = (gameSeq[currentSeqElemIdx] == 3);
  // }
  // else if (keyCode == EventKeyboard::KeyCode::KEY_4) {
  //   guessed = (gameSeq[currentSeqElemIdx] == 4);
  // }

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void GameNode::unloadSpriteCache() {
  // AnimationCache::getInstance()->removeAnimation(DigitNode::kFireRingAnimationName);

  // SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(kPlistFileName);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
