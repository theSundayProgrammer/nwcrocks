local obj = MyObject(42)
print(obj:get())    -- 42
local obj2 = MyObject(4)
print(obj2:get())    -- 42
obj2:set(-1.25)
print(obj2:get())    -- -1.5

