// SPDX-License-Identifier: GPL-3.0-or-later
#define BOOST_ALL_NO_LIB
#define BOOST_ERROR_CODE_HEADER_ONLY
#include <windows.h>
#include <cstdio>
#include <cmath>
#include <string>
#include <vector>
#include <Debug.h>
#include <core/Functions.h>
#include <kenshi/Kenshi.h>
#include <kenshi/Character.h>
#include <kenshi/RaceData.h>
#include <kenshi/gui/CharacterStatsWindow.h>
#include <kenshi/gui/DatapanelGUI.h>
#include <kenshi/gui/DataPanelLine.h>
#include "StatsPanelLayout.h"
#include "RaceLocalization.h"
#include <kenshi/LocaleInfo.h>
static const char* labels[] = {"Strength", "Melee Attack", "Labouring", "Science", "Engineering", "Robotics", "Weapon Smithing", "Armour Smithing", "Field Medic", "Thievery", "Turrets", "Farming", "Cooking", "Hive Medic", "Veterinary", "Stealth", "Athletics", "Dexterity", "Melee Defence", "Weapons", "Toughness", "Assassination", "Swimming", "Perception", "Katanas", "Sabres", "Hackers", "Heavy Weapons", "Blunt", "Martial Arts", "Mass Combat", "Dodge", "Survival", "Polearms", "Crossbows", "Precision Shooting", "Lockpicking", "Crossbow Smithing"};

static void (*originalStatsUpdate)(CharacterStatsWindow*)=0;
static bool named(MyGUI::Widget* w,const std::string& suffix) {
    const std::string& n=w->getName();return n.size()>=suffix.size() && n.compare(n.size()-suffix.size(),suffix.size(),suffix)==0;
}
static MyGUI::Widget* find(MyGUI::Widget* w,const std::string& name) {
    if(!w)return 0;if(named(w,name))return w;
    for(size_t i=0;i<w->getChildCount();++i){MyGUI::Widget* found=find(w->getChildAt(i),name);if(found)return found;}
    return 0;
}
// Names and colours from the installed Kenshi_TextboxPaintedText template.
// Existing native TextBox getters return empty font names in this game build.
static const char* statsFont="Kenshi_PaintedTextFont_Medium";
static MyGUI::IntCoord relativeTo(MyGUI::Widget* w,MyGUI::Widget* ancestor) {
    MyGUI::IntCoord r=w->getCoord();
    for(MyGUI::Widget* p=w->getParent();p && p!=ancestor;p=p->getParent()){r.left+=p->getLeft();r.top+=p->getTop();}
    return r;
}
static void remember(MyGUI::Widget* w) {
    if(!w->getUserString("RaceMetrics.BaseRect").empty())return;
    MyGUI::IntCoord r=w->getCoord();char s[128];sprintf_s(s,"%d %d %d %d",r.left,r.top,r.width,r.height);w->setUserString("RaceMetrics.BaseRect",s);
}
static MyGUI::IntCoord baseline(MyGUI::Widget* w) {
    remember(w);MyGUI::IntCoord r;
    sscanf_s(w->getUserString("RaceMetrics.BaseRect").c_str(),"%d %d %d %d",&r.left,&r.top,&r.width,&r.height);return r;
}
static MyGUI::TextBox* makeText(MyGUI::Widget* parent,const std::string& id) {
    MyGUI::Widget* old=find(parent,id);if(old)return old->castType<MyGUI::TextBox>(false);
    MyGUI::TextBox* t=parent->createWidget<MyGUI::TextBox>("Kenshi_TextBoxEmptySkin",MyGUI::IntCoord(0,0,1,1),MyGUI::Align::Left|MyGUI::Align::Top,id);
    t->setNeedMouseFocus(false);t->setNeedKeyFocus(false);t->setNeedToolTip(false);return t;
}
static void updateRacialStats(CharacterStatsWindow* window) {
    originalStatsUpdate(window);
    if(!window->character || !window->mMainWidget || !window->isVisible() || !window->attributesDatapanel)return;
    RaceData* race=window->character->getRace();if(!race)return;
    MyGUI::Widget* attributes=find(window->mMainWidget,"AttributesPanel");
    MyGUI::Widget* heading=find(window->mMainWidget,"lbAttributes");
    MyGUI::Widget* description=find(window->mMainWidget,"lbDescription");
    if(!attributes || !attributes->getParent() || !heading || !description)return;
    MyGUI::Widget* frame=attributes->getParent();MyGUI::Widget* parent=frame->getParent();if(!parent)return;
    int category=window->attributesDatapanel->getCurrentCategory();
    DataPanelLine* first=window->attributesDatapanel->getLineByNum(category,0);
    if(!first)return;
    LocaleManager* manager=LocaleManager::getInstance();
    LocaleInfo* currentLocale=manager?manager->getCurrentLocale():0;
    const RaceLocale& language=raceLocale(currentLocale?currentLocale->id:"");
    std::vector<int> stats;std::vector<float> factors;
    for(int stat=1;stat<STAT_END;++stat){float f=race->getStatMod(static_cast<StatsEnumerated>(stat));if(_finite(f)&&f>0&&fabs(f-1)>0.0001f){stats.push_back(stat);factors.push_back(f);}}
    MyGUI::IntCoord originalFrame=baseline(frame),originalAttributes=baseline(attributes);
    MyGUI::IntCoord originalRoot=baseline(window->mMainWidget);
    // Description controls remain intact; only move their frames if more height is needed.
    MyGUI::Widget* descText=find(window->mMainWidget,"Description1Panel");
    MyGUI::Widget* descFrame=descText ? descText->getParent() : 0;
    MyGUI::IntCoord originalDescription=baseline(description),originalDescFrame;
    if(descFrame)originalDescFrame=baseline(descFrame);
    int bottom=0,rowHeight=0;
    for(int i=0;i<window->attributesDatapanel->getNumLines(category);++i){DataPanelLine* line=window->attributesDatapanel->getLineByNum(category,i);if(!line)continue;
        for(int j=0;j<line->getNumWidgets();++j){MyGUI::Widget* w=line->getWidget(j);if(!w)continue;MyGUI::IntCoord r=relativeTo(w,attributes);if(r.bottom()>bottom)bottom=r.bottom();if(r.height>rowHeight)rowHeight=r.height;}}
    if(rowHeight<18)rowHeight=24;
    // Match the native inset, with enough clearance from the painted border.
    int pad=originalAttributes.left+6;if(pad<12)pad=12;
    int titleHeight=heading->getHeight();if(titleHeight<24)titleHeight=24;
    MyGUI::TextBox* title=makeText(parent,"RaceMetrics.StatsTitle");
    title->setFontName(statsFont);
    title->setCaption(MyGUI::UString(language.title));
    int columnWidth=title->getTextSize().width+pad*2;
    // Long translated stat names need space alongside their numeric values too.
    for(size_t i=0;i<stats.size();++i){
        CharacterStatsWindow::Stat* definition=CharacterStatsWindow::getStat(static_cast<StatsEnumerated>(stats[i]));
        if(definition && !definition->name.empty()){
            title->setCaption(MyGUI::UString(definition->name));
            int required=(title->getTextSize().width+pad*2)*4/3+4;
            if(required>columnWidth)columnWidth=required;
        }
    }
    title->setCaption(MyGUI::UString(language.title));
    if(columnWidth<originalFrame.width)columnWidth=originalFrame.width;
    int extraWidth=stats.empty()?0:columnWidth-originalFrame.width;
    columnWidth=originalFrame.width+extraWidth;
    MyGUI::Widget* skills=find(window->mMainWidget,"Skills1Panel");
    MyGUI::Widget* skillsFrame=skills?skills->getParent():0;
    MyGUI::Widget* skillsHeading=find(window->mMainWidget,"lbSkills");
    MyGUI::IntCoord originalSkills,originalSkillsHeading,originalHeading=baseline(heading);
    if(skillsFrame)originalSkills=baseline(skillsFrame);
    if(skillsHeading)originalSkillsHeading=baseline(skillsHeading);
    RaceStatsLayout layout=raceStatsLayout(originalFrame.top,bottom,pad,rowHeight,titleHeight,int(stats.size()),relativeTo(description,parent).top);
    // Use the original description top, not the already shifted top on later updates.
    layout.extraHeight=layout.frameTop+layout.frameHeight+pad-originalDescription.top;if(layout.extraHeight<0)layout.extraHeight=0;
    if(stats.empty())layout.extraHeight=0;
    MyGUI::Widget* raceFrame=find(parent,"RaceMetrics.StatsFrame");
    if(!raceFrame){raceFrame=parent->createWidget<MyGUI::Widget>("Kenshi_FrameSkin",MyGUI::IntCoord(0,0,1,1),MyGUI::Align::Left|MyGUI::Align::Top,"RaceMetrics.StatsFrame");raceFrame->setNeedMouseFocus(false);}
    int oldExtra=atoi(parent->getUserString("RaceMetrics.ExtraHeight").c_str());
    int oldWidth=atoi(parent->getUserString("RaceMetrics.ExtraWidth").c_str());
    if(oldExtra!=layout.extraHeight || oldWidth!=extraWidth){MyGUI::IntCoord r=originalRoot;r.height+=layout.extraHeight;r.width+=extraWidth;window->mMainWidget->setCoord(r);char v[32];sprintf_s(v,"%d",layout.extraHeight);parent->setUserString("RaceMetrics.ExtraHeight",v);sprintf_s(v,"%d",extraWidth);parent->setUserString("RaceMetrics.ExtraWidth",v);}
    if(skillsFrame){MyGUI::IntCoord r=originalSkills;r.left+=extraWidth;skillsFrame->setCoord(r);}
    if(skillsHeading){MyGUI::IntCoord r=originalSkillsHeading;r.left+=extraWidth;skillsHeading->setCoord(r);}
    MyGUI::IntCoord hd=originalHeading;hd.width+=extraWidth;heading->setCoord(hd);
    MyGUI::IntCoord d=originalDescription;d.top+=layout.extraHeight;description->setCoord(d);
    if(descFrame){d=originalDescFrame;d.top+=layout.extraHeight;d.width+=extraWidth;descFrame->setCoord(d);}
    MyGUI::IntCoord ar=originalFrame;ar.width=columnWidth;ar.height=stats.empty()?originalFrame.height:layout.attributesHeight;frame->setCoord(ar);
    MyGUI::IntCoord ac=originalAttributes;ac.width+=extraWidth;ac.height=ar.height-ac.top-pad;attributes->setCoord(ac);
    title->setCoord(originalFrame.left,layout.titleTop,columnWidth,titleHeight);
    title->setFontName(statsFont);title->setTextColour(MyGUI::Colour(0.384314f,0.282353f,0.168627f));
    title->setTextAlign(MyGUI::Align::Center);
    title->setVisible(!stats.empty());
    raceFrame->setCoord(originalFrame.left,layout.frameTop,columnWidth,layout.frameHeight);raceFrame->setVisible(!stats.empty());
    int valueWidth=columnWidth/4;
    for(size_t i=0;i<stats.size();++i){char id[48];sprintf_s(id,"RaceMetrics.Stat.%02d",int(i));std::string prefix=id;
        MyGUI::TextBox* a=makeText(raceFrame,prefix+".label");MyGUI::TextBox* b=makeText(raceFrame,prefix+".value");
        a->setFontName(statsFont);b->setFontName(statsFont);
        a->setTextColour(MyGUI::Colour(0.4f,0.380392f,0.356863f));b->setTextColour(factors[i]>1 ? MyGUI::Colour(0.36f,0.56f,0.20f) : MyGUI::Colour(0.72f,0.23f,0.18f));
        a->setCoord(pad,pad+int(i)*rowHeight,columnWidth-valueWidth-pad*2,rowHeight);
        b->setCoord(columnWidth-valueWidth-pad,pad+int(i)*rowHeight,valueWidth,rowHeight);
        a->setTextAlign(MyGUI::Align::Left|MyGUI::Align::VCenter);b->setTextAlign(MyGUI::Align::Right|MyGUI::Align::VCenter);
        CharacterStatsWindow::Stat* definition=CharacterStatsWindow::getStat(static_cast<StatsEnumerated>(stats[i]));
        std::string label=definition && !definition->name.empty()?definition->name:(stats[i]>0 && size_t(stats[i])<=sizeof(labels)/sizeof(labels[0])?labels[stats[i]-1]:"Unknown stat");
        a->setCaption(MyGUI::UString(label));b->setCaption(MyGUI::UString(raceFactor(factors[i],language)));
        a->setVisible(true);b->setVisible(true);
    }
    for(size_t i=0;i<raceFrame->getChildCount();++i){MyGUI::Widget* w=raceFrame->getChildAt(i);const std::string& n=w->getName();if(n.find("RaceMetrics.Stat.")==0&&atoi(n.c_str()+17)>=int(stats.size()))w->setVisible(false);}
    std::string signature;char info[256];sprintf_s(info,"RaceMetrics v0.11: rows=%d rowHeight=%d padding=%d extraHeight=%d extraWidth=%d",int(stats.size()),rowHeight,pad,layout.extraHeight,extraWidth);signature=info;
    if(parent->getUserString("RaceMetrics.LastLog")!=signature){DebugLog(info);parent->setUserString("RaceMetrics.LastLog",signature);}
}
__declspec(dllexport) void startPlugin() {
    if(KenshiLib::GetKenshiVersion().GetVersion()!="1.0.65"){ErrorLog("RaceMetrics: unsupported game version; disabled.");return;}
    if(KenshiLib::AddHook(KenshiLib::GetRealAddress(&CharacterStatsWindow::_NV_update),updateRacialStats,&originalStatsUpdate)!=KenshiLib::SUCCESS)ErrorLog("RaceMetrics: Stats window hook failed.");
    else DebugLog("RaceMetrics v0.11: localized racial XP multipliers in Character Stats.");
}
BOOL WINAPI DllMain(HINSTANCE,DWORD,LPVOID){return TRUE;}

