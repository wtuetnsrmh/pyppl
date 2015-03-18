
#include "SimpleAudioEngine.h"
#include "MainScene.h"
#include "MenuLayer.h"
#include "GameLayer.h"
#include "GameBgLayer.h"
#include "ShotLayer.h"
#include "Global.h"
#include "BossSprite.h"
#include "FileUtil.h"
#include "SkillBoxLayer.h"
#include "GoodsBoxLayer.h"
#include "BottomMenuLayer.h"
#include "PowerLayer.h"
#include "FireLayer.h"
#include <algorithm>
using namespace CocosDenshion;  
USING_NS_CC;
/*
www.3366.com/flash/1000258.shtml
*/

//int PAOPAO_SIZE;
Scene* MainScene::createScene()
{
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();
	int width = visibleSize.width - origin.x;
	int height = visibleSize.height - origin.y;

	std::vector<std::string> searchPaths = FileUtils::getInstance()->getSearchPaths();
	searchPaths.insert(searchPaths.begin(), "effect");
	searchPaths.insert(searchPaths.begin(), "data");
	searchPaths.insert(searchPaths.begin(), "fonts");
	searchPaths.insert(searchPaths.begin(), "gun");
	searchPaths.insert(searchPaths.begin(), "monster");
	searchPaths.insert(searchPaths.begin(), "score");
	searchPaths.insert(searchPaths.begin(), "ui");
	searchPaths.insert(searchPaths.begin(), "wav");
	searchPaths.insert(searchPaths.begin(), "goods");

	FileUtil::readBarrierData();
	FileUtil::readGameGoods();
	FileUtil::readTalentData();
	FileUtil::readNewRecord();
	FileUtil::readGuide();

	BossSprite::preloadAnimation();
	 // 'scene' is an autorelease object
cocos2d::Point  diff = cocos2d::Point(0, -100);
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = MainScene::create();
	

    // add layer as a child to scene
    scene->addChild(layer);
	
	g_menuLayer = MenuLayer::create();
	layer->addChild(g_menuLayer, 1);

	g_gameLayer = GameLayer::create();
	

	auto gameBg = GameBgLayer::create();
	MyRect gf = g_gameLayer->getGameFrame();
	gameBg->addGameFrame(gf.x, gf.y, gf.width, gf.height);
	layer->addChild(gameBg);

	layer->addChild(g_gameLayer);
	
	//g_gameLayer->gameStart();

	

	g_shotLayer = ShotLayer::create();
	layer->addChild(g_shotLayer);
	g_shotLayer->initGun();

	g_scoreLayer = ScoreLayer::create();
	layer->addChild(g_scoreLayer);
	
	g_skillBoxLayer = SkillBoxLayer::create();
	layer->addChild(g_skillBoxLayer);

	g_miscLayer = MiscLayer::create();
	layer->addChild(g_miscLayer);
    g_miscLayer->isSound=true;
    
    //g_gameLayer->gameStart();

    g_goodsBoxLayer = GoodsBoxLayer::create();
	g_miscLayer->addChild(g_goodsBoxLayer);
    
	auto bottomMenuLayer = BottomMenuLayer::create();
	g_miscLayer->addChild(bottomMenuLayer);
    
    

	g_powerLayer = PowerLayer::create();
	g_miscLayer->addChild(g_powerLayer);

	g_powerupsUI = PowerUpsUI::create();
	g_miscLayer->addChild(g_powerupsUI);
    
   

	

#ifdef WIN32
	 AllocConsole();  
	freopen("CONIN$", "r", stdin);  
	freopen("CONOUT$", "w", stdout);  
	freopen("CONOUT$", "w", stderr);  
#endif

	SimpleAudioEngine::getInstance()->preloadEffect( "wav/shot.wav" );
	SimpleAudioEngine::getInstance()->preloadEffect( "wav/lose.wav" );
	SimpleAudioEngine::getInstance()->preloadEffect( "wav/bg.mp3" );
	SimpleAudioEngine::getInstance()->preloadEffect( "wav/wenyi.wav" );
	SimpleAudioEngine::getInstance()->preloadEffect( "wav/win.mp3" );
    SimpleAudioEngine::getInstance()->preloadEffect( "wav/btn.wav" );
    
    
    
    Director::getInstance()->getTextureCache()->addImage("ui/store/storeBg.png");
    Director::getInstance()->getTextureCache()->addImage("ui/store/BACK1.png");
    Director::getInstance()->getTextureCache()->addImage("ui/store/BACK2.png");
    Director::getInstance()->getTextureCache()->addImage("ui/next/bg.png");
    Director::getInstance()->getTextureCache()->addImage("ui/loseUI/loseUI0.png");
    Director::getInstance()->getTextureCache()->addImage("ui/loseUI/loseUI1.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/pass.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/1.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/2.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/3.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/4.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/5.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/6.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/7.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/8.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/9.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/10.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/11.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/12.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/13.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/14.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/15.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/301/16.png");
    
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/1.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/2.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/3.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/4.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/5.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/6.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/7.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/8.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/9.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/10.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/11.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/12.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/13.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/14.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/15.png");
    Director::getInstance()->getTextureCache()->addImage("ui/effect/bubble/300/16.png");
    
    
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/1-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/2-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/3-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/4-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/5-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/6-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/7-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/8-01.png");
    Director::getInstance()->getTextureCache()->addImage("ui/coinShop/9-01.png");
    
    Director::getInstance()->getTextureCache()->addImage("upgrade/0-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/1-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/2-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/3-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/4-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/5-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/6-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/7-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/8-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/9-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/10-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/11-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/12-01.png");
    Director::getInstance()->getTextureCache()->addImage("upgrade/13-01.png");
    
    Director::getInstance()->getTextureCache()->addImage("goodsShop/bg.png");
    
    Director::getInstance()->getTextureCache()->addImage("grass/1-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/2-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/3-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/4-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/5-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/6-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/7-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/8-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/9-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/10-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/11-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/12-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/13-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/14-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/15-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/16-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/17-01.png");
    Director::getInstance()->getTextureCache()->addImage("grass/18-01.png");
    
    
    
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
//    auto closeItem = MenuItemImage::create(
//                                           "CloseNormal.png",
//                                           "CloseSelected.png",
//                                           CC_CALLBACK_1(MainScene::menuCloseCallback, this));
//    
//	closeItem->setPosition(cocos2d::Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
//                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    //auto menu = Menu::create(closeItem, NULL);
    //menu->setPosition(Point::ZERO);
    //this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
	
	
    return true;
}


void MainScene::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
