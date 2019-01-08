#include "NameMaker.h"
#include "RandF.h"

std::string NameMaker::makeName()
{
    std::string name = m_names.at(m_randF.get()->randi(0,m_names.size()-1));
    std::string surname = m_snames.at(m_randF.get()->randi(0,m_snames.size()-1));
    std::string final;
    final = name;
    final.append(" ");
    final.append(surname);
    return final;
}
