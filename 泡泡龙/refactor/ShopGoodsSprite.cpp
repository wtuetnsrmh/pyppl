#include "ShopGoodsSprite.h"
#include "Global.h"

ShopGoodsSprite::ShopGoodsSprite(void)
{
}


ShopGoodsSprite::~ShopGoodsSprite(void)
{
}

bool ShopGoodsSprite::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	} 
	return true;
}
void ShopGoodsSprite::initWithType(int type)
{
	_type = type;
	char s1[64];
	sprintf(s1, "shop/%d-01.png", type);
	goods=Sprite::create(s1);  
	this->addChild(goods);
	this->setShopGoodsType(type);

	char s2[64];
	sprintf(s2, "shop/%d-02.png", type);
	goodsName=Sprite::create(s2);  
	this->addChild(goodsName);
	goodsName->setPosition(cocos2d::Point(0,180));
}
void ShopGoodsSprite::changeType(int type)
{
	this->setShopGoodsType(type);
	char sn1[64];
	sprintf(sn1, "shop/%d-01.png", type);

	char sn2[64];
	sprintf(sn2, "shop/%d-02.png", type);

	Texture2D *textture1;
	Texture2D *textture2;


	Sprite * temp_obj1 = Sprite::create(sn1); 
	Sprite * temp_obj2 = Sprite::create(sn2); 

	textture1 = temp_obj1->getTexture();  
	textture2 = temp_obj2->getTexture();  

	goods->setTexture(textture1); 
	goodsName->setTexture(textture2); 

}