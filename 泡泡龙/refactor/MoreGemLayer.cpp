//
//  MoreGemLayer.cpp
//  bubble2
//
//  Created by MacMini on 14-7-24.
//
//

#include "MoreGemLayer.h"
#include "Global.h"
#include "Masklayer.h"
#include "LoseUI.h"
MoreGemLayer::MoreGemLayer(void)
{
    
}
MoreGemLayer::~MoreGemLayer(void)
{
    
}
bool MoreGemLayer::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    if(this->getChildByTag(MASKLAYER)==NULL)
    {
        MaskLayer *mask = MaskLayer::create();
        this->addChild(mask);
        mask->setTag(MASKLAYER);
    }
    
    MyRect gameFrame = g_gameLayer->getGameFrame();
    cocos2d::Point center = cocos2d::Point(gameFrame.x+gameFrame.width/2,gameFrame.y+gameFrame.height/2);
    Sprite *bg = Sprite::create("goodsShop/noGemBg.png");
    this->addChild(bg);
    bg->setPosition(center);
    
    Button *yesBtn=Button::create();
    yesBtn->setTouchEnabled(true);
    yesBtn->loadTextures("goodsShop/yes1.png", "goodsShop/yes2.png","");
    this->addChild(yesBtn);
    yesBtn->setTag(YES);
    yesBtn->setPosition(cocos2d::Point(center.x-yesBtn->getContentSize().width/2-20, center.y-yesBtn->getContentSize().height));
    yesBtn->addTouchEventListener(this, toucheventselector(MoreGemLayer::touchEvent));
    
    
    Button *noBtn=Button::create();
    noBtn->setTouchEnabled(true);
    noBtn->loadTextures("goodsShop/no1.png", "goodsShop/no2.png","");
    this->addChild(noBtn);
    noBtn->setTag(NO);
    noBtn->setPosition(cocos2d::Point(center.x+noBtn->getContentSize().width/2+20, center.y-noBtn->getContentSize().height));
    noBtn->addTouchEventListener(this, toucheventselector(MoreGemLayer::touchEvent));

    
    return true;
}
void MoreGemLayer::touchEvent(Sprite* pSender,TouchEventType type)
{
    int tag =pSender->getTag();
    switch(type){
		case TOUCH_EVENT_BEGAN:
            
			break;
		case TOUCH_EVENT_MOVED:
            
			break;
		case  TOUCH_EVENT_ENDED:
            g_miscLayer->playWav("btn.wav");
            switch (tag) {
                case YES:
                    g_goodsShopLayer->removeChild(this);
                    if(g_miscLayer->getChildByTag(COINHELPUI)==NULL)
                    {
                        g_coinShopUI = CoinShopUI::create();
                        g_miscLayer->addChild(g_coinShopUI);
                        g_coinShopUI->setTag(COINHELPUI);
                    }
                    break;
                case NO:
                    g_miscLayer->removeChild(g_goodsShopLayer);
                    g_miscLayer->removeChild(g_goodsShopLayer->lc);
                    if (g_miscLayer->getChildByTag(LOSEUI)==NULL&&g_gameLayer->getGameState()==G_OVER)
                    {
                        auto loseUI = LoseUI::create();
                        g_miscLayer->addChild(loseUI);
                        loseUI->setTag(LOSEUI);
                        g_gameLayer->gameRealOver();
                        
                    }
                    break;
                    
                default:
                    break;
            }
			
			break;
    }
    
	
}