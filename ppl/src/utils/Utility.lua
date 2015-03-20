--
-- Author: Your Name
-- Date: 2015-03-17 16:24:56
--
-- 根据索引获取坐标位置
--[[
cocos2d::Point  GameLayer::getPointByRowAndCol(int row, int col)  
{ 
	int BUBBLE_RADIUS = PAOPAO_SIZE/2;

	int diffx = _paopaoOffset.x + _centerPointOffset.x;
	int diffy = _paopaoOffset.y + _centerPointOffset.y;

	int posX = col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS  + (row % 2) * BUBBLE_RADIUS + diffx ;
	int posY = ( row * 2 * BUBBLE_RADIUS * sin(PI/3) + BUBBLE_RADIUS ) + diffy ;
	return cocos2d::Point(posX, posY);
}  

RowCol GameLayer::getRowColByPos( int nPosX, int nPosY)
{
	int nRow, nCol;
	-- –Ë“™Àƒ…·ŒÂ»Î
	int BUBBLE_RADIUS = PAOPAO_SIZE/2;
	int diffx = _paopaoOffset.x + _centerPointOffset.x;
	int diffy = _paopaoOffset.y + _centerPointOffset.y;
	nRow = ( nPosY - diffy -BUBBLE_RADIUS )/( 2 *BUBBLE_RADIUS *sin ( PI/3 ) )  +0.5; 
	nCol = ( nPosX - diffx - ( nRow % 2 ) * BUBBLE_RADIUS - BUBBLE_RADIUS ) * 1.0 /(  2 *BUBBLE_RADIUS ) + 0.5; 

	return RowCol( nRow, nCol );
}
]]

local BublleSprite = import("..app.scenes.BublleSprite")
local BublleBasic = import("..app.modle.BublleBasic")


------------wheel-------------------
function convertA1ToA2(centerPoint,paopaoPos, angle)
	local x,y = paopaoPos.x,paopaoPos.y
	local angle1 = 0
	if (x-centerPoint.x) == 0 then
		if (y-centerPoint.y>0) then
			angle1 = 90
		elseif(y-centerPoint.y<0) then
			angle1 = 270
		end
	else
		angle1 = math.atan((y-centerPoint.y) / (x-centerPoint.x) )
		angle1 = math.deg(angle1) --CC_RADIANS_TO_DEGREES
	end
	if(x-centerPoint.x < 0.0) then
		angle1 = angle1 + 180
	end
	-- TODO 用cc.pGetDistance
	local r = cc.pGetDistance(paopaoPos,centerPoint)
	-- math.sqrt(math.pow(y-centerPoint.y, 2) + math.pow(x-centerPoint.x, 2) )

	local x1 = r * math.cos( math.rad(  angle1 + angle)  )
	local x11= x1	+ centerPoint.x
	local y1 = r * math.sin( math.rad(( angle1 + angle ) )
	local y11 = y1 + centerPoint.y
	local truePoint = cc.p(x11, y11)
	
	return truePoint
end

--三点求三角形面积 S=(1/2)*(x1y2+x2y3+x3y1-x1y3-x2y1-x3y2)
function trianglearea(x1,y1,x2,y2,x3,y3)
	local temp = (x2*y3-y2*x3+x3*y1-y3*x1+x1*y2-x2*y1)
	return math.abs(1.0/2*temp)
end

--两点距离
function dist(x1, y1, x2, y2)
	return math.sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))
end

-- 点(x1,y1)到直线(x2,y2,x3,y3)的距离   d=|Aa+Bb+C|/√(A^2+B^2) 
function distance(x1, y1, x2, y2, x3, y3)
	local temp1 = trianglearea(x1,y1,x2,y2,x3,y3)
	local temp2 = cc.pGetDistance(cc.p(x2,y2),cc.p(x3,y3)) --dist(x2,y2,x3,y3)
	return 2*temp1/temp2
end

--点(x1,y1)到直线(x2,y2,x3,y3)的垂足
function chuizu( x1, y1, x2, y2, x3, y3)
	if(x2 == x3) then --竖着的一直线
		return cc.p(x2,y1)
	elseif(y2 == y3) then --横着的一直线
		return cc.p(x1,y2)
	else 
		float k = (y3-y2)/(x3-x2)
		float xx = (k*k*x2 + k * (y1-y2) + x1)/(k*k+1)
		float yy = k*(xx - x2) + y2
		return cc.p(xx,yy)
	end
end


function sign(x)
	if(x>0.0) then
		return 1
	else
		return -1
	end
end
------------------------------------
--生成发射球角度
function isAngle(dxNew, dyNew, color)
    local a1 = math.atan((dxNew - bubpos.x) / (dyNew - bubpos.y))
	local r = 90 - a1 * 360 / (2 * math.pi)
    if isSteel(color) then
        if r < 55 or r > 125 then 
            return true
        end
	elseif r < 20 or r > 160 then
		return true
	end
	return false
end

--生成发射球角度
function getAngles(dxNew, dyNew, color)
    local a1 = math.atan((dxNew - bubpos.x) / (dyNew - bubpos.y))
	local r = 90 - a1 * 360 / (2 * math.pi)
    return r
end

--箭头角度
function isHeadAngle(dxNew, dyNew)
	local a1 = math.atan((dxNew - bubpos.x) / (dyNew - bubpos.y))
    return a1 * 360 / (2 * math.pi)
end

--生成发射球的偏移量
function angleOffset(dxNew, dyNew)
	local a1 = math.atan((dxNew - bubpos.x) / (dyNew - bubpos.y))
	local r = 90 - a1 * 360 / (2 * math.pi)
	local mDX = speNums.speed * math.cos(r * math.pi / 180)
	local mDY = speNums.speed * math.sin(r * math.pi / 180)
	return mDX, mDY
end

--扩展内存数据表
function resetTable()
	if pot > #sours then
		for j = #sours + 1, pot do
			sours[j] = 0
			curBubLevel[j] = 0
		end
	end
end

--清理内存
function clearMemory()
	collectgarbage("collect")
	-- director:purgeCachedData()
    --CCTextureCache:sharedTextureCache():removeAllTextures()
end

function table.RCkeyof(hashtable, value)
    for k, v in pairs(hashtable) do
        if tonumber(v.m_nRow) == tonumber(value.m_nRow) and tonumber(v.m_nCol) == tonumber(value.m_nCol) then return k end
    end
    return nil
end

-- 是否和左右边缘碰撞
function isCollisionWithBorder(m_curReady)
	local pos = cc.p(m_curReady:getPositionX(),m_curReady:getPositionY())

	if (pos.x < BUBBLE_RADIUS or pos.x > display.size.width - BUBBLE_RADIUS) then
		return true
	end

	return false
end

-- 是否和顶层边缘碰撞，
function isCollisionWithTopBorder(pBubble)
	if not pBubble then
		return false
	end

	local pos = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
	if (pos.y > display.size.height - BUBBLE_RADIUS) then
		return true
	end

	return false
end

--是否和上方的球碰撞
function isCollisionWithBubble(pos1, radius1, pos2, radius2)
	return math.pow(pos1.x - pos2.x, 2) + math.pow(pos1.y - pos2.y, 2) < math.pow(radius1 + radius2, 2)
end

-- 是否碰撞，仅包括是否和上方所有的球碰撞和顶层边缘碰撞， 不包括和左右边缘碰撞
function isCollision(m_curReady,m_listBubble)
	local bRet = false

	local size = display.size
	if (m_curReady:getPositionY() > size.height - BUBBLE_RADIUS) then
		--找出与第一行的哪个相撞
		for _,bubble in pairs(m_listBubble) do
			if pBubble and bubble:getRow() == 0 then
				local pos1 = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
				local pos2 = cc.p(m_curReady:getPositionX(),m_curReady:getPositionY())
				if isCollisionWithBubble(pos1, BUBBLE_RADIUS - COLLISON_OFFSET, pos2, BUBBLE_RADIUS - COLLISON_OFFSET) then
					bRet = true
					return bRet,{m_nRow = pBubble:getRow(),m_nCol = pBubble:getCol() }
				end
			end
		end
		print("碰撞到顶部但第一行却没有相撞的球")
		bRet = true
		return bRet
	end

	local temp = {}
	for _,v in pairs(m_listBubble) do
		table.insert(temp,v)
	end

	local len = table.nums(temp)
	for i = len, 1, -1 do
		local pBubble = temp[i]
		local pos1 = cc.p(pBubble:getPositionX(),pBubble:getPositionY())
		local pos2 = cc.p(m_curReady:getPositionX(),m_curReady:getPositionY())
		if pBubble and isCollisionWithBubble(pos1, BUBBLE_RADIUS - COLLISON_OFFSET, pos2, BUBBLE_RADIUS - COLLISON_OFFSET) then
			bRet = true
			return bRet,{m_nRow = pBubble:getRow(),m_nCol = pBubble:getCol() }
		end
	end

	return bRet
end

function createBubble(color)
	local pBubble = BublleSprite.new(BublleBasic.new({color = color}))
	return pBubble
end

function randomCreateColor()
	local t = randomInt(1, MAX_RANDOM) + CUR_RANDOM
	local color = t >= MAX_COLOR and (t% MAX_COLOR +1) or t
	return color
end


function randomBubble()
	local color = randomCreateColor()
	local pBubble = BublleSprite.new(BublleBasic.new({color = color}))
	return pBubble
end

function getPosByRowAndCol(row, col)
	local posX, posY, size

	size = display.size

	posX = col * 2 * BUBBLE_RADIUS + BUBBLE_RADIUS + (row % 2) * BUBBLE_RADIUS
	posY = size.height - ( row * 2 * BUBBLE_RADIUS * math.sin(PI/3) + BUBBLE_RADIUS )

	return cc.p(posX, posY)

end

function GetRowColByPos( nPosX, nPosY)
	local nRow, nCol
	--需要四舍五入
	nPosY = display.size.height - nPosY 

	nRow = ( nPosY - BUBBLE_RADIUS )/( 2 *BUBBLE_RADIUS *math.sin ( PI/3 ) ) +0.5

	nCol = ( nPosX - ( nRow % 2 ) * BUBBLE_RADIUS - BUBBLE_RADIUS ) /(  2 *BUBBLE_RADIUS ) +0.5
	nCol = nCol < 0 and 0 or nCol
	return math.floor(nRow), math.floor(nCol)
end

--@获得周围停靠位置的列表nRow,nCol为要计算的停靠位置，vecPos返回它周围的位置
function GetAround( nRow, nCol, vecPos )
	if (not IsValidPos(nRow, nCol)) then
		return
	end

	--同一行
	if (IsValidPos(nRow, nCol - 1)) then
		table.insert(vecPos,{m_nRow = nRow, m_nCol = nCol - 1})
	end
	if (IsValidPos(nRow, nCol + 1)) then
		table.insert(vecPos,{m_nRow = nRow, m_nCol = nCol + 1})
	end

	-- 左斜线
	if (IsValidPos(nRow - 1, nCol)) then
		table.insert(vecPos,{m_nRow = nRow - 1, m_nCol = nCol})
	end
	if (IsValidPos(nRow + 1, nCol)) then
		table.insert(vecPos,{m_nRow = nRow + 1, m_nCol = nCol})
	end

	-- 如果是单行
	local curCol
	if (nRow % 2 == 0) then
		curCol = nCol - 1
	else
		curCol = nCol + 1
	end

	-- 右斜线
	if (IsValidPos(nRow - 1, curCol)) then
		table.insert(vecPos,{m_nRow = nRow - 1, m_nCol = curCol})
	end
	if (IsValidPos(nRow + 1, curCol)) then
		table.insert(vecPos,{m_nRow = nRow + 1, m_nCol = curCol})
	end

end

function GetGoldenLine( nRow,  nCol, listRowCol)	
	if (not IsValidPos(nRow, nCol)) then
		return
	end

	local nextRowCol = {m_nRow = nRow, m_nCol = nCol}
	local nextFlag = false
	
	table.insert(listRowCol,nextRowCol)
	
	-- 左上角
	repeat
		if (nextRowCol.m_nRow % 2 == 0) then
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
			nextRowCol.m_nCol = nextRowCol.m_nCol - 1
		else
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
		end

		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)
	-- 右下角
	nextRowCol.m_nRow = nRow
	nextRowCol.m_nCol = nCol
	repeat
		if (nextRowCol.m_nRow % 2 == 0) then
			nextRowCol.m_nRow = nextRowCol.m_nRow + 1
		else
			nextRowCol.m_nRow = nextRowCol.m_nRow + 1
			nextRowCol.m_nCol = nextRowCol.m_nCol + 1
		end
	
		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)

	-- 同一行
	nextRowCol.m_nRow = nRow
	nextRowCol.m_nCol = -1
	repeat
		nextRowCol.m_nCol = nextRowCol.m_nCol + 1
	
		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)	
	-- 右上角
	nextRowCol.m_nRow = nRow
	nextRowCol.m_nCol = nCol
	repeat
		if (nextRowCol.m_nRow % 2 == 0) then
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
		else
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
			nextRowCol.m_nCol = nextRowCol.m_nCol + 1
		end
	
		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)
	
	-- 左下角
	nextRowCol.m_nRow = nRow
	nextRowCol.m_nCol = nCol
	repeat
		if (nextRowCol.m_nRow % 2 == 0) then
			nextRowCol.m_nRow = nextRowCol.m_nRow + 1
			nextRowCol.m_nCol = nextRowCol.m_nCol - 1
		else
			nextRowCol.m_nRow = nextRowCol.m_nRow + 1
		end
	
		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)

end

--得到整条斜线上球的集合，分为左右斜线之分，左斜线代表从左上角到右下角， 右斜线代表从右上角到左下角,由mode值来区分
function GetSliveryLine(nRow, nCol, listRowCol)
	if (not IsValidPos(nRow, nCol)) then
		return
	end

	local nextRowCol = {m_nRow = nRow, m_nCol = nCol}
	local nextFlag = false

	table.insert(listRowCol,nextRowCol)

	-- 左上角
	repeat
		if (nextRowCol.m_nRow % 2 == 0) then
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
			nextRowCol.m_nCol = nextRowCol.m_nCol - 1
		else
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
		end

		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)

	-- 同一行
	nextRowCol.m_nRow = nRow
	nextRowCol.m_nCol = -1
	repeat
		nextRowCol.m_nCol = nextRowCol.m_nCol + 1

		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)

	-- 右上角
	nextRowCol.m_nRow = nRow
	nextRowCol.m_nCol = nCol
	repeat
		if (nextRowCol.m_nRow % 2 == 0) then
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
		else
			nextRowCol.m_nRow = nextRowCol.m_nRow - 1
			nextRowCol.m_nCol = nextRowCol.m_nCol + 1
		end

		if (IsValidPosRC(nextRowCol)) then
			table.insert(listRowCol,nextRowCol)
			nextFlag = true
		else
			nextFlag = false
		end
	until (nextFlag)
end

--@一个位置是否有效的停靠点
function IsValidPos( nRow, nCol )
	if (nRow < 0 or nCol < 0) then
		return false
	end

	if nCol >= (MAX_COLS - nRow % 2) then
		return false
	end
	-- 这里行数有可能增加
	-- if (nRow >= MAX_ROWS || nCol >= MAX_COLS - nRow % 2)
	-- {
	-- 	return false
	-- }

	return true
end

function IsValidPosRC(nRownCol)
	return IsValidPos(nRownCol.m_nRow, nRownCol.m_nCol)
end
