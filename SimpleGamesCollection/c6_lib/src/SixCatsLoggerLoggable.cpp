#include "SixCatsLoggerLoggable.h"

#include "SixCatsLogger.h"

SixCatsLoggerLoggable::SixCatsLoggerLoggable() {
  //nothing to do here
}

SixCatsLoggerLoggable:: ~SixCatsLoggerLoggable() {
  // nothing to do here
}

void SixCatsLoggerLoggable::setLogger(std::shared_ptr<SixCatsLogger> c6new) {
  c6 = c6new;
}