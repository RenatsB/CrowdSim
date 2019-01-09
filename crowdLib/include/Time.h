#ifndef CROWDLIB_TIME_H_
#define CROWDLIB_TIME_H_
#include <chrono>
class Time
{
public :
  Time()
  {
      m_pause=false;
      m_runClock=true;
  }
  ~Time()
  {
      m_runClock = false;
  }

  float GetTime();
  float DeltaTime();
  void LaunchTimer();
private:
  std::chrono::high_resolution_clock::time_point getCurrentTime();
  void tick();
  bool m_runClock = true;
  bool m_pause = false;
  std::chrono::high_resolution_clock m_clock;
  std::chrono::high_resolution_clock::time_point m_startTime;
  float m_prevTime;
  float m_deltaTime = 1.f;
  float m_time;
};
#endif //CROWDLIB_TIME_H_
