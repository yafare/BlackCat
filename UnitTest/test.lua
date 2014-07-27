
function Test(id)
    print('Test')

    local n = uint64(2)
    local i = id * n
    local s = 'uint64: ' .. tostring(i)
    print(s)
    return i
end

function TestTable(id, fishes)
    print('TestTable')

    for k, v in pairs(fishes) do
        print(k .. ': ' .. v.type .. ' ' .. v.count)
    end
    return { x = 1, y = 2 }
end
