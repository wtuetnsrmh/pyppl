#include "LQFlyNumber.h"

LQFlyNumber* LQFlyNumber::create(int num, bool isSign)  
{  
	//char s[64];

	//sprintf(s,"%d",num);


    LQFlyNumber* numsprite = LQFlyNumber::create();  
	
	numsprite->initWithString(num, isSign); 
    
    return numsprite;  
}  
 
  
LQFlyNumber::LQFlyNumber():  
    m_numbertype(klq_flaytype_normal)  
{  
      
}  
  
LQFlyNumber::~LQFlyNumber()  
{  
    //CC_SAFE_RELEASE_NULL(m_label);  
}  
  
bool LQFlyNumber::initWithString(int num, bool isSign)  
{  
    this->setCurValueStr(num, isSign);  
    return true;  
}  
  
/** sets a new curValue to the CCSprite. */  
void LQFlyNumber::setCurValueStr(int num, bool isSign)  
{  
    if (isSign == true)
    {
		if(num < 0)
			_plusPicture = Sprite::create("score/boss/-.png");
		else
		{
			_plusPicture = Sprite::create("score/plus.png");
		}
		this->addChild(_plusPicture);
		_plusPicture->setVisible(true);
    }
	else{
		if(_plusPicture!=NULL)
			_plusPicture->setVisible(false);
	}
	
	char s[32];
	int posx = 40;
	int checknum = num;
	if(num < 0)
	{
		checknum = abs(num);
		while ((checknum = checknum / 10) > 0)
		{
		sprintf(s, "score/boss/%d.png", checknum);
		_numberPicture = Sprite::create(s);
		this->addChild(_numberPicture);
		_numberPicture->setPosition(posx, 0);
		posx += 40;
		}
		sprintf(s, "score/boss/%d.png", num%10);
		_numberPicture = Sprite::create(s);
		this->addChild(_numberPicture);
		_numberPicture->setPosition(posx, 0);
	}else
	{
		while ((checknum = checknum / 10) > 0)
		{
		sprintf(s, "score/%d.png", checknum);
		_numberPicture = Sprite::create(s);
		this->addChild(_numberPicture);
		_numberPicture->setPosition(posx, 0);
		posx += 40;
		}
		sprintf(s, "score/%d.png", num%10);
		_numberPicture = Sprite::create(s);
		this->addChild(_numberPicture);
		_numberPicture->setPosition(posx, 0);
	}
	

	/*
	if (m_label==NULL){  
	const char* fontname = "flynumber.fnt";  //numberFontName(numberFontSize()).c_str()  
	m_label = LabelBMFont::create(m_curValueStr,fontname,300, kCCTextAlignmentCenter);  
	m_label->setColor(ccRED);  
	m_label->setPosition(CCPointZero);  
	this->addChild(m_label,1);  
	}else  
	m_label->setString(m_curValueStr);  
	*/
	
}  
  

void LQFlyNumber::startNumberAnimation()  
{  
    //调用changeScene函数  
    switch (m_numbertype) {   
        case klq_flaytype_break:{ //暴击  放大字体，红色  
            FiniteTimeAction* endcall = CallFunc::create(this, callfunc_selector(LQFlyNumber::removeObject));  
            
            FiniteTimeAction* scaleby1 = ScaleBy::create(0.5f,1.8f);  
            FiniteTimeAction* moveby1 = MoveBy::create(0.5f,Point(40, 70));  
            FiniteTimeAction* delay1 = DelayTime::create(0.2f);  
            FiniteTimeAction* fadeout1 = FadeOut::create(0.2f);  
            Sequence* sequAct1 = Sequence::create(delay1, fadeout1, endcall, NULL);  
            Spawn* spAct1 = Spawn::create(sequAct1, moveby1, scaleby1, NULL);//  
            this->runAction( spAct1 );  
            break;  
        }  
        case klq_flaytype_resolve:{ //分解  缩小字体，黄色  
            FiniteTimeAction* endcall = CallFunc::create(this, callfunc_selector(LQFlyNumber::removeObject));  
            this->setScale(0.7f);  
            
            FiniteTimeAction* moveby1 = MoveBy::create(0.5f,Point(20, 50));  
            FiniteTimeAction* delay1 = DelayTime::create(0.2f);  
            FiniteTimeAction* fadeout1 = FadeOut::create(0.2f);  
            Sequence* sequAct1 = Sequence::create(delay1, fadeout1, endcall, NULL);  
            Spawn* spAct1 = Spawn::create(sequAct1, moveby1, NULL);//  
            this->runAction( spAct1 );  
            break;  
        }  
        default:{ //正常  
            FiniteTimeAction* endcall = CallFunc::create(this, callfunc_selector(LQFlyNumber::removeObject));  
			FiniteTimeAction* scaleby1 = ScaleBy::create(0.06f,1.2f); 
			FiniteTimeAction* scaleby2 = ScaleBy::create(0.06f,0.5f);
			FiniteTimeAction* scaleby3 = ScaleBy::create(0.06f,1.2f);
		
            FiniteTimeAction* moveby1 = MoveBy::create(0.8f,Point(20, 50));  
           
           FiniteTimeAction* fadeout1 = FadeOut::create(1.0f);  

			
            Sequence* sequAct1 = Sequence::create(scaleby1,scaleby2,scaleby3, fadeout1, endcall, NULL);  
            Spawn* spAct1 = Spawn::create(sequAct1, moveby1, NULL);//  
            this->runAction( spAct1 );  
            break;  
        }  
    }  
}  
  
void LQFlyNumber::removeObject()  
{  
    stopAllActions();  
    removeAllChildrenWithCleanup(true);  
	if(this->getParent() != NULL){
		this->getParent()->removeChild(this);
	}
}  
