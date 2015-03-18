#include "TalentItemLayer.h"
#include "Global.h"
#include "TalentMgr.h"
#include "TalentData.h"
#include <iostream>

TalentItemLayer::TalentItemLayer(void)
{
	_talentType = 0;
}


TalentItemLayer::~TalentItemLayer(void)
{
}

bool TalentItemLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
	this->setContentSize(cocos2d::Size(1398,300));
	return true;
}
void TalentItemLayer::initWithType(int type)
{
	this->setTalentType(type);
	cocos2d::Size  sz = this->getContentSize();
	cocos2d::Point  po = this->getPosition();
	cocos2d::Point  centerPo = cocos2d::Point(po.x+sz.width/2,po.y+sz.height/2);


	//Sprite *iconBg = Sprite::create("upgrade/iconBg.png");
	//this->addChild(iconBg);
	//iconBg->setPosition(po.x+70,centerPo.y);
	
	char s1[64];
	sprintf(s1, "upgrade/%d-01.png", type);
	_talentImg = Sprite::create(s1);
    cocos2d::Point  iconPo = cocos2d::Point(po.x+_talentImg->getContentSize().width/2,centerPo.y);
	this->addChild(_talentImg);
	_talentImg->setPosition(iconPo);

	

	_talentName = Label::create("","Copperplate Gothic Bold.ttf",35);
	_talentName->setColor(Color3B(255,255,255));
	this->addChild(_talentName);
	_talentName->setPosition(po.x+_talentImg->getContentSize().width,centerPo.y+60);
    _talentName->setAnchorPoint(cocos2d::Point(0,0));


	 int level = g_talentMgr->getTalent(type)->getTalentLevel();
	switch(type)
	{
		case 0:
			_talentName->setString("Increase the number of skill bar by 1");
		break;
		case 1:
			if(level==0)
			{
				_talentName->setString("Skill_XX");
			}else if(level==1)
			{
				_talentName->setString("Electric Bubble + Any Color = Electric Bubble");
			}else if(level==2)
			{
				_talentName->setString("Colorful Bubble + Any Color * 2 = Colorful Bubble");
			}else if(level==3)
			{
				_talentName->setString("Colorful Bubble + Any Color = Colorful Bubble");
			}else if(level==4)
			{
				_talentName->setString("Electric Bubble + Any Color * 2 = Colorful Bubble");
			}
		break;
		case 2:
			_talentName->setString("Increase duration by 2");
		break;
		case 3:
			_talentName->setString("Increase duration by 1");
		break;
		case 4:
			_talentName->setString("Increase duration by 1");
		break;
		case 5:
			_talentName->setString("Increase the damage of Fire");
		break;
		case 6:
			_talentName->setString("Increase score multiplier by 1/2/4/8");
			break;
		case 7:
			if(level==0)
			{
				_talentName->setString("GoldMulti");
			}else if(level==1)
			{
				_talentName->setString("Double coins after stage 1");
			}else if(level==2)
			{
				_talentName->setString("Triple coins after stage 2");
			}else if(level==3)
			{
				_talentName->setString("Four times coins after stage 4");
			}else if(level==4)
			{
				_talentName->setString("Five times coins after stage 6");
			}
			break;
		case 8:
			_talentName->setString("Increase the damage of electric bubble to boss by 10");
			break;
		case 9:
			_talentName->setString("Increase the damage of elimination around boss by 10");
			break;
		case 10:
			_talentName->setString("Increase the damage of hitting boss by 10");
			break;
		case 11:
			_talentName->setString("Energy fills 10%/20%/40%/80% faster");
			break;
		case 12:
			_talentName->setString("More position for good bubble");
			break;
		case 13:
			_talentName->setString("Good bubble spawn 50%/100%/150%/200% more frequently");
			break;
	}

    centerPo.x =po.x+_talentImg->getContentSize().width+114;
	for (int i=0;i<TALENTLEVEL;i++)
	{
		Sprite *lv = Sprite::create("ui/store/lvBg.png");
		this->addChild(lv);
		lv->setPosition(centerPo.x,centerPo.y);
		centerPo.x +=228;
	}

	centerPo = cocos2d::Point(po.x+sz.width/2,po.y+sz.height/2);

	upgradeBtn = ui::Button::create();
	upgradeBtn->setTouchEnabled(true);
	upgradeBtn->loadTextures("ui/store/upBtn1.png","ui/store/upBtn2.png","");
	cocos2d::Size  btnSz = upgradeBtn->getContentSize();
	this->addChild(upgradeBtn);
	upgradeBtn->setPosition(cocos2d::Point(centerPo.x+sz.width/2-btnSz.width/2,po.y+btnSz.height/2));
	upgradeBtn->setTag(UP_UPGRADE);
	upgradeBtn->addTouchEventListener(this, toucheventselector(TalentItemLayer::upgradeButtonTouch));

	cocos2d::Point  btnPo = upgradeBtn->getPosition();
	
	Sprite *gold = Sprite::create("ui/store/gold.png");
	this->addChild(gold);
	gold->setPosition(centerPo.x-160,btnPo.y-30);
	_goldlabel = Label::create("","Copperplate Gothic Bold.ttf",40);
	_goldlabel->setColor(Color3B(255,255,255));
    _goldlabel->setAnchorPoint(cocos2d::Point(0,0));
    //_goldlabel->enableOutline(Color4B(0,0,0,255),1);
	this->addChild(_goldlabel);
	_goldlabel->setPosition(gold->getPosition().x+gold->getContentSize().width-30,btnPo.y-40);

	Sprite *tiao = Sprite::create("ui/store/tiao.png");
	this->addChild(tiao);
	sz = tiao->getContentSize();
	tiao->setPosition(po.x+sz.width/2,po.y);
    
   

	this->updateLevelShow(level);
    this->updateGoldByLevel(level);


	if(g_gameLayer->getEnableTalentUpdate()==true&&level<4)
	{
		upgradeBtn->setVisible(true);
        upgradeBtn->setTouchEnabled(true);
	}else
	{
		upgradeBtn->setVisible(false);
        upgradeBtn->setTouchEnabled(false);
	}
}
void TalentItemLayer::updateItem(int idx)
{
    this->setTalentType(idx);

	int level = g_talentMgr->getTalent(idx)->getTalentLevel();
	switch(idx)
	{
		case 0:
			_talentName->setString("Increase the number of skill bar by 1");
		break;
		case 1:
			if(level==0)
			{
				_talentName->setString("Electric Bubble + Any Color = Electric Bubble");
			}else if(level==1)
			{
				_talentName->setString("Electric Bubble + Any Color = Electric Bubble");
			}else if(level==2)
			{
				_talentName->setString("Colorful Bubble + Any Color * 2 = Colorful Bubble");
			}else if(level==3)
			{
				_talentName->setString("Colorful Bubble + Any Color = Colorful Bubble");
			}else if(level==4)
			{
				_talentName->setString("Electric Bubble + Any Color * 2 = Colorful Bubble");
			}
		break;
		case 2:
			_talentName->setString("Increase duration by 2");
		break;
		case 3:
			_talentName->setString("Increase duration by 1");
		break;
		case 4:
			_talentName->setString("Increase duration by 1");
		break;
		case 5:
			_talentName->setString("Increase the damage of Fire");
		break;
		case 6:
            if(level==0)
			{
                _talentName->setString("Increase score multiplier by 1");
            }else if(level==1)
			{
				_talentName->setString("Increase score multiplier by 1");
			}else if(level==2)
			{
				_talentName->setString("Increase score multiplier by 2");
			}else if(level==3)
			{
				_talentName->setString("Increase score multiplier by 4");
			}else if(level==4)
			{
				_talentName->setString("Increase score multiplier by 8");
			}
			break;
		case 7:
			if(level==0)
			{
				_talentName->setString("Double coins after stage 1");
			}else if(level==1)
			{
				_talentName->setString("Double coins after stage 1");
			}else if(level==2)
			{
				_talentName->setString("Triple coins after stage 2");
			}else if(level==3)
			{
				_talentName->setString("Four times coins after stage 4");
			}else if(level==4)
			{
				_talentName->setString("Five times coins after stage 6");
			}
			break;
		case 8:
			_talentName->setString("Increase the damage of electric bubble to boss by 10");
			break;
		case 9:
			_talentName->setString("Increase the damage of elimination around boss by 10");
			break;
		case 10:
			_talentName->setString("Increase the damage of hitting boss by 10");
			break;
		case 11:
            if(level==0)
			{
				_talentName->setString("Energy fills 10%faster");
			}else if(level==1)
			{
				_talentName->setString("Energy fills 10% faster");
			}else if(level==2)
			{
				_talentName->setString("Energy fills 20% faster");
			}else if(level==3)
			{
				_talentName->setString("Energy fills 40% faster");
			}else if(level==4)
			{
				_talentName->setString("Energy fills 80% faster");
			}
			
			break;
		case 12:
			_talentName->setString("More position for good bubble");
			break;
		case 13:
            if(level==0)
			{
				_talentName->setString("Good bubble spawn 50% more frequently");
			}else if(level==1)
			{
				_talentName->setString("Good bubble spawn 50%more frequently");
			}else if(level==2)
			{
				_talentName->setString("Good bubble spawn 100% more frequently");
			}else if(level==3)
			{
				_talentName->setString("Good bubble spawn 150% more frequently");
			}else if(level==4)
			{
				_talentName->setString("Good bubble spawn 200% more frequently");
                
			}
			
			break;
	}
	Texture2D *textture;  
	char s1[64];
	sprintf(s1, "upgrade/%d-01.png", idx);
	Sprite * temp_obj = Sprite::create(s1); 
	textture = temp_obj->getTexture(); 
	_talentImg->setTexture(textture);
    
    
    
	this->updateLevelShow(level);
    this->updateGoldByLevel(level);
    
    if(g_gameLayer->getEnableTalentUpdate()==true&&level<4)
	{
		upgradeBtn->setVisible(true);
        upgradeBtn->setTouchEnabled(true);
	}else
	{
		upgradeBtn->setVisible(false);
        upgradeBtn->setTouchEnabled(false);
	}
   
    
	
}
//void TalentItemLayer::reset()
//{
//    this->updateLevelShow(0);
//    this->updateGoldByLevel(0);
//    g_talentMgr->restTalentTest();
//}
void TalentItemLayer::upgradeButtonTouch(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		std::cout << "TalentItemLayer touchEvent begin" << std::endl;
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		g_miscLayer->playWav("btn.wav");
		switch (tag)
		{
		case UP_UPGRADE:
			int level = g_talentMgr->upgradeTalent(this->getTalentType());
			this->updateLevelShow(level);
            this->updateGoldByLevel(level);
			g_upGradeLayer->updateGold(g_goodsData->gold);
			break;
		}

		break;
	}

}

void TalentItemLayer::updateLevelShow(int level)
{
	for (int i=0; i<4; i++)
	{
		this->removeChildByTag(1001+i);
	}
	if(level > 4 || level<=0)
		return;
	
	cocos2d::Size  sz = this->getContentSize();
	cocos2d::Point  po = this->getPosition();
	cocos2d::Point  centerPo = cocos2d::Point(po.x+sz.width/2,po.y+sz.height/2);
	 centerPo.x =po.x+_talentImg->getContentSize().width+114;
	for (int i=0; i<level; i++)
	{
		Sprite *lv = Sprite::create("ui/store/lv.png");
		this->addChild(lv);
		lv->setTag(1001+i);
		lv->setPosition(centerPo.x,centerPo.y);
		centerPo.x += 228;
	}

	//_lastLevel = level;
	
}
void TalentItemLayer::updateGoldByLevel(int level)
{
    int gold = g_talentMgr->getCost(this->getTalentType(), level+1);
    if(gold==0)
    {
        _goldlabel->setString("Highest Level");
        upgradeBtn->setVisible(false);
    }else
    {
        char s1[32];
        sprintf(s1, "%d", gold);
        _goldlabel->setString(s1);
    }
    
   
    
}