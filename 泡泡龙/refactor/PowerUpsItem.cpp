#include "PowerUpsItem.h"
#include "Global.h"
#include <iostream>

PowerUpsItem::PowerUpsItem(void)
{
}


PowerUpsItem::~PowerUpsItem(void)
{
}

bool PowerUpsItem::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
	this->setContentSize(cocos2d::Size(1395,313));
	return true;
}
void PowerUpsItem::initWithType(int type)
{
	cocos2d::Size  sz = this->getContentSize();
	cocos2d::Point  po = this->getPosition();
	cocos2d::Point  centerPo = cocos2d::Point(po.x+sz.width/2,po.y+sz.height/2);

	_itemBg = Sprite::create("ui/powerups/item2.png");
	_itemBg->setPosition(centerPo);
	this->addChild(_itemBg);

    _nameLabel=Label::create("","Copperplate Gothic Bold.ttf",50);
	_nameLabel->setColor(Color3B(255,255,0));
    _nameLabel->setAnchorPoint(cocos2d::Point(0,0));
	//_nameLabel->enableOutline(Color4B(0,0,0,255),2);
	this->addChild(_nameLabel);
    
	_desLabel=Label::create("", "Copperplate Gothic Bold.ttf",43);
	_desLabel->setColor(Color3B(255,255,255));
	//_desLabel->enableOutline(Color4B(0,0,0,255),1);
	//_desLabel->setPosition(iconPo.x+200,iconPo.y-50);
     _desLabel->setAnchorPoint(cocos2d::Point(0,0));
	this->addChild(_desLabel);
    cocos2d::Point  iconPo ;
	switch(type)
	{
		case 0:
			_nameLabel->setString("Assistant Ray");
			_desLabel->setString("Help you to aim precisely");
			this->setPowerType(TT_AssistLine);
			char s[64];
			sprintf(s,"power/%d-01.png",TT_AssistLine);
			_iconImg=Sprite::create(s);
            iconPo = cocos2d::Point(po.x+_iconImg->getContentSize().width/2,centerPo.y);
			_iconImg->setPosition(iconPo);
			this->addChild(_iconImg);
		break;
		case 1:
			_nameLabel->setString("Heart Freeze");
			_desLabel->setString("Freeze heart to avoid reduction");
			this->setPowerType(TT_HeartFreeze);
			sprintf(s,"power/%d-01.png",TT_HeartFreeze);
			_iconImg=Sprite::create(s);
            iconPo = cocos2d::Point(po.x+_iconImg->getContentSize().width/2,centerPo.y);
			_iconImg->setPosition(iconPo);
			this->addChild(_iconImg);
		break;
		case 2:
			_nameLabel->setString("Electrical storm");
			_desLabel->setString("Many electrical bubbles");
			this->setPowerType(TT_Electric);
			sprintf(s,"power/%d-01.png",TT_Electric);
			_iconImg=Sprite::create(s);
            iconPo = cocos2d::Point(po.x+_iconImg->getContentSize().width/2,centerPo.y);
			_iconImg->setPosition(iconPo);
			this->addChild(_iconImg);
		break;
		case 3:
			_nameLabel->setString("Fire");
			_desLabel->setString("Attack boss directly");
			this->setPowerType(TT_FireShot);
			sprintf(s,"power/%d-01.png",TT_FireShot);
			_iconImg=Sprite::create(s);
            iconPo = cocos2d::Point(po.x+_iconImg->getContentSize().width/2,centerPo.y);
			_iconImg->setPosition(iconPo);
			this->addChild(_iconImg);
		break;
	}
    _nameLabel->setPosition(iconPo.x+_iconImg->getContentSize().width/2,iconPo.y+50);
    _desLabel->setPosition(iconPo.x+_iconImg->getContentSize().width/2,iconPo.y-50);
    

	 
	_chooseBtn=Button::create();
	_chooseBtn->loadTextures("ui/powerups/choose1.png","ui/powerups/choose1.png","");
	_chooseBtn->setTouchEnabled(true);
	cocos2d::Size btnSz = _chooseBtn->getContentSize();
	_chooseBtn->setPosition(cocos2d::Point(centerPo.x+sz.width/2-btnSz.width/2-10,centerPo.y));
	
	this->addChild(_chooseBtn);
	_chooseBtn->setTag(P_CHOOSE);
	_chooseBtn->addTouchEventListener(this, toucheventselector(PowerUpsItem::touchEvent));

	_itemIndex = type;
    Sprite *tiao = Sprite::create("ui/store/tiao.png");
	this->addChild(tiao);
	sz = tiao->getContentSize();
	tiao->setPosition(po.x+sz.width/2,po.y);
	//g_powerupsUI->setNowPower(TT_AssistLine);
}
void PowerUpsItem::touchEvent(Sprite* pSender,TouchEventType type)
{
	int tag=((Sprite*)pSender)->getTag();
	switch(type){
	case TOUCH_EVENT_BEGAN:
		break;
	case TOUCH_EVENT_MOVED:
		break;
	case  TOUCH_EVENT_ENDED:
		g_miscLayer->playWav("btn.wav");
		switch (tag)
		{
			case P_CHOOSE:
				g_powerupsUI->updateNowPower(this->getPowerType(),_nameLabel->getString(),_desLabel->getString(), _itemIndex);
				g_powerupsUI->setNowPower(this->getPowerType());
				g_powerLayer->changePowerIcon(this->getPowerType());
				updateItemState(1);
				break;
			case P_OPEN:
				updateItemState(2);
				break;
		}
		break;
	}

}
void PowerUpsItem::updateItemState(int state)
{
	_state = state;
	Texture2D *textture;  
	Sprite * temp_obj ;
	switch (state)
	{
		case 1://“——°‘Ò
			
			temp_obj= Sprite::create("ui/powerups/item3.png"); 
			textture = temp_obj->getTexture(); 
			_itemBg->setTexture(textture);
            _chooseBtn->loadTextures("ui/powerups/choose2.png","ui/powerups/choose2.png","");
			//_chooseBtn->setVisible(false);
			break;
		case 2://ø…—°‘Ò
			temp_obj = Sprite::create("ui/powerups/item2.png"); 
			textture = temp_obj->getTexture(); 
			_itemBg->setTexture(textture);
			//_chooseBtn->setVisible(true);
			_chooseBtn->loadTextures("ui/powerups/choose1.png","ui/powerups/choose1.png","");
			_chooseBtn->setTag(P_CHOOSE);
			break;
		default:
			temp_obj = Sprite::create("ui/powerups/item1.png"); 
			textture = temp_obj->getTexture(); 
			_itemBg->setTexture(textture);
			_chooseBtn->setVisible(true);
			_chooseBtn->loadTextures("ui/powerups/open1.png","ui/powerups/open2.png","");
			_chooseBtn->setTag(P_OPEN);
			break;
	}
}
void PowerUpsItem::setPowerType(Talent_Type type)
{
	_powerType = type;
}
Talent_Type PowerUpsItem::getPowerType()
{
	return _powerType;
}
