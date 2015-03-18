#ifndef __MiscLayer_H__
#define __MiscLayer_H__
#include "GameEnum.h"
#include "cocos2d.h"
#include "editor-support/cocostudio/CocoStudio.h"
using namespace cocostudio;
using namespace cocos2d;
class Bubble;
class EffectSprite;
class MiscLayer :
	public Layer
{
public:
	MiscLayer(void);
	~MiscLayer(void);
	virtual bool init();  
	CREATE_FUNC(MiscLayer);  
	void flyNumber(std::list<cocos2d::Point> &ptList);
	void flyOneNumber(Node *obj, void *data);
	void flyOneNumber2(cocos2d::Point pt, int nubmer);

	void showLoseUI();
	void removeLoseUI();
	
	void playWav(const char * file);
	void playMp3(const char * file);
	void playBackgroundMusic(const char * file);

	//µÙ¬‰–ßπ˚
	void dropBubbleAction(Bubble* pBubble);
	void playDropBubbleEffect(Node *obj, void *data);
	void removeNodeAfterEffect(Node *obj);
	void playDownBubbleEffect(Node *node);

	//œ˚≥˝–ßπ˚
	void clearBubbleAction(Bubble* pBubble,bool isAddPower);
	void playClearBubbleEffect(Node *obj, void *data);

	//alert
	void showAlert(int type);
    bool isSound;

	void flyToPower(cocos2d::Point pobubble);
	void addFireShot(bool isAdd);
	void addShiZhong(bool isAdd);

	void fireLaster(cocos2d::Point endPoint); //���ͼ���
	int getShortestLine(float length);

	void playPowerEffect1(Node *node, void *data);
	void playPowerEffect2(Node *node, void *data);
	void playPowerEffect3(Node *node);
	void removePlayPowerEffect(Node *node);
    void showLoading(bool isLoading);
private:
	std::map<int, int> _animationInitMap;
	std::map<int, Armature *>_animationMap;
	time_t _lastBossShowtime;
	Armature *armature;

	int LINE_LENGTH[7];

	EffectSprite *powerEffect;
   
};

#endif

