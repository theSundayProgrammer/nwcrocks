print("***** start: writebatch test *****")

local rocksdb = require("nwcrocks")
local format = string.format

local options = {
  increase_parallelism = 1,
  create_if_missing = true
}


local db = rocksdb.open(options, "/tmp/rocks_db.test")
local writebatch = db:batch_begin()
local key, val
for i = 1,10 do
  key = format("writebatch:key:%d", i)
  val = format("writebatch:val:%d", i)
  writebatch:put(key, val)
  assert(writebatch:count() == i)
end
writebatch:write()
writebatch:clear()
assert(writebatch:count() == 0)
writebatch:destroy()

--print("***** done:ddritebatch test *****")
db:close()

--[[
print("start: get")
for i = 0, 1000 do
  key = format("lrocks_db_key:%d", i)
  expected_value = format("lrocks_db_value:%d", i)
  value = db:get( key)
  assert(value == expected_value)
end
print("done: get")

print("delete: start", key)
key = "lrocks_db_key:delete_me"
db:put(key, "delete")
db:remove( key)
value = db:get(key)
lete: end", key, type(value), value, "-")
assert(value == nil)
--]]
