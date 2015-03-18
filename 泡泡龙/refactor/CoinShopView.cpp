#include "CoinShopView.h"  
#include "SimpleAudioEngine.h"  
#include "CoinShopItem.h"

using namespace cocos2d;  
using namespace CocosDenshion;  

CoinShopView::CoinShopView(void)
{
}


CoinShopView::~CoinShopView(void)
{
} 

bool CoinShopView::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}  

	_tableView = TableView::create(this, cocos2d::Size(1398, 1300));
	_tableView->setAnchorPoint(cocos2d::Point(0, 0));
	_tableView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);

	_tableView->setPosition(cocos2d::Point(0,0));

	
	_tableView->setDelegate(this);  

	_tableView->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);  

	this->addChild(_tableView);  
	_tableView->setTouchEnabled(false);
	
	_tableView->reloadData(); 
	_tableView->setTouchEnabled(true);

	return true;  
}
cocos2d::Size  CoinShopView::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(1398,300);
}
TableViewCell* CoinShopView::tableCellAtIndex(TableView *table,ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
    
	if (!cell)
	{
        
		cell = new TableViewCell();
        
		cell->autorelease();
        
		CoinShopItem *item = CoinShopItem::create();
		item->initWithType(idx);
		item->setAnchorPoint(cocos2d::Point(0,0));
		item->setPosition(cocos2d::Point(0, 0));
		item->setTag(POWER_ITEM_TAG);
		cell->addChild(item);

	} else
	{
		CoinShopItem *item = (CoinShopItem*)cell->getChildByTag(POWER_ITEM_TAG);
		item->updateItem(idx);
	}


	return cell;  

}
ssize_t CoinShopView::numberOfCellsInTableView(TableView *table)  
{  
	return 9;  
}  
void CoinShopView::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{   
}
void CoinShopView::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{ 
}
void CoinShopView::tableCellHighlight(TableView *table, TableViewCell *cell)  
{  
}  
void CoinShopView::tableCellUnhighlight(TableView *table, TableViewCell *cell)  
{  
}  
void CoinShopView::tableCellTouched(TableView *table, TableViewCell *cell)
{  

}