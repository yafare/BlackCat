require("TaxasTable")
require("TaxasRedis")
require("TaxasCard")

StartGameInfo = {
	["PlayerCards"] = { {} },	-- [uid]={crad. card}
}

-- 初始化桌子
function InitTable()
	math.randomseed(os.time());
end

-- 重置桌子
function ResetTable()
	TaxasTable["Gameing"] = 1;
	TaxasTable["AllInPlayers"] = {};
	TaxasTable["DwonCards"] = {};
	TaxasTable["CurMaxBet"] = 0;
	TaxasTable["CurMaxRaise"] = 0;
	TaxasTable["Pot"] = 0;
	TaxasTable["CurPlayer"] = 0;

	local tPlayers = TaxasTable["Players"];
	for i, v in pairs(tPlayers) do
		v["OwnCard"] = {};
		v["SidePool"] = {};
		v["CurBet"] = 0;
		v["GiveUp"] = 0;
		v["AllIn"] = 0;
	end
end

-- 检测桌子是否准备好
function CheckTableReady()
	if TaxasTable["Gameing"] == 1 then
		return nil;
	end
	if TaxasTable["PlayerCount"] < TaxasTable["TableInfo"]["MinPlayerCount"] then
		return nil;
	end
	
	return 0;
end

-- 检测桌子里的玩家是否符合条件
function CheckTablePlayers()
	local tMovePlayers = {};
	for i, v in pairs(TaxasTable["Players"]) do
		if v["Money"] < TaxasTable["TableInfo"]["MinMoney"] then
		table.insert(tMovePlayers, v);
	end
	if table.getn(tMovePlayers) <= 0 then
		return nil;
	end
	
	return tMovePlayers;
end

-- 玩家进入桌子
function EnterTable(uid)
	-- 获取玩家信息
	local playerInfo = RedisGetPlayerInfo(uid);
	if playerInfo == nil then
		return nil;
	end

	-- 获取一个空位置
	local tEmptyIndex = FindEmtyIndex();
	if tEmptyIndex == nil then
		return nil;
	end
	-- 设置玩家位置
	TaxasTable["Players"][tEmptyIndex] = playerInfo;
	-- 增加一位玩家
	TaxasTable["PlayerCount"] = TaxasTable["PlayerCount"] + 1;
end

-- 比赛开始
function StartTable()
	-- 检测桌子是否准备好
	if CheckTableReady() == nil then
		return nil;
	end
	-- 重置桌子
	ResetTable();
	-- 设置庄主位置
	NextButtonPlayer();
	-- 发牌
	GetCards(TaxasTable); 
	-- 选择大盲注左侧玩家下注
	SetCurPlayer(0);
end

--
function OnFlod(uid)
	local tIndex = GetPlayerIndex(uid);
	if tIndex == nil then
		return nil;
	end
	
	
end

function OnCall(uid)

end

function OnRaise(uid)

end
