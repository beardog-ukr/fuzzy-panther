#include "snake/BodyPartsFactory.h"
#include "snake/DirectionType.h"
#include "snake/GameNode.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace snake;

static const int kMoveDuration = 2.0;

static const struct {
  string barrelIdle;
  string barrelDie;
  string elementMove;
  string elementMoveVertical;
} kAnimationName = {
  .barrelIdle = "barrel_idle",
  .barrelDie = "barrel_die",
  .elementMove = "element_move",
  .elementMoveVertical = "element_move_vertical"
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BodyPartsFactory::BodyPartsFactory() {
//
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

BodyPartsFactory::~BodyPartsFactory() {

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Sprite* BodyPartsFactory::createBarrel(const int x, const int y) const {
  const string tmpfn = "snake/barrel/barrel_a.png";  // "snake/wizard/wizard_fly_01.png";
  // ;
  Sprite* sp = Sprite::createWithSpriteFrameName(tmpfn);
  if (sp == nullptr) {
    C6_D2(c6, "Failed to create ", tmpfn );
    return nullptr;
  }

  Vec2 pos;
  pos.x = x*GameNode::kCellSize + GameNode::kCellSize/2;
  pos.y = y*GameNode::kCellSize + GameNode::kCellSize/2;
  sp->setPosition(pos);


  Animation* animation = AnimationCache::getInstance()->getAnimation(kAnimationName.barrelIdle);
  if (animation == nullptr) {
    C6_D2(c6, "Failed to find ", kAnimationName.barrelIdle);
    return sp;
  }
  Animate* animate = Animate::create(animation);
  RepeatForever* ra =  RepeatForever::create(animate);
  sp->runAction(ra);

  return sp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Sprite* BodyPartsFactory::createBarrelExplode(const int x, const int y) const {
  const string tmpfn = "snake/barrel/barrel_a.png";  //
  // ;
  Sprite* sp = Sprite::createWithSpriteFrameName(tmpfn);
  if (sp == nullptr) {
    C6_D2(c6, "Failed to create ", tmpfn );
    return nullptr;
  }

  Vec2 pos;
  pos.x = x*GameNode::kCellSize + GameNode::kCellSize/2;
  pos.y = y*GameNode::kCellSize + GameNode::kCellSize/2;
  sp->setPosition(pos);

  Animation* animation = AnimationCache::getInstance()->getAnimation(kAnimationName.barrelDie);
  if (animation == nullptr) {
    C6_D2(c6, "Failed to find ", kAnimationName.barrelDie);
    return sp;
  }
  Animate* animate = Animate::create(animation);
//  RepeatForever* ra =  RepeatForever::create(animate);
  sp->runAction(animate);

  return sp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Sprite* BodyPartsFactory::createIdlePart(const SnakeElementInfo& partInfo) {
  const string tmpfn = "snake/wizard/wizard_fly_01.png";
  // ;
  Sprite* sp = Sprite::createWithSpriteFrameName(tmpfn);
  if (sp == nullptr) {
    C6_D2(c6, "Failed to create ", tmpfn );
    return nullptr;
  }
  Vec2 pos;
  pos.x = partInfo.targetX*GameNode::kCellSize + GameNode::kCellSize/2;
  pos.y = partInfo.targetY*GameNode::kCellSize + GameNode::kCellSize/2;

  sp->setPosition(pos);

  return sp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

Sprite* BodyPartsFactory::createMovingPart(const SnakeElementInfo& partInfo) {
  const string tmpfn = "snake/wizard/wizard_death_02.png";//
  // ;
  Sprite* sp = Sprite::createWithSpriteFrameName(tmpfn);
  if (sp == nullptr) {
    C6_D2(c6, "Failed to create ", tmpfn );
    return nullptr;
  }
  Vec2 pos;
  pos.x = partInfo.targetX*GameNode::kCellSize + GameNode::kCellSize/2;
  pos.y = partInfo.targetY*GameNode::kCellSize + GameNode::kCellSize/2;

  Vec2 previousPos;
  previousPos.x = partInfo.previousX*GameNode::kCellSize + GameNode::kCellSize/2;
  previousPos.y = partInfo.previousY*GameNode::kCellSize + GameNode::kCellSize/2;

  sp->setPosition(previousPos);

  string animName = kAnimationName.elementMove;

  // mirror sprite if it moves east
  if (previousPos.x > pos.x) {
    sp->setScaleX(-1);
  }
  else if (previousPos.x == pos.x) {
    // use
    animName = kAnimationName.elementMoveVertical;
  }

  MoveTo* mt = MoveTo::create(kMoveDuration, pos);
  sp->runAction(mt);

  Animation* animation = AnimationCache::getInstance()->getAnimation(animName);
  if (animation == nullptr) {
    C6_D2(c6, "Failed to find ", animName);
    return sp;
  }
  animation->setDelayPerUnit(kMoveDuration/animation->getTotalDelayUnits());
  Animate* animate = Animate::create(animation);
  sp->runAction(animate);

  return sp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//string BodyPartsFactory::getAnimationNameForType(const DirectionType type) const {
//  string afn = "";
//  switch(type) {
//  case kRegularWestEast:
//    afn = kAnimationNameWE;
//    break;
//  case kRegularNorthEast:
//    afn = kAnimationNameNE;
//    break;
//  case kRegularEastSouth:
//    afn = kAnimationNameSE;
//    break;

//  }



//  return afn;
//}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

