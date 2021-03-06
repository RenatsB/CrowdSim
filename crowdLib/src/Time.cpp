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
  m_deltaTime = 1.f;
  m_prevTime = 0.f;
  /*while(m_runClock)
    {
      if(!m_pause)
        tick();
    }*/
  tick();
}

void Time::tick()
{
  std::chrono::high_resolution_clock::time_point now = getCurrentTime();
  //m_time = FpMilliseconds(now-m_startTime).count();
  m_time = std::chrono::duration_cast<std::chrono::milliseconds>(now-m_startTime).count() / 1000.f;

  m_deltaTime = m_time - m_prevTime;
  m_prevTime = m_time;
}

float Time::GetTime()
{
    return m_time;
}

float Time::DeltaTime()
{
    return m_deltaTime;
}
