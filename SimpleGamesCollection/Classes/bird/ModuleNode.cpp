#include "bird/ModuleNode.h"

#include "SixCatsLogger.h"
#include "SixCatsLoggerMacro.h"
#include <sstream>

USING_NS_CC;
using namespace std;

using namespace bird;
//static int objectIdGenerator = 0;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ModuleNode::ModuleNode() {
  //objectId = objectIdGenerator++;

}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ModuleNode::~ModuleNode() {
  // ntdh
  C6_D1(c6, "here");
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


ModuleNode* ModuleNode::create(const std::string& filename, std::shared_ptr<SixCatsLogger> inc6) {
  ModuleNode *result = new (std::nothrow) ModuleNode();

  if (result != nullptr) {
    result->setLogger(inc6);
  }


  if (result->initWithFile(filename)) {
    result->autorelease();
    return result;
  }
  //else
  CC_SAFE_DELETE(result);
  return nullptr;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .