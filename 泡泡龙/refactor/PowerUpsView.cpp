#include "PowerUpsView.h"  
#include "SimpleAudioEngine.h"  
#include "PowerUpsItem.h"

using namespace cocos2d;  
using namespace CocosDenshion;  

PowerUpsView::PowerUpsView(void)
{
}


PowerUpsView::~PowerUpsView(void)
{
} 

bool PowerUpsView::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}  

	_tableView = TableView::create(this, cocos2d::Size(1395, 1300));
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
cocos2d::Size  PowerUpsView::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(1395,313);
}
TableViewCell* PowerUpsView::tableCellAtIndex(TableView *table,ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
    
	if (!cell)
	{
        
		cell = new TableViewCell();
        
		cell->autorelease();
        
		PowerUpsItem *item = PowerUpsItem::create();
		item->initWithType(idx);
        if (idx==0) {
            item->updateItemState(1);
        }else{
            item->updateItemState(2);
        }
		
		item->setAnchorPoint(cocos2d::Point(0,0));
		item->setPosition(cocos2d::Point(0, 0));
		item->setTag(POWER_ITEM_TAG);
        
		cell->addChild(item);

	}  

	return cell;  

}
ssize_t PowerUpsView::numberOfCellsInTableView(TableView *table)  
{  
	return 4;  
}  
void PowerUpsView::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{   
}
void PowerUpsView::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{ 
}
void PowerUpsView::tableCellHighlight(TableView *table, TableViewCell *cell)  
{  
}  
void PowerUpsView::tableCellUnhighlight(TableView *table, TableViewCell *cell)  
{  
}  
void PowerUpsView::tableCellTouched(TableView *table, TableViewCell *cell)
{  
}
void PowerUpsView::updateItems(int itemIndex)
{
	for (int i=0; i<numberOfCellsInTableView(_tableView); i++)
	{
		if(i!=itemIndex){
			TableViewCell *cell = _tableView->cellAtIndex(i); //this->tableCellAtIndex(_tableView, 2);
			PowerUpsItem *item = dynamic_cast<PowerUpsItem *>(cell->getChildByTag(POWER_ITEM_TAG));
			item->updateItemState(2);
		}
	}
	
}