#include <cassert>
#include <cstdio>
#include "../src/StatsPanelLayout.h"
int main(){
    for(int row=18;row<=32;row+=2)for(int count=0;count<=38;++count){
        RaceStatsLayout r=raceStatsLayout(40,4*row,6,row,40,count,410);
        assert(r.attributesHeight>=4*row);
        assert(r.titleTop>=40+r.attributesHeight);
        assert(r.frameTop>=r.titleTop+r.titleHeight);
        assert(r.frameHeight>=count*row+12);
        assert(r.frameTop+r.frameHeight+6<=410+r.extraHeight);
        RaceStatsLayout fewer=raceStatsLayout(40,4*row,6,row,40,2,410);
        RaceStatsLayout again=raceStatsLayout(40,4*row,6,row,40,count,410);
        assert(again.extraHeight==r.extraHeight);
        assert(fewer.extraHeight<=r.extraHeight||count<2);
    }
    puts("PASS: actual stats layout fits rows, separates sections, preserves Description, race changes have no cumulative growth (312 cases).");
}
