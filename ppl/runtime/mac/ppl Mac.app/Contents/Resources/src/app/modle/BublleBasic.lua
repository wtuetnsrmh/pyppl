--
-- Author: yzm
-- Date: 2015-03-09 14:58:23
--
--
local BublleBasic = class("BublleBasic")

function BublleBasic:ctor(data)
	self.type = data.type or 1
	self.color = data.color or 1
	self.x = data.x
	self.y = data.y
	self.attributes = data.attributes
    self.speed = 5
    self.imageFilename = string.format("popups/bublle%d.png",1)
    -- self.imageFilename = string.format("popups/bublle%d.png",self.type)
end

return BublleBasic