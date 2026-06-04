#pragma once


class PttSink
{
public:
  virtual ~PttSink() = default;
  virtual void ptt(bool on) = 0;
};
