#include "NameMaker.h"

std::string NameMaker::makeName()
{
    srand (time(NULL));
    std::string name = m_names.at(u_rand(m_names.size()-1));
    std::string surname = m_snames.at(u_rand(m_snames.size()-1));
    std::string final;
    final.append(name);
    final.append(" ");
    final.append(surname);
    return final;
}

int NameMaker::u_rand(int r_high)
{
    double r = rand()/(1.0 + RAND_MAX);
    int range = r_high - 1;
    int r_scaled = (r * range);
    return r_scaled;
}
