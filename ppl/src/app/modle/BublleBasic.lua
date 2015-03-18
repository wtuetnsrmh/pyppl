--
-- Author: yzm
-- Date: 2015-03-09 14:58:23
--
--
local BublleBasic = class("BublleBasic")

function BublleBasic:ctor(data)
	self.type = data.type or 0
	self.color = data.color or 1
	self.row = data.row or -1
	self.col = data.col or -1
	self.attributes = data.attributes or {}
    self.speed = data.speed or 5
    self.imageFilename = string.format("#bublle%02d.png",self.color)
    -- self.imageFilename = string.format("popups/bublle%d.png",self.type)
end

return BublleBasic