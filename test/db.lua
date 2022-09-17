local rocksdb = require("nwcrocks")
local format = string.format
for k,v in pairs(rocksdb) do
  print(k..": "..tostring(v))
end

local options = {
  increase_parallelism = 1,
  create_if_missing = true
}


local db = rocksdb.open(options, "/tmp/rocks_db.test")

print("start: put")
for i = 0, 1000 do
  key = format("lrocks_db_key:%d", i)
  value = format("lrocks_db_value:%d", i)
  db:put( key, value)
end
print("done: put")

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
print("delete: end", key, type(value), value, "-")
assert(value == nil)
db:close()

