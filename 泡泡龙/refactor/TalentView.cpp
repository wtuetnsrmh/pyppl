#include "TalentView.h"  
#include "SimpleAudioEngine.h"  
#include "TalentItemLayer.h"
#include "Global.h"

using namespace cocos2d;  
using namespace CocosDenshion;  

TalentView::TalentView(void)
{
}


TalentView::~TalentView(void)
{
} 

bool TalentView::init()  
{  
	if ( !Layer::init() )  
	{  
		return false;  
	}  

	//this->setTouchEnabled(true);
	//ÃÌº”¡–±Ì  

	_tableView=TableView::create(this, cocos2d::Size(1398, 1300));
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
cocos2d::Size  TalentView::cellSizeForTable(TableView *table)
{
	return cocos2d::Size(1398,300);
}
//…˙≥…cell
TableViewCell* TalentView::tableCellAtIndex(TableView *table,ssize_t idx)
{
	TableViewCell *cell = table->dequeueCell();
    
	if (!cell)
	{
        
		cell = new TableViewCell();
        
		cell->autorelease();
        
		//…Ë÷√µ±«∞cellÕº∆¨
		TalentItemLayer *talentItem = TalentItemLayer::create();
		talentItem->initWithType(idx);
		talentItem->setAnchorPoint(cocos2d::Point(0,0));
		talentItem->setPosition(cocos2d::Point(0, 0));
		talentItem->setTag(123);
		cell->addChild(talentItem);

	} 
	else
	{
		TalentItemLayer *talentItem = (TalentItemLayer*)cell->getChildByTag(123);
		talentItem->updateItem(idx);
	}

	return cell;  

}
//cellµƒ ˝¡ø  
ssize_t TalentView::numberOfCellsInTableView(TableView *table)  
{  
	return 14;  
}  
void TalentView::scrollViewDidScroll(cocos2d::extension::ScrollView* view)
{   
	/*Point off = view->getContentOffset();
    if (off.y < view->minContainerOffset().y) {  
        view->setContentOffset(Point(off.x, view->minContainerOffset().y));  
    }  
      
    if (off.y > view->maxContainerOffset().y) {  
        view->setContentOffset(Point(off.x, view->maxContainerOffset().y));  
    } */
}
void TalentView::scrollViewDidZoom(cocos2d::extension::ScrollView* view)
{ 
}
void TalentView::tableCellHighlight(TableView *table, TableViewCell *cell)  
{  
}  
void TalentView::tableCellUnhighlight(TableView *table, TableViewCell *cell)  
{  
}  
void TalentView::tableCellTouched(TableView *table, TableViewCell *cell)
{  
}
void TalentView::updateItems()
{
//	for (int i=0; i<5; i++)
//	{
//
//        TableViewCell *cell = _tableView->cellAtIndex(i);
//        TalentItemLayer *item = dynamic_cast<TalentItemLayer *>(cell->getChildByTag(123));
//        item->reset();
//        
//	}
    g_talentMgr->restTalentTest();
	
}