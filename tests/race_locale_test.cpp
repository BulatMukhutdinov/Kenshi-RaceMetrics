#include "../src/RaceLocalization.h"
#include <cassert>
#include <cstring>
int main(){
    const char* ids[]={"en_GB","ru_RU","de_DE","es_ES","fr_FR","pt_BR","ja_JP","ko_KR","zh_CN"};
    for(int i=0;i<9;++i){
        const RaceLocale& l=raceLocale(ids[i]);assert(&l==&raceLocales[i]);
        assert(l.title[0]);
        std::string bonus=raceFactor(1.2f,l),penalty=raceFactor(.8f,l);
        assert(bonus==(l.decimal==','?"x1,20":"x1.20"));
        assert(penalty==(l.decimal==','?"x0,80":"x0.80"));
    }
    assert(&raceLocale("RU-ru.UTF-8")==&raceLocales[1]);
    assert(&raceLocale("fr")==&raceLocales[4]);
    assert(&raceLocale("unknown")==&raceLocales[0]);
    assert(&raceLocale("")==&raceLocales[0]);
    assert(strcmp(raceLocale("en").title,"RACIAL XP MULTIPLIERS")==0);
    puts("PASS: nine locales, regional aliases, English fallback, bonus/penalty formatting");
}
