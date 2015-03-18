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
    print("bublleModel.type",bublleModel.y)
    -- type:类型 特殊球
    -- color:颜色 randomColors：颜色随机数
    local tempLabel = display.newTTFLabel({text = bublleModel.color..bublleModel.type,size = 20})
    	:align(display.CENTER,BUBLLE_RADIS/2,BUBLLE_RADIS/2):addTo(sprite)
    return sprite
end)

function BublleSprite:ctor(bublleModel)
    self.model = bugModel
end

function BublleSprite:getModel()
    return self.model
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