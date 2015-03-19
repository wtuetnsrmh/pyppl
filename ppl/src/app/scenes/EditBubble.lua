--
-- Author: yzm
-- Date: 2015-03-09 14:55:37
-- 编辑球

local EditBubble = class("EditBubble", function(bublleModel)
    local sprite = display.newSprite(bublleModel.imageFilename)
    local tempLabel = display.newTTFLabel({text = bublleModel.color..bublleModel.type,size = 30 ,color = cc.c3b(255,0,0) })
    	:align(display.CENTER,BUBBLE_RADIUS,BUBBLE_RADIUS):addTo(sprite)
    return sprite
end)

function EditBubble:ctor(bublleModel,callBack)
    self.model = bublleModel

    self:setTouchEnabled(true)
    self:addNodeEventListener(cc.NODE_TOUCH_EVENT, callBack)
end

function EditBubble:getModel()
    return self.model
end

function EditBubble:setRowColIndex(row, col)
    self.model.row = row
    self.model.col = col
end

function EditBubble:getType()
    return tonumber(self.model.type)
end

function EditBubble:setType(type)
    self.model.type = type
end

function EditBubble:getRow()
    return tonumber(self.model.row)
end

function EditBubble:setRow(row)
    self.model.row = row
end

function EditBubble:getCol()
    return tonumber(self.model.col)
end

function EditBubble:setCol(col)
    self.model.col = col
end

function EditBubble:getColor()
    return tonumber(self.model.color)
end

function EditBubble:setColor(color)
    self.model.color = color
end

function EditBubble:randomColor(lower, greater)
    local m_color = randomInt(lower, greater)
    self.model.color = m_color
    return m_color
end

function EditBubble:start(destination)
    return self
end

function EditBubble:step(dt)
    self.model:step(dt)
    self:update()
    return self
end

function EditBubble:update()

end

return EditBubble