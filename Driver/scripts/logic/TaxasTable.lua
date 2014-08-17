PlayerTableInfo = {
	-- 持久化信息
	["Uid"] = 0,		  -- 玩家id
	["Money"] = 0,		  -- 拥有的金额
	
	-- 游戏中信息
    	["OwnCard"] = {},         -- 手里的牌
    	["SidePool"] = {},        -- 边池
    	["CurBet"] = 0,           -- 当轮投注
    	["GiveUp"] = 0,           -- 是否放弃
    	["AllIn"] = 0,            -- 全压钱数
}

TaxasTableInfo = {
	["Inited"] = 0,		  -- 初始化标示
	["MinMoney"] = 0,	  -- 进入桌子最小金额
	["MinPlayerCount"] = 0,	  -- 最小玩家数
    	["MaxPlayerCount"] = 0,   -- 最大玩家数
	["BigBlinds"] = 0,        -- 大盲注
    	["SmallBlinds"] = 0,      -- 小盲注
}

TaxasTable = {
	["Gameing"] = 0,	  -- 游戏中标示
    	["TableInfo"] = nil,      -- 桌子信息

    	["Players"] = {},         -- 所有玩家
	["PlayerCount"] = 0,	  -- 所有玩家个数

    	["ButtonPlayer"] = 0,  	  -- 当前庄家
    	["AllInPlayers"] = {},    -- 全压的玩家
	["DownCards"] = {},	  -- 底牌
	["CurMaxBet"] = 0,	  -- 当前最大投注数
	["CurMaxRaise"] = 0,	  -- 当前最大加注数
    	["Pot"] = 0,              -- 底池
	["CurPlayer"] = 0,	  -- 当前下注玩家
}

function checkTableInited()
	if TaxasTable["TableInfo"] == nil then
		return nil;
	end
	if TaxasTable["TableInfo"]["Inited"] == 0 then
		return nil;
	end
	return 1;
end

-- 获取某个位置上的下一位玩家
function getNextPlayer(index)
	print("DEBUG, index : " .. index)

	local tIndex = 0;
	local tMaxPlayerCount = TaxasTable["TableInfo"]["MaxPlayerCount"];
	
	tIndex = index + 1;
	while tIndex <= tMaxPlayerCount do
		local tPlayer = TaxasTable["Players"][tIndex];
		if tPlayer ~= nil then
			return tIndex;
		end
		tIndex = tIndex + 1;
	end

	tIndex = 1;
	while tIndex < index do
		local tPlayer = TaxasTable["Players"][tIndex];
		if tPlayer ~= nil then
			return tIndex;
		end
		tIndex = tIndex + 1;
	end
	
	print("DEBUG, none next player")
	return nil;
end

-- 下一个庄主
function NextButtonPlayer()
	if checkTableInited(TaxasTable) == nil then
		return nil;
	end
	print("DEBUG,CurIndex:" .. TaxasTable["ButtonPlayer"]);
	local tPlayerSize = table.maxn(TaxasTable);
	-- 设置第一个玩家为庄主
	if TaxasTable["ButtonPlayer"] == 0 then
		local tAllPlayers = TaxasTable["Players"];
		for i,v in pairs(tAllPlayers) do
			TaxasTable["ButtonPlayer"] = i;
			break;
		end
		print("DEBUG,NextButton is " .. TaxasTable["ButtonPlayer"]);
	else
		local tCurIndex = TaxasTable["ButtonPlayer"];
		-- 重新设置庄主
		if tCurIndex >= tPlayerSize then
			for i,v in TaxasTable["Players"] do
                        	TaxasTable["ButtonPlayer"] = i;
                            	break;
		        end
		-- 找到下一个玩家
		else
			while tCurIndex < tPlayerSize do
				tCurIndex = tCurIndex + 1;
				local tPlayer = TaxasTable["Players"][tCurIndex]
				if tPlayer ~= nil then
					TaxasTable["ButtonPlayer"] = tCurIndex;
					break;
				end 
			end
		end
		print("DEBUG,NextButton is " .. TaxasTable["ButtonPlayer"]);
	end
end

-- 找出一个空位置
function FindEmptyIndex()
        if checkTableInited(TaxasTable) == nil then
                return nil;
        end
	local tMaxCount = TaxasTable["TableInfo"]["MaxPlayerCount"];
	
	for i = 1, tMaxCount do
		if TaxasTable["Players"][i] == nil then
			return i;
		end
	end
	return nil;
end

-- 小盲注位置
function SmallBlindsPlayer()
	if checkTableInited(TaxasTable) == nil then
		return nil;
	end

	local tCurButton = TaxasTable["ButtonPlayer"];
	return getNextPlayer(tCurButton);
end

-- 大盲注位置
function BigBlindsPlayer()
	if checkTableInited(TaxasTable) == nil then
		return nil;
	end

	local tSmallBlindsIndex = SmallBlindsPlayer(TaxasTable);
	if tSmallBlindsIndex == nil then
		return nil;
	end

	return getNextPlayer(tSmallBlindsIndex);
end

-- 设置当前下注玩家
function SetCurPlayer(t)
	if t == 0 then		-- 第一轮开始
		if TaxasTable["CurPlayer"] == 0 then
			local tIndex = BigBlindsPlayer();
			TaxasTable["CurPlayer"] = getNextPlayer(tIndex);
		end
	elseif t == 1 then	-- 每轮的下一个玩家
		local tCurIndex = TaxasTable["CurPlayer"];
		while 1 do
			tCurIndex = getNextPlayer(tCurIndex);
			if TaxasTable["AllInPlayers"][tCurIndex] == nil then
				TaxasTable["CurPlayer"] = tCurIndex;
				break;
			end
		end
	else			-- 之后每论开始
		TaxasTable["CurPlayer"] = SmallBlindsPlayer();
	end
end

-- 查询玩家位置
function GetPlayerIndex(uid)
	local tPlayers = TaxasTable["Players"];
	if tPlayers == nil then
		return nil;
	end

	for i, v in pairs(tPlayers) do
		if v["Uid"] == uid then
			return i;
		end
	end
	
	return nil;
end
