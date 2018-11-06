#ifndef NAMEMAKER_H_
#define NAMEMAKER_H_
#include <string>
#include <cstdlib>
#include <array>
class NameMaker
{
public:
std::string makeName();
private:
int u_rand(int r_high);
std::array<std::string,40> m_names = {"John","Ryan","Kate","Lucy","Eric","Josh","Mary","Justin",
                                   "Lester","Bertha","Stacy","Rob","Alex","Nancy","Luke",
                                   "Gillian","Rosey","Mark","Bethany","Chris","Anthony",
                                   "Greg","Jennifer","Craig","Daniel","Courtney","Bella",
                                   "Max","Jack","Lacey","Duncan","Derek","Megan","Rose",
                                   "Jon","Gregory","Lance","Mike","Barbara","Clementine"};
std::array<std::string,40> m_snames = {"Lark","Gosling","Prince","Munro","Budd","Rammstein","Duke",
                                     "Stanton","Minor","Beckhoff","Bennington","Digory","Larson",
                                     "Drew","Nicholls","Gregory","Rosewell","Maxwell","Hinderson",
                                     "Mitchell","Lex","Martin","Gillam","Longville","Daniels",
                                     "Boatington","Gum","Jackson","Peterson","Fox","Wall",
                                     "Frederick","Gliss","Cord","Macey","Hurtington","Lord",
                                     "Cranburry","Jerkins","Boulleville"};
};
#endif //NAMEMAKER_H_
