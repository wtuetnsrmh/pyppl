#include "LoseUI.h"

#include "GameConst.h"
#include "Global.h"
#include "UpGradeLayer.h"
#include "ShopLayer.h"
#include "Stat.h"
#include "GameCenterCpp.h"
#include "MaskLayer.h"


#define LUI 1000
LoseUI::LoseUI(void)
{
}


LoseUI::~LoseUI(void)
{
}

bool LoseUI::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}  
	//this->setScale(0.3f);
cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = Director::getInstance()->getVisibleOrigin();
	//LayerColor *lc = LayerColor::create(Color4B(0, 0, 0, 110));

	//this->addChild(lc, 0);

    lc = LayerColor::create(Color4B(0, 0, 0, 210));
    g_miscLayer->addChild(lc);
	//ÃÌº”loseUI
	auto loseUI= cocostudio::GUIReader::getInstance()->widgetFromJsonFile("ui/loseUI/loseUI_1.ExportJson");
cocos2d::Size  sz = loseUI->getContentSize();
	cocos2d::Point center = cocos2d::Point(origin.x+visibleSize.width/2-sz.width/2, origin.y+visibleSize.height/2-sz.height/2);
	loseUI->setPosition(center);
	this->addChild(loseUI);
 
	 Button* shareBtn=(Button*)loseUI->getChildByName("shareBtn");
	 shareBtn->setTag(BTN_SHARE);
	 shareBtn->addTouchEventListener(this, toucheventselector(LoseUI::touchEvent));



	 //÷ÿ–¬È_ º∞¥≈•
	 Button* replayBtn=(Button*)loseUI->getChildByName("replayBtn");
	 replayBtn->setTag(BTN_RESTART);
	 replayBtn->addTouchEventListener(this, toucheventselector(LoseUI::touchEvent));


	 Button* upgradeBtn=(Button*)loseUI->getChildByName("upgradeBtn");
	 upgradeBtn->setTag(BTN_UPGRADE);
	 upgradeBtn->addTouchEventListener(this, toucheventselector(LoseUI::touchEvent));

	 Button* shopBtn=(Button*)loseUI->getChildByName("shopBtn");
	 shopBtn->setTag(BTN_SHOP);
	 shopBtn->addTouchEventListener(this, toucheventselector(LoseUI::touchEvent));
	
	_score = (Text*)loseUI->getChildByName("score");
	_bubbleEliminate = (Text*)loseUI->getChildByName("bubbleEliminate");
	_bubbleDrop = (Text *)loseUI->getChildByName("bubbleDrop");
	_baseScore = (Text *)loseUI->getChildByName("bscore");
	_coin = (Text *)loseUI->getChildByName("coin");
	_stage = (Text *)loseUI->getChildByName("stage");

	Sprite *help = Sprite::create("help/1.png");
	this->addChild(help);
	Texture2D *textture;  

	char s1[64];
	int n = rand() %4+1;
	sprintf(s1, "help/%d.png", n);
	Sprite * temp_obj = Sprite::create(s1); 
	textture = temp_obj->getTexture(); 
	help->setTexture(textture);
	cocos2d::Size hSz = help->getContentSize();
	//help->setScale(1.9f, 1.8f);
	cocos2d::Size  newHSz = cocos2d::Size(hSz.width, hSz.height);
	help->setPosition(center.x+hSz.width/2+55, center.y+sz.height-newHSz.height/2-82);
	

	 g_miscLayer->playWav("lose.wav" );

	 if(visibleSize.width<IPADWIDTH)
	 {
		 this->setScale( visibleSize.width / IPADWIDTH,visibleSize.height / IPADHEIGHT);

	 }
//	if(visibleSize.width<UIWIDTH||visibleSize.height<UIHEIGHT)
//	{
//		 this->setScale( visibleSize.width / UIWIDTH,visibleSize.height / UIHEIGHT);
//	}
	

	this->setLoseData();

	 return true;  
}  

void LoseUI::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();

	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:

		break;
	case  TOUCH_EVENT_ENDED:
		switch(tag){
		case BTN_SHARE:
            this->gotoShare();
			break;
		case BTN_RESTART:
			g_gameLayer->gameRestartFormOne();
            g_miscLayer->removeChild(lc);
			g_miscLayer->removeChild(this);
            

			break;
		case BTN_MENU:
			break;
		case BTN_UPGRADE:
			if (g_miscLayer->getChildByTag(UPGRADE)==NULL)
                {
                    g_upGradeLayer = UpGradeLayer::create();
                    g_miscLayer->addChild(g_upGradeLayer);
                    g_upGradeLayer->setTag(UPGRADE);
                }
			break;
		case BTN_SHOP:
			//if (g_miscLayer->getChildByTag(SHOPUI)==NULL)
            //    {
            //        auto ShopUI = ShopLayer::create();
            //        g_miscLayer->addChild(ShopUI);
            //        ShopUI->setTag(SHOPUI);
            //    }
			if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
			{
				g_coinShopUI = CoinShopUI::create();
				g_miscLayer->addChild(g_coinShopUI);
				g_coinShopUI->setTag(COINHELPUI);
			}
			break;
		}
	}

}
void LoseUI::setLoseData()
{
	auto data = g_gameLayer->getStatData();
	char s1[64];
	sprintf(s1, "%d", data.score);
	char s2[64];
	sprintf(s2, "%d", data.bubbleEliminate);
	char s3[64];
	sprintf(s3, "%d", data.bubbleDrop);
	char s4[64];
	sprintf(s4,"%d", data.coin);
	char s5[64];
	sprintf(s5, "x %d", data.stage);
	char s6[64];
	sprintf(s6, "%d",data.baseScore);
	

	_score->setText(s1);
	_bubbleEliminate->setText(s2);
	_bubbleDrop->setText(s3);
	_coin->setText(s4);
	_stage->setText(s5);
	_baseScore->setText(s6);

	//int s = g_recordScore;

//	if(data.score>s)
//	{
//		Sprite *best = Sprite::create("NEW.png");
//		this->addChild(best);
//		best->setPosition(this->getPosition().x+this->getContentSize().width/2+120,this->getPosition().y+this->getContentSize().height/2-20);
//	}else
//	{
//		Label *bestScore = Label::create("","font/Marker Felt.ttf",30);
//		bestScore->setColor(Color3B(255,0,0));
//		bestScore->enableOutline(Color4B(255,255,255,255),1);
//		char bs[64];
//		sprintf(bs, "Best: %d", s);
//		bestScore->setString(bs);
//		this->addChild(bestScore);
//		bestScore->setPosition(this->getPosition().x+this->getContentSize().width/2+270,this->getPosition().y+this->getContentSize().height/2-100);
//	}
	
	
}

void LoseUI::gotoShare()
{
	this->cutScreen();
    auto delay = DelayTime::create(1);
    this->runAction(Sequence::create(delay,CallFunc::create(this,callfunc_selector(LoseUI::gotoShareDelay)), NULL));
	
}
void LoseUI::gotoShareDelay()
{
    GameCenterCpp * center = new GameCenterCpp();
    center->share(1, true, _sharePicPath);
}
const char *  LoseUI::cutScreen()
{
	Size win = Director::getInstance()->getWinSize();
	RenderTexture *cut = RenderTexture::create(win.width,win.height);
	cut->retain();
	cut->begin();
	this->visit();
	cut->end();
	string jpg = "sharePic.jpg";//--分享图片名称
	cut->saveToFile(jpg);
    
	std::string fullpath = FileUtils::getInstance()->getWritablePath();
    _sharePicPath = fullpath.c_str() ;
    log("Image saved %s", fullpath.c_str());
    
    return _sharePicPath;
    
}