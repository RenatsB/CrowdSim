#ifndef CROWDLIB_NAMEMAKER_H_
#define CROWDLIB_NAMEMAKER_H_
#include <string>
#include <cstdlib>
#include <array>
#include <memory>
class RandF;

class NameMaker
{
public:
NameMaker(std::shared_ptr<RandF> _r)
{
    m_randF = _r;
}
std::string makeName();
private:
std::shared_ptr<RandF> m_randF;
int u_rand(int r_high);
std::array<std::string,40> m_names = {"John","Ryan","Kate","Lucy","Eric","Josh","Mary","Justin",
                                   "Lester","Bertha","Stacy","Rob","Alex","Nancy","Luke",
                                   "Gillian","Rosey","Mark","Bethany","Chris","Anthony",
                                   "Greg","Jennifer","Craig","Daniel","Courtney","Bella",
                                   "Max","Jack","Lacey","Duncan","Derek","Megan","Rose",
                                   "Jon","Gregory","Lance","Mike","Barbara","Clementine"};
std::array<std::string,40> m_snames = {"Stark","Baddison","Stabby","Kickers","Fle","Rammstein","Duke",
                                     "Rock","Minor","Tortures","Buyers","Jumpy","WoopWoop",
                                     "Baddison","Quantum","Kicksass","Rosewell","Abductor","Punchers",
                                     "Rammers","Buddies","Breakdown","Mental","Longrun","Giggles",
                                     "Insaneman","Metal","Brute","Poachey","Stableson","Saneman",
                                     "Greasey","Afraid","Runaway","Macey","Magnificent","Claw",
                                     "Cuntington","Jerkins","Crusher"};
};
#endif //CROWDLIB_NAMEMAKER_H_
