#include "SixCatsLogger.h"

#include <iostream> // cout
using namespace std;

//#define LOCAL_C6_DEBUG 1

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SixCatsLogger::SixCatsLogger() {
  selfSetup();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SixCatsLogger::SixCatsLogger(LogLevel inLogLevel) {
  selfSetup();
  logLevel = inLogLevel;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

SixCatsLogger::~SixCatsLogger() {
  #ifdef LOCAL_C6_DEBUG
  cout << "SixCatsLogger obj destroyed" << endl;
  #endif // ifdef LOCAL_C6_DEBUG
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::c(const string& message) {
  write(Critical, message);
}

void SixCatsLogger::c(const string& metaInfo, const string& message) {
  write(Critical, composeLine(metaInfo, message));
}

void SixCatsLogger::c(MessageMaker makeMessage) {
  writeWithFunction(Critical, makeMessage);
}

void SixCatsLogger::c(const string & metaInfo, MessageMaker makeMessage) {
  writeWithFunction(Critical, metaInfo, makeMessage);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::d(const string& message) {
  write(Debug, message);
}

void SixCatsLogger::d(const string& metaInfo, const string& message) {
  write(Debug, composeLine(metaInfo, message));
}

void SixCatsLogger::d(MessageMaker makeMessage) {
  writeWithFunction(Debug, makeMessage);
}

void SixCatsLogger::d(const string& metaInfo, MessageMaker makeMessage) {
  writeWithFunction(Debug, metaInfo, makeMessage);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::f(const string& message) {
  write(Flood, message);
}

void SixCatsLogger::f(const string& metaInfo, const string& message) {
  write(Flood, composeLine(metaInfo, message));
}

void SixCatsLogger::f(MessageMaker makeMessage) {
  writeWithFunction(Flood, makeMessage);
}

void SixCatsLogger::f(const string& metaInfo, MessageMaker makeMessage) {
  writeWithFunction(Flood, metaInfo, makeMessage);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::i(const string& message) {
  write(Info, message);
}

void SixCatsLogger::i(const string& metaInfo, const string& message) {
  write(Info, composeLine(metaInfo, message));
}

void SixCatsLogger::i(MessageMaker makeMessage) {
  writeWithFunction(Info, makeMessage);
}

void SixCatsLogger::i(const string& metaInfo, MessageMaker makeMessage) {
  writeWithFunction(Info, metaInfo, makeMessage);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::t(const string& message) {
  write(Trace, message);
}

void SixCatsLogger::t(const string& metaInfo, const string& message) {
  write(Trace, composeLine(metaInfo, message));
}

void SixCatsLogger::t(MessageMaker makeMessage) {
  writeWithFunction(Trace, makeMessage);
}

void SixCatsLogger::t(const string& metaInfo, MessageMaker makeMessage) {
  writeWithFunction(Trace, metaInfo, makeMessage);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::selfSetup() {
  setLogLevel(SixCatsLogger::Critical);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::setLogLevel(LogLevel inLogLevel) {
  logLevel = inLogLevel; //
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::w(const string& message) {
  write(Warning, message);
}

void SixCatsLogger::w(const string& metaInfo, const string& message) {
  write(Warning, composeLine(metaInfo, message));
}

void SixCatsLogger::w(MessageMaker makeMessage) {
  writeWithFunction(Warning, makeMessage);
}

void SixCatsLogger::w(const string& metaInfo, MessageMaker makeMessage) {
  writeWithFunction(Warning, metaInfo, makeMessage);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void SixCatsLogger::write(LogLevel messageLevel, const string str) {
  if (messageLevel > logLevel) {
    return;
  }

  cout << str << endl;
}

void SixCatsLogger::writeWithFunction(LogLevel messageLevel,
                                      std::function<std::string()>makeMessage) {
  if (messageLevel > logLevel) {
    return;
  }

  write(messageLevel, makeMessage());
}

void SixCatsLogger::writeWithFunction(LogLevel messageLevel,
                                      const string                          & metaInfo,
                                      std::function<std::string()>makeMessage) {
  if (messageLevel > logLevel) {
    return;
  }

  write(messageLevel, composeLine(metaInfo, makeMessage()));
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .