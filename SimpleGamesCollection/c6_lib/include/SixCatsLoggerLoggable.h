#pragma once

#include <memory>
class SixCatsLogger;

class SixCatsLoggerLoggable {
public:
  SixCatsLoggerLoggable();
  virtual ~SixCatsLoggerLoggable();

  void setLogger(std::shared_ptr<SixCatsLogger> c6);

protected:
  std::shared_ptr<SixCatsLogger> c6;
};