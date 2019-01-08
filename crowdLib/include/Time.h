#ifndef CROWDLIB_TIME_H_
#define CROWDLIB_TIME_H_
#include <chrono>
class Time
{
public :
  Time()
  {
      LaunchTimer();
  }
  ~Time()=default;

  float GetTime();
  float DeltaTime();
private:
  void LaunchTimer();
  std::chrono::high_resolution_clock::time_point getCurrentTime();
  void tick();
  bool m_runClock;
  bool m_pause;
  std::chrono::high_resolution_clock m_clock;
  std::chrono::high_resolution_clock::time_point m_startTime;
  float m_prevTime;
  float m_deltaTime;
  float m_time;
};
#endif //CROWDLIB_TIME_H_
