#ifndef __MONKEYPROCESSOR_H__
#define __MONKEYPROCESSOR_H__

#include "SystemStatus.h"

class MonkeyProcessor
{
  SystemStatus _systemStatus;
  struct SystemStatus::BuildInfo PrevBuildInfo[SystemStatus::STATUS_COUNT];
  bool checkStatus(SystemStatus::BuildStatus status);
  void copyPrev();
  bool _firstGet = true;
  void (*logMsg)(const char *, ...);

public:
  MonkeyProcessor(void (*logMsg)(const char *, ...)) :  logMsg(logMsg), _systemStatus(logMsg)
  {
  }

  enum Action
  {
    Quiet,
    Chirp,
    Clap
  };

  bool initialize();

  Action process();
};

#endif
