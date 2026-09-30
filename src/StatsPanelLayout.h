#pragma once
// Shared by the real widget update and standalone geometry checks.
struct RaceStatsLayout {
    int attributesHeight,titleTop,titleHeight,frameTop,frameHeight,extraHeight;
};
inline RaceStatsLayout raceStatsLayout(int attributeTop,int nativeBottom,int padding,int rowHeight,int titleHeight,int rows,int descriptionTop) {
    RaceStatsLayout r;
    r.attributesHeight=nativeBottom+padding*2;
    r.titleTop=attributeTop+r.attributesHeight+padding;
    r.titleHeight=titleHeight;
    r.frameTop=r.titleTop+titleHeight;
    r.frameHeight=rows*rowHeight+padding*2;
    r.extraHeight=r.frameTop+r.frameHeight+padding-descriptionTop;
    if(r.extraHeight<0)r.extraHeight=0;
    return r;
}
