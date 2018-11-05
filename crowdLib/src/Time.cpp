#include "Time.h"
using FpMilliseconds =
        std::chrono::duration<float, std::chrono::milliseconds::period>;

std::chrono::high_resolution_clock::time_point Time::getCurrentTime()
{
  return m_clock.now();
}

void Time::LaunchTimer()
{
  static_assert(std::chrono::treat_as_floating_point<FpMilliseconds::rep>::value,
                "Rep required to be floating point");
  m_startTime = m_clock.now();
  m_time = 0.f;
  m_deltaTime = 0.f;
  m_prevTime = 0.f;
  while(m_runClock)
    {
      if(!m_pause)
        tick();
    }
}

void Time::tick()
{
  std::chrono::high_resolution_clock::time_point now = getCurrentTime();
  m_time = FpMilliseconds(now-m_startTime).count();
  m_deltaTime = m_prevTime - m_time;
  m_prevTime = m_prevTime - m_time;
}
