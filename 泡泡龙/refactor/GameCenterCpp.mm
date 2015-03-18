#include "GameCenterCpp.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "GameCenterOC.h"
#include "GameShareOC.h"
#include "Global.h"
//#include "GameActRecord.h"
//#include "GameMenu.h"
//#include "GameAdOC.h"
#endif
void GameCenterCpp::openGameCenter()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS¥˙¬Î
    //[GameCenterOC hMessageBox:@"cocos2dx sdsd" title:@"Himi"];
    GameCenterOC * oc=[GameCenterOC getInstance];
    [oc openLeaderboard];
#else
    //Android¥˙¬Î
#endif
}
void GameCenterCpp::submitScore(float score)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //iOS¥˙¬Î
    GameCenterOC * oc=[GameCenterOC getInstance];
    [oc submitscore:(score)];
#else
    //Android¥˙¬Î
#endif
}
GameCenterCpp * GameCenterCpp::gSpritewithFile(const char *spName)
{
    GameCenterCpp * pobSprite = new GameCenterCpp();
    if (pobSprite&&pobSprite->initWithFile(spName)) {
        pobSprite->openGameCenter();
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return NULL;
}
void GameCenterCpp::share(int uiKind,bool isRecordBroken,const char *sharePicPath)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    int score = g_gameLayer->getStatData().stage;
    string fullpath = FileUtils::getInstance()->getWritablePath();
     std::string str("hello");
    NSString *nsPath=[NSString stringWithCString:fullpath.c_str() encoding:NSUTF8StringEncoding];
    GameShareOC * ss = [GameShareOC getInstance];
    //if (GameMenu::pathByLanguage=="cn") {
        //[ss openShare:0 IsRecordBroken:isRecordBroken Maxsorce:score UIkind:uiKind SharePicPath:(nsPath)];
    //}else
     [ss openShare:1 IsRecordBroken:isRecordBroken Maxsorce:score UIkind:uiKind SharePicPath:(nsPath)];
    
#else
    //Android¥˙¬Î
#endif
}
/*void GameCenterCpp::gameoverrecord(int gametime,int score,int step,int mergeTimes,int mergeMaxScore,int state)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GameActRecord * ss = [GameActRecord getInstance];
    [ss gameOverRecord :gametime Score:score Step:(step) MergeTimes:(mergeTimes) MergeTMaxScore: (mergeMaxScore) State:(state)];
#else
    //Android¥˙¬Î
#endif
}
void GameCenterCpp::gameGuideAppear(int guideIndex)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GameActRecord * ss = [GameActRecord getInstance];
    [ss gameGuideAppearOC:guideIndex];
#else
    //Android¥˙¬Î
#endif
}*/
/*void GameCenterCpp::initMoreGamesCache()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GameAdOC * ss = [GameAdOC getInstance];
    [ss cacheMoreApps];
#else
    
#endif
}*/
/*void GameCenterCpp::showMoreGames()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    GameAdOC * ss = [GameAdOC getInstance];
    [ss moreGames];
#else
    
#endif
}*/
GameCenterCpp::~GameCenterCpp()
{

}