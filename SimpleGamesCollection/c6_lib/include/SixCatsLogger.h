#pragma once

#include "SixCatsLoggerMacro.h"

#include <string>
#include <sstream> // std::ostringstream
#include <functional>

typedef std::function<std::string()> MessageMaker;

class SixCatsLogger {
public:

  enum LogLevel {
    Critical = 0,
    Warning,
    Info,
    Debug,
    Trace,
    Flood
  };

  SixCatsLogger();
  SixCatsLogger(LogLevel logLevel);
  virtual ~SixCatsLogger();

  inline bool warningEnabled() const {
    return (Warning <= logLevel);
  }

  inline bool infoEnabled() const {
    return (Info <= logLevel);
  }

  inline bool debugEnabled() const {
    return (Debug <= logLevel);
  }

  inline bool traceEnabled() const {
    return (Trace <= logLevel);
  }

  inline bool floodEnabled() const {
    return (Flood <= logLevel);
  }

  void setLogLevel(LogLevel logLevel);

  void c(const std::string& message);
  void c(const std::string& metaInfo, const std::string& message);
  void c(MessageMaker makeMessage);
  void c(const std::string& metaInfo, MessageMaker makeMessage);

  void w(const std::string& message);
  void w(const std::string& metaInfo, const std::string& message);
  void w(MessageMaker makeMessage);
  void w(const std::string& metaInfo, MessageMaker makeMessage);

  void i(const std::string& message);
  void i(const std::string& metaInfo, const std::string& message);
  void i(MessageMaker makeMessage);
  void i(const std::string& metaInfo, MessageMaker makeMessage);

  void d(const std::string& message);
  void d(const std::string& metaInfo, const std::string& message);
  void d(MessageMaker makeMessage);
  void d(const std::string& metaInfo, MessageMaker makeMessage);

  void t(const std::string& message);
  void t(const std::string& metaInfo, const std::string& message);
  void t(MessageMaker makeMessage);
  void t(const std::string& metaInfo, MessageMaker makeMessage);

  void f(const std::string& message);
  void f(const std::string& metaInfo, const std::string& message);
  void f(MessageMaker makeMessage);
  void f(const std::string& metaInfo, MessageMaker makeMessage);

protected:

  void selfSetup();

  LogLevel logLevel;

  //TODO: add & to str
  void        write(const LogLevel messageLevel, const std::string str);

  void        writeWithFunction(const LogLevel messageLevel, MessageMaker makeMessage);
  void        writeWithFunction(const LogLevel messageLevel, const std::string& metaInfo,
                                MessageMaker makeMessage);

  inline std::string composeLine(const std::string& metaInfo,
                                 const std::string& message) const {
    return metaInfo + " >> " + message;
  }
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// this came from http://stackoverflow.com/questions/1666802/is-there-a-class-macro-in-c
inline std::string c6_methodName(const std::string& prettyFunction) {
  size_t colons = prettyFunction.find("::");
  size_t begin  = prettyFunction.substr(0, colons).rfind(" ") + 1;
  size_t end    = prettyFunction.rfind("(") - begin;

  const std::string ss = prettyFunction.substr(begin, end) + "()";

  return ss;
}

#define __c6_MN__ c6_methodName(__PRETTY_FUNCTION__)
