require("TaxasTable")

ALLCard = {
	2,2,2,2,		-- 1-4
	3,3,3,3,		-- 5-8
	4,4,4,4,		-- 9-12
	5,5,5,5,		-- 13-16
	6,6,6,6,		-- 17-20
	7,7,7,7,		-- 21-24
	8,8,8,8,		-- 25-28
	9,9,9,9,		-- 29-32
	10,10,10,10,		-- 33-36
	11,11,11,11,		-- 37-40
	12,12,12,12,		-- 41-44
	13,13,13,13,		-- 45-48
	14,14,14,14,		-- 49-52
}

-- 随机获取几个玩家的牌, 返回牌ALLCard的索引
function RandCard(count)
	local tTotal = 5 + count * 2;
	local tCount = 0;
	local tCards = {};
	while tCount < tTotal do
		local tIndex = math.random(1, 52);
		if tCards[tIndex] == nil then
			tCards[tIndex] = 1;
			tCount = tCount + 1;
		end
	end
	
	return tCards;
end

function GetCards(tableInfo)
	local tCards = RandCard(tableInfo["PlayerCount"]);
	local tCount = 0;
	local tFull = 2;
	local tIndex = 0;
	for i, v in pairs(tCards) do
		tCount = tCount + 1;
		if tCount <= 5 then
			table.insert(tableInfo["DownCards"], i);
		else
			if tFull == 2 then
				tIndex = getNextPlayer(tIndex);
				tFull = 0;
			end
			table.insert(tableInfo["Players"][tIndex]["OwnCard"], i);
			tFull = tFull + 1;
		end
	end
end
