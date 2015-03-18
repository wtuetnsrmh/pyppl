#include "SkillBoxLayer.h"
#include "GameConst.h"
#include "Global.h"
#include "SkillSprite.h"
#include "SkillStateBg.h"
#include "TalentData.h"
#include "TalentMgr.h"
SkillBoxLayer::SkillBoxLayer(void)
{
}


SkillBoxLayer::~SkillBoxLayer(void)
{
}

bool SkillBoxLayer::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}
	cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
	cocos2d::Point  gunPo =g_shotLayer->getGunPosition();
//    int kbHeight =35*PIC_RADIO;
//	cocos2d::Size  gunSz =Size((g_shotLayer->_gunBg->getContentSize().width)*PIC_RADIO,(g_shotLayer->_gunBg->getContentSize().height-kbHeight)*PIC_RADIO);
  
    
	cocos2d::Point  po = cocos2d::Point(gunPo.x+2*SKILL_BWIDTH-60,gunPo.y+60);

	int columnCount = g_talentMgr->getTalent(TT_Skill_Column)->getTalentLevel();
	for (int i=0;i<columnCount;i++)
	{
		SkillStateBg *sp = SkillStateBg::initWithState(1);
		sp->setPosition(cocos2d::Point(po));
		this->addChild(sp);
		sp->setTag( SKILL_STATE );
		po.x+=SKILL_BWIDTH;

		_skillBgStateList.push_back(sp);
	}
	for (int i=columnCount;i<SKILL_NUMBER;i++)
	{
		SkillStateBg *sp = SkillStateBg::initWithState(0);
		sp->setPosition(cocos2d::Point(po));
		this->addChild(sp);
		sp->setTag( SKILL_STATE );
		po.x+=SKILL_BWIDTH;

		_skillBgStateList.push_back(sp);
	}
    
    
    
    if(visibleSize.width<IPADWIDTH)
    {
        float radioX = visibleSize.width / IPADWIDTH;
        float radioY = visibleSize.height / IPADHEIGHT ;
        this->setScale(radioX , radioY);
        
        this->setPosition(0,188);
    }
	return true;  
}

void SkillBoxLayer::addSkill(std::list<int> typeList)
{
	if(typeList.size() == 0)
		return;
	std::list<int>::iterator iter;
	int delayIndex = 0;

	int curCount = _skillList.size();
	MyRect gameFrame = g_gameLayer->getGameFrame();

	int startLeftX = 0;
	if(curCount>=1)
		startLeftX = (curCount-1) * SKILL_SWIDTH + SKILL_BWIDTH;
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  gunPo =g_shotLayer->getGunPosition();
//    int kbHeight =35*PIC_RADIO;
//cocos2d::Size  gunSz = cocos2d::Size((g_shotLayer->_gunBg->getContentSize().width)*PIC_RADIO,(g_shotLayer->_gunBg->getContentSize().height-kbHeight)*PIC_RADIO);
cocos2d::Point  po = cocos2d::Point(gunPo.x+2*SKILL_BWIDTH-60,gunPo.y+60);
cocos2d::Point  leftPos = cocos2d::Point(po.x + startLeftX ,po.y);
cocos2d::Point  rightPos = cocos2d::Point(gameFrame.x + gameFrame.width,po.y);

	int columnCount = g_talentMgr->getTalent(TT_Skill_Column)->getTalentLevel();
	for(iter = typeList.begin(); iter!=typeList.end(); ++iter)
	{
		if (curCount >= columnCount)
		{
			break;
		}
		int type = *iter;
		if (this->getChildByTag(type)==NULL)
		{
			SkillSprite *sp = SkillSprite::initWithType(type);
			_skillList.push_back(sp);
			this->addChild(sp);
			sp->setTag(type);
			sp->setScale(0.5f);
			sp->setPosition(rightPos);

            sp->runAction(Sequence::create(
            					DelayTime::create( delayIndex*0.7f), MoveTo::create(0.8f, leftPos),
                                ScaleTo::create(0.5f, 1),
                                NULL));
			//≤•∑≈∂Øª≠
			delayIndex++;
			curCount++;

			//int space = (curCount==1?SKILL_BWIDTH:SKILL_SWIDTH);

			leftPos.x += SKILL_BWIDTH;
		}
	}
}

bool SkillBoxLayer::hasSkill()
{
	if (_skillList.size()>0)
	{
		return true;
	}else
	{
		return false;
	}
}
int SkillBoxLayer::useSkill()
{
	if (_skillList.size()>0)
	{
		SkillSprite * sp= _skillList.front();
		int skillType = sp->getSkillType();
		_skillList.pop_front();
		this->removeChild(sp);
		//_typelist.pop_front();
		doSkillMove();
		return skillType;
	}
	return 1;
}
void SkillBoxLayer::doSkillMove()
{
	std::list<SkillSprite *>::iterator iter;
	int delayIndex = 0;
	
	int curCount = 0;
cocos2d::Size  visibleSize = Director::getInstance()->getVisibleSize();
cocos2d::Point  gunPo =g_shotLayer->getGunPosition();
//    int kbHeight =35*PIC_RADIO;
//cocos2d::Size  gunSz = cocos2d::Size((g_shotLayer->_gunBg->getContentSize().width)*PIC_RADIO,(g_shotLayer->_gunBg->getContentSize().height-kbHeight)*PIC_RADIO);
cocos2d::Point  leftPos = cocos2d::Point(gunPo.x+2*SKILL_BWIDTH-60,gunPo.y+600);
	for(iter = _skillList.begin(); iter!=_skillList.end(); ++iter)
	{
		SkillSprite * sp = *iter;

		sp->runAction(Sequence::create(
                    DelayTime::create( delayIndex*0.7f), MoveTo::create(0.8f, leftPos),
                    ScaleTo::create(0.5f, 1),
                    NULL));

		
		delayIndex++;
		curCount++;

		//int space = (curCount==1?SKILL_BWIDTH:SKILL_SWIDTH);
		leftPos.x += SKILL_BWIDTH ;
	}
}


void SkillBoxLayer::updateSkillBgState()
{
	int columnCount = g_talentMgr->getTalent(TT_Skill_Column)->getTalentLevel();
	for (int i=0;i<columnCount;i++)
	{
		_skillBgStateList.at(i)->changeState(1);
	}
	for (int i=columnCount;i<SKILL_NUMBER;i++)
	{
		_skillBgStateList.at(i)->changeState(0);
	}
}

