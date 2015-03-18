--
-- Author: yzm
-- Date: 2015-03-10 13:38:03
--[[
显示完整的item
local ShipListView = require("app.views.ShipListView")
 
local MainScene = class("MainScene", function()
                            return display.newScene("MainScene")
end)
 
function MainScene:ctor()
    self.shipList1 = ShipListView.new({
            bgColor = cc.c4b(200, 200, 200, 120),
            viewRect = cc.rect(display.cx - 240, display.cy + 28, 520, 150),
            direction = cc.ui.UIScrollView.DIRECTION_HORIZONTAL},
        self)
        :onTouch(handler(self, self.touchListener))
        :addTo(self)
    for i=1,100 do
        local item = self.shipList1:newItem()
        local content = display.newSprite("flattop.png")
        item:addContent(content)
        item:setItemSize(104, 150)
        self.shipList1:addItem(item)
    end
    self.shipList1:reload()
    self.shipList1:checkArrowStatus()
end
 
function MainScene:touchListener(event)
    local listView = event.listView
    listView:checkArrowStatus()
    if "clicked" == event.name then
        -- TODO: 
    elseif "moved" == event.name then
        -- TODO: 
    elseif "ended" == event.name then
        -- TODO: 
    else
        -- print("event name:" .. event.name)
    end
end
]]
--
local c = cc
local UIScrollView = c.ui.UIScrollView
 
local ShipListView = class("ShipListView", c.ui.UIListView)
 
function ShipListView:ctor(params, root)
    ShipListView.super.ctor(self, params)
    self.root = root
    -- init arrow
    local viewRect = params.viewRect
    self.fArrow = display.newSprite("UI/Arsenal/ui_common_indicator.png"):addTo(self.root)
    self.fArrow:setFlippedX(true)
    self.fArrow:setVisible(false)
    self.fArrow:setPosition(viewRect.x - 10, viewRect.y + viewRect.height/2)
    self.bArrow = display.newSprite("UI/Arsenal/ui_common_indicator.png"):addTo(self.root)
    self.bArrow:setPosition(viewRect.x + viewRect.width + 10, viewRect.y + viewRect.height/2)
    self.bArrow:setVisible(false)
end
 
function ShipListView:scrollToItem(item)
     
end
 
function ShipListView:autoFixScroll()
    local item, pos = self:getFirstVisibleItem()
    local bound = item:getBoundingBox()
    local nodePoint = self.container:convertToWorldSpace(
        c.p(bound.x + bound.width/2, bound.y))
    local index
    if c.rectContainsPoint(self.viewRect_, nodePoint) then
        index = pos
    else
        index = pos + 1
    end
    local toItem = self.items_[index]
    bound = toItem:getBoundingBox()
    self:scrollToPos(-bound.x + self.viewRect_.x, 0)
end
 
function ShipListView:getFirstVisibleItem()
    for i=1,#self.items_ do
        if self:isItemInViewRect(self.items_<i>) then
            return self.items_<i>, i
        end
    end
end
 
function ShipListView:checkArrowStatus()
    if #self.items_ == 0 then
        return
    end
    local fNeed, bNeed = self:isNeedArrow()
    if fNeed then
        self.fArrow:setVisible(true)
    else
        self.fArrow:setVisible(false)
    end
    if bNeed then
        self.bArrow:setVisible(true)
    else
        self.bArrow:setVisible(false)
    end
end
 
function ShipListView:isNeedArrow()
    local fItem = self.items_[1]
    local bound = fItem:getBoundingBox()
    local nodePoint = self.container:convertToWorldSpace(
        c.p(bound.x, bound.y))
    local fNeed =  not c.rectContainsPoint(self.viewRect_, nodePoint)
 
    bItem = self.items_[#self.items_]
    bound = bItem:getBoundingBox()
    nodePoint = self.container:convertToWorldSpace(
        c.p(bound.x + bound.width, bound.y))
    local bNeed =  not c.rectContainsPoint(self.viewRect_, nodePoint)
    return fNeed, bNeed
end
 
function ShipListView:scrollToPos(x, y)
    local scrollLength = c.pGetLength(c.pSub(c.p(x, y), self.position_))
    self.position_ = c.p(x, y)
    local action = c.MoveTo:create(0.5, self.position_)
    self.scrollNode:runAction(c.EaseElasticOut:create(action))
end
 
function ShipListView:getAllItem()
    return self.items_
end
 
function ShipListView:getFirstItem()
    return self.items_[1]
end
 
function ShipListView:getLastItem()
    return self.items_[#self.items_]
end
-- override
function ShipListView:onTouch_(event)
    if "began" == event.name and not self:isTouchInViewRect(event) then
        printInfo("UIScrollView - touch didn't in viewRect")
        return false
    end
 
    if "began" == event.name and self.touchOnContent then
        local cascadeBound = self.scrollNode:getCascadeBoundingBox()
        if not cc.rectContainsPoint(cascadeBound, cc.p(event.x, event.y)) then
            return false
        end
    end
 
    if "began" == event.name then
        self.prevX_ = event.x
        self.prevY_ = event.y
        self.bDrag_ = false
        local x,y = self.scrollNode:getPosition()
        self.position_ = {x = x, y = y}
 
        transition.stopTarget(self.scrollNode)
        self:callListener_{name = "began", x = event.x, y = event.y}
 
        self:enableScrollBar()
        -- self:changeViewRectToNodeSpaceIf()
 
        self.scaleToWorldSpace_ = self:scaleToParent_()
 
        return true
    elseif "moved" == event.name then
        if self:isShake(event) then
            return
        end
 
        self.bDrag_ = true
        self.speed.x = event.x - event.prevX
        self.speed.y = event.y - event.prevY
 
        if self.direction == UIScrollView.DIRECTION_VERTICAL then
            self.speed.x = 0
        elseif self.direction == UIScrollView.DIRECTION_HORIZONTAL then
            self.speed.y = 0
        else
            -- do nothing
        end
 
        self:scrollBy(self.speed.x, self.speed.y)
        self:callListener_{name = "moved", x = event.x, y = event.y}
    elseif "ended" == event.name then
        if self.bDrag_ then
            self.bDrag_ = false
            self:scrollAuto()
            -- self:autoFixScroll()
            self:callListener_{name = "ended", x = event.x, y = event.y}
 
            self:disableScrollBar()
        else
            self:callListener_{name = "clicked", x = event.x, y = event.y}
        end
    end
end
 
function ShipListView:scrollAuto()
    local status = self:twiningScroll()
    if status == "normal" then
        self:elasticScroll(true)
    elseif status == "sideShow" then
        self:elasticScroll(false)
    end
end
 
function ShipListView:twiningScroll()
    if self:isSideShow() then
        -- printInfo("UIScrollView - side is show, so elastic scroll")
        return "sideShow"
    end
    if math.abs(self.speed.x) < 10 and math.abs(self.speed.y) < 10 then
        -- printInfo("#DEBUG, UIScrollView - isn't twinking scroll:"
        --     .. self.speed.x .. " " .. self.speed.y)
        return "normal"
    end
 
    local disX, disY = self:moveXY(0, 0, self.speed.x*6, self.speed.y*6)
 
    transition.moveBy(self.scrollNode,
                      {x = disX, y = disY, time = 0.3,
                       easing = "sineOut",
                       onComplete = function()
                           self:elasticScroll(true)
    end})
end
 
function ShipListView:elasticScroll(fix)
    local cascadeBound = self:getScrollNodeRect()
    local disX, disY = 0, 0
    local viewRect = self:getViewRectInWorldSpace()
 
    -- dump(cascadeBound, "UIScrollView - cascBoundingBox:")
    -- dump(viewRect, "UIScrollView - viewRect:")
 
    if cascadeBound.width < viewRect.width then
        disX = viewRect.x - cascadeBound.x
    else
        if cascadeBound.x > viewRect.x then
            disX = viewRect.x - cascadeBound.x
        elseif cascadeBound.x + cascadeBound.width < viewRect.x + viewRect.width then
            disX = viewRect.x + viewRect.width - cascadeBound.x - cascadeBound.width
        end
    end
 
    if cascadeBound.height < viewRect.height then
        disY = viewRect.y + viewRect.height - cascadeBound.y - cascadeBound.height
    else
        if cascadeBound.y > viewRect.y then
            disY = viewRect.y - cascadeBound.y
        elseif cascadeBound.y + cascadeBound.height < viewRect.y + viewRect.height then
            disY = viewRect.y + viewRect.height - cascadeBound.y - cascadeBound.height
        end
    end
    if 0 == disX and 0 == disY then
        if fix then
            self:autoFixScroll()
        end
        return
    end
     
    transition.moveBy(self.scrollNode,
                      {x = disX, y = disY, time = 0.3,
                       easing = "backout",
                       onComplete = function()
                           self:callListener_{name = "scrollEnd"}
    end})
end
 
return ShipListView