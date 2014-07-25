
function Test(id)
    print(id)
end

function TestTable(id, fishes)
    for k, v in pairs(fishes) do
        print(k .. ': ' .. v.type .. ' ' .. v.count)
    end
    return { x = 1, y = 2 }
end
