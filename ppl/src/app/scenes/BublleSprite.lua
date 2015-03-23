--
-- Author: yzm
-- Date: 2015-03-09 14:55:37
--
local BublleSprite = class("BublleSprite", function(bublleModel)
    -- local texture = display.getImage(self.model.imageFilename)
    -- local frameWidth = texture:getPixelsWide() / 3
    -- local frameHeight = texture:getPixelsHigh()

    -- local spriteFrame = display.newSpriteFrame(texture, cc.rect(0, 0, frameWidth, frameHeight))
    -- local sprite = display.newSprite(spriteFrame)
    -- sprite.animationName_ = self.model.imageFilename
    -- sprite.frameWidth_ = frameWidth
    -- sprite.frameHeight_ = frameHeight
    local sprite = display.newSprite(bublleModel.imageFilename)
    -- print("bublleModel.type",bublleModel.col)
    -- type:类型 特殊球
    -- color:颜色 randomColors：颜色随机数
    local tempLabel = display.newTTFLabel({text = bublleModel.color..bublleModel.type,size = 30 ,color = cc.c3b(255,0,0) })
    	:align(display.CENTER,BUBBLE_RADIUS,BUBBLE_RADIUS):addTo(sprite)
    return sprite
end)

function BublleSprite:ctor(bublleModel)
    self.model = bublleModel
end

function BublleSprite:setStickPp(bubble)
    
end

function BublleSprite:getAngleForce()
    return 1
end


function BublleSprite:getModel()
    return self.model
end

function BublleSprite:setRowColIndex(row, col)
    self.model.row = row
    self.model.col = col
end

function BublleSprite:getType()
    return tonumber(self.model.type)
end

function BublleSprite:setType(type)
    self.model.type = type
end

function BublleSprite:getRow()
    return tonumber(self.model.row)
end

function BublleSprite:setRow(row)
    self.model.row = row
end

function BublleSprite:getCol()
    return tonumber(self.model.col)
end

function BublleSprite:setCol(col)
    self.model.col = col
end

function BublleSprite:getColor()
    return tonumber(self.model.color)
end

function BublleSprite:setColor(color)
    self.model.color = color
end

function BublleSprite:randomColor(lower, greater)
    local m_color = randomInt(lower, greater)
    self.model.color = m_color
    return m_color
end

function BublleSprite:start(destination)
    -- self.model_:setDestination(destination)
    -- self:updatePosition()
    -- self:playAnimationForever(display.getAnimationCache(self.animationName_))
    return self
end

function BublleSprite:step(dt)
    self.model:step(dt)
    self:update()
    return self
end

function BublleSprite:update()

end

return BublleSprite