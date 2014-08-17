require("TaxasTable")
require("TaxasCard")

-- #################### unit test by lyl #########################
TaxasTableInfo["Inited"] = 1
TaxasTable["TableInfo"] = TaxasTableInfo
TaxasTable["Players"][1] = {["name"] = "11",}

NextButtonPlayer()

if TaxasTable["ButtonPlayer"] ~= 1 then
	SDM_Assert();
end

TaxasTableInfo["Inited"] = 0
TaxasTable["TableInfo"] = nil
TaxasTable["Players"][1] = nil
-- #################### end ##########################

-- #################### unit test by lyl #########################
TaxasTableInfo["Inited"] = 1
TaxasTable["TableInfo"] = TaxasTableInfo
TaxasTable["Players"][1] = {["name"] = "11",}
TaxasTable["Players"][2] = {["name"] = "22",}
TaxasTable["Players"][4] = {["name"] = "44",}
TaxasTable["ButtonPlayer"] = 1;
TaxasTable["TableInfo"]["MaxPlayerCount"] = 9;

local tSmall = SmallBlindsPlayer()
if tSmall ~= 2 then
	SDM_Assert();
end

local tBig = BigBlindsPlayer()
if tBig ~= 4 then
	SDM_Assert();
end

TaxasTableInfo["Inited"] = 0
TaxasTable["TableInfo"]["MaxPlayerCount"] = 0;
TaxasTable["TableInfo"] = nil
TaxasTable["Players"][1] = nil
TaxasTable["Players"][2] = nil
TaxasTable["Players"][4] = nil
TaxasTable["ButtonPlayer"] = 0;
-- #################### end ##########################

-- #################### unit test by lyl #########################
TaxasTableInfo["Inited"] = 1
TaxasTable["TableInfo"] = TaxasTableInfo
TaxasTable["Players"][1] = {["name"] = "11",}
TaxasTable["Players"][2] = {["name"] = "22",}
TaxasTable["Players"][3] = {["name"] = "33",}
TaxasTable["ButtonPlayer"] = 1;
TaxasTable["TableInfo"]["MaxPlayerCount"] = 9;

local tEmpty= FindEmptyIndex()
if tEmpty~= 4 then
        SDM_Assert();
end

TaxasTableInfo["Inited"] = 0
TaxasTable["TableInfo"]["MaxPlayerCount"] = 0;
TaxasTable["TableInfo"] = nil
TaxasTable["Players"][1] = nil
TaxasTable["Players"][2] = nil
TaxasTable["Players"][3] = nil
TaxasTable["ButtonPlayer"] = 0;
-- #################### end ##########################

-- #################### unit test by lyl #########################
math.randomseed(os.time())
local tCount = 5
local tCards = RandCard(tCount)
local tSize = 0;
for i, v in pairs(tCards) do
	tSize = tSize + 1;
	print("count : " .. tCount .. ", size : " .. tSize .. ", card : " .. i .. ", " .. v)
end
if tSize ~= (tCount * 2 + 5) then
	SDM_Assert();
end
-- #################### end ##########################

-- #################### unit test by lyl #########################
TaxasTable["PlayerCount"] = 3
TaxasTable["Players"][1] = { ["OwnCard"] = {} }
TaxasTable["Players"][2] = { ["OwnCard"] = {} }
TaxasTable["Players"][4] = { ["OwnCard"] = {} }
TaxasTableInfo["Inited"] = 1
TaxasTableInfo["MaxPlayerCount"] = 9
TaxasTable["TableInfo"] = TaxasTableInfo

GetCards(TaxasTable)
for i, v in pairs(TaxasTable["DownCards"]) do
	print("----" .. v);
end
for i, v in pairs(TaxasTable["Players"]) do
	print("index : " .. i .. ", card1 : " .. v["OwnCard"][1] .. ", card2 : " .. v["OwnCard"][2])
end
-- #################### end ##########################

-- #################### unit test by lyl #########################
TaxasTable["PlayerCount"] = 3
TaxasTable["Players"][1] = { ["OwnCard"] = {} }
TaxasTable["Players"][2] = { ["OwnCard"] = {} }
TaxasTable["Players"][4] = { ["OwnCard"] = {} }
TaxasTable["Players"][5] = { ["OwnCard"] = {} }
TaxasTableInfo["Inited"] = 1
TaxasTableInfo["MaxPlayerCount"] = 9
TaxasTable["TableInfo"] = TaxasTableInfo
TaxasTable["ButtonPlayer"] = 1;

SetCurPlayer(0)
print("type0 : " .. TaxasTable["CurPlayer"]);
SetCurPlayer(1)
print("type1 : " .. TaxasTable["CurPlayer"]);
SetCurPlayer(2)
print("type2 : " .. TaxasTable["CurPlayer"]);
-- #################### end ##########################
