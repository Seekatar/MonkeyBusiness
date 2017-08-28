#include "MonkeyProcessor.h"

bool MonkeyProcessor::initialize()
{
  for (int i = 0; i < SystemStatus::STATUS_COUNT; i++)
  {
    PrevBuildInfo[i].Status = SystemStatus::BuildStatus::BuildUnknown;
  }
  return _systemStatus.initialize();
}

bool MonkeyProcessor::checkStatus(SystemStatus::BuildStatus status)
{
  for (int i = 0; i < SystemStatus::STATUS_COUNT; i++)
  {
    if (_systemStatus.BuildInfo[i].Status == status)
    {
      int j = 0;
      for (; j < SystemStatus::STATUS_COUNT; j++)
      {
        if (strcmp(PrevBuildInfo[j].Id, _systemStatus.BuildInfo[i].Id) == 0 )
          return PrevBuildInfo[j].Status != status;
      }
      if (j == SystemStatus::STATUS_COUNT)
        return true;
    }
  }
  return false;
}

void MonkeyProcessor::copyPrev()
{
  for (int i = 0; i < SystemStatus::STATUS_COUNT; i++)
  {
    PrevBuildInfo[i] = _systemStatus.BuildInfo[i];
  }
}

MonkeyProcessor::Action MonkeyProcessor::process()
{
  Action ret = Quiet;
  if (_systemStatus.process())
  {
    if (!_firstGet)
    {
      if (checkStatus(SystemStatus::BuildStatus::Failure))
        ret = Clap;
      else if (checkStatus(SystemStatus::BuildStatus::Success))
        ret = Chirp;
    }
    _firstGet = false;
    copyPrev();
  }
  return ret;
}
