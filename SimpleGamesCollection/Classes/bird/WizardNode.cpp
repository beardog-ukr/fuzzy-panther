#include "bird/WizardNode.h"
#include "bird/CommonUtils.h"
using namespace bird;

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

static const float downStepDuration = 0.5; // time to go down for one step
static const float upStepDuration = 0.5; //
static const int stepSize = 32;

static const string framesPlistFN = "bird/wizard.plist";
static const string animationsPlistFN = "bird/wizard_animations.plist";

static const string animationNameUp = "wizard_up";
static const string animationNameDie = "wizard_die";
static const string animationNameDown = "wizard_idle";

enum ActionTagsWizardNode {
  WNAT_vertical_move,
  WNAT_up_anim
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

WizardNode::WizardNode() {
  //ntdh
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

WizardNode::~WizardNode() {
  // ntdh
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


WizardNode* WizardNode::create(std::shared_ptr<SixCatsLogger> inc6) {
  WizardNode *result = new (std::nothrow) WizardNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }

  if (!result->init()) {
    CC_SAFE_DELETE(result);
    return nullptr;
  }

  result->autorelease();
  return result;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::doDie(cocos2d::CallFunc *cf ) {
  stopAllActionsByTag(WNAT_vertical_move);

  Animation* animation = AnimationCache::getInstance()->getAnimation(animationNameDie);
  Animate* animate = Animate::create(animation);

  Sequence* seq = Sequence::create(animate, cf, nullptr);
  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::doGoDown() {
  stopAllActionsByTag(WNAT_vertical_move);

  ActionInterval* action = prepareDownAction();

  runAction(action);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::doGoUp() {
  stopAllActionsByTag(WNAT_vertical_move);

  ActionInterval* actionDown = prepareDownAction();

  Animation* animationDown = AnimationCache::getInstance()->getAnimation(animationNameDown);
  Animate* animateDown = Animate::create(animationDown);
  animateDown->setDuration(actionDown->getDuration());
  Spawn* spawnDown = Spawn::createWithTwoActions(animateDown, actionDown);

  ActionInterval* actionUp = prepareUpAction();

  Animation* animationUp = AnimationCache::getInstance()->getAnimation(animationNameUp);
  Animate* animateUp = Animate::create(animationUp);
  animateUp->setDuration(actionUp->getDuration());
  Spawn* spawnUp = Spawn::createWithTwoActions(animateUp, actionUp);

  Sequence* seq = Sequence::create(spawnUp, spawnDown, nullptr);
  seq->setTag(WNAT_vertical_move);

  runAction(seq);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WizardNode::init() {
  initWithSpriteFrameName("wizard_idle_00.png");

  if (!initPhysicsBody()) {
    return false;
  }

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WizardNode::initPhysicsBody() {
  Vec2 points[8] = {
    {.x =  0, .y = 25},
    {.x = 11, .y = 20},
    {.x = 20, .y = 0},
    {.x = 15, .y = -15},
    {.x = 0, .y = -20},
    {.x = -15,.y = -15},
    {.x = -20,.y =  0},
    {.x = -15,.y = 20}
  };

  PhysicsBody* physicsBody = PhysicsBody::createPolygon(&(points[0]), 8,
                                                        PhysicsMaterial(0.1f, 1.0f, 0.0f));
  physicsBody->setDynamic(false);
  physicsBody->setCategoryBitmask(CM_wizard);
  physicsBody->setCollisionBitmask(CM_cliff);
  physicsBody->setContactTestBitmask(0xFFFFFFFF);

  addComponent(physicsBody);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WizardNode::loadAnimations() {
  SpriteFrameCache* const sfc = SpriteFrameCache::getInstance();

  sfc->addSpriteFramesWithFile(framesPlistFN);
  if (!sfc->isSpriteFramesWithFileLoaded(framesPlistFN)) {
    return false;
  }

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->addAnimationsWithFile(animationsPlistFN);

  return true;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::setLowPoint(const Vec2& lp) {
  lowPoint = lp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void WizardNode::start() {
  doGoDown();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActionInterval* WizardNode::prepareDownAction() const {
  const Vec2 cp = getPosition();
  C6_D4(c6, "Will go from ", cp.x, ":", cp.y);
  C6_D4(c6, "Will go to   ", lowPoint.x, ":", lowPoint.y);
  const float duration = (fabs(cp.y + stepSize - lowPoint.y)/stepSize)*downStepDuration;
  C6_D3(c6, "Will go for ", duration, " seconds");

  MoveTo* mta = MoveTo::create(duration, lowPoint);//
  mta->setTag(WNAT_vertical_move);
  EaseIn* ea = EaseIn::create(mta, 3.0f);
  ea->setTag(WNAT_vertical_move);

  return ea;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ActionInterval* WizardNode::prepareUpAction() const {
  Vec2 newPosition = getPosition();
  newPosition.y = newPosition.y + stepSize;

  MoveTo* mta = MoveTo::create(upStepDuration, newPosition);//
  // mta->setTag(WNAT_vertical_move);
  EaseIn* ea = EaseIn::create(mta, 3.0f);
  // ea->setTag(WNAT_vertical_move);

  return ea;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool WizardNode::unloadAnimations() {

  AnimationCache * const ac = AnimationCache::getInstance();
  ac->removeAnimation(animationNameUp);
  ac->removeAnimation(animationNameDown);
  ac->removeAnimation(animationNameDie);

  SpriteFrameCache::getInstance()->removeSpriteFramesFromFile(framesPlistFN);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .