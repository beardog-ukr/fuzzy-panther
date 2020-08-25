#include "sokoban/ActorNode.h"
using namespace sokoban;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

const int kTileSize = 48;
//const float terationDuration = 0.7;

static const struct {
  string north;
  string south;
  string east;
  string west;
} kMoveAnimationNames = {
  .north = "actor_move_north",
  .south = "actor_move_south",
  .east = "actor_move_east",
  .west = "actor_move_west"
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActorNode::ActorNode(const float id) {
  iterationDuration = id;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActorNode::~ActorNode() {
  C6_F1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Vec2 ActorNode::calculatePosition(const int x, const int y) const {
  Vec2 result = {.x = kTileSize/2, .y = kTileSize/2};
//  if ((x>=4) ||(x<0) ||(y>=4) ||(y<0) ) {
//    return result;
//  }
  //
  result.x = result.x + x*kTileSize;
  result.y = result.y + y*kTileSize;

  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActorNode* ActorNode::create(const float iterationDuration, shared_ptr<SixCatsLogger> c6) {

  ActorNode *pRet = new(std::nothrow) ActorNode(iterationDuration);
  if (pRet ==nullptr) {
    return nullptr;
  }

  pRet->setLogger(c6);

  if (!pRet->initSelf()) {
    delete pRet;
    pRet = nullptr;
    return nullptr;
  }

  pRet->autorelease();
  return pRet;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void ActorNode::doChangePositionTo(const int newGameX, const int newGameY) {
  C6_D1(c6, "here");

  Vec2 newPosition = calculatePosition(newGameX, newGameY);

  string an;
  if (newGameX == gameX) {
    if (newGameY > gameY) {
      an = kMoveAnimationNames.north;
    }
    else {
      an = kMoveAnimationNames.south;
    }
  }
  else {
    if (newGameX > gameX) {
      an = kMoveAnimationNames.east;
    }
    else {
      an = kMoveAnimationNames.west;
    }
  }
  Animation* animation = AnimationCache::getInstance()->getAnimation(an);
  animation->setDelayPerUnit(iterationDuration/animation->getTotalDelayUnits());
  Animate* animate = Animate::create(animation);

  CallFunc *cf = CallFunc::create([this]() {
    this->setSpriteFrame("sokoban/kfm/kfm_walk_south_00");
  });

  Sequence* seq = Sequence::create(animate, cf, nullptr);
  runAction(seq);

  MoveTo* ma = MoveTo::create(iterationDuration, newPosition);
  runAction(ma);

  gameX = newGameX;
  gameY = newGameY;

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool ActorNode::initSelf() {
  const string fn = "sokoban/kfm_walk_south_02.png";
  if (!initWithFile(fn)) {
    C6_C2(c6, "Failed to init with file ", fn);
    return false;    //
  }

//  const Size itemSize = getContentSize();
//  tileSize = itemSize.height;

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void ActorNode::setGamePosition(const int newGameX, const int newGameY) {
  gameX = newGameX;
  gameY = newGameY;

  setPosition(calculatePosition(gameX, gameY));
  //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void ActorNode::unloadAnimations() {
  AnimationCache* ac = AnimationCache::getInstance();
  ac->removeAnimation(kMoveAnimationNames.north);
  ac->removeAnimation(kMoveAnimationNames.south);
  ac->removeAnimation(kMoveAnimationNames.east);
  ac->removeAnimation(kMoveAnimationNames.west);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
