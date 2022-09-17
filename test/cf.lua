#!/usr/bin/env luajit
local rocksdb = require("nwcrocks")

local format = string.format

local options = {
    increase_parallelism = 1,
    create_if_missing = true,
    create_missing_column_families = true
}

local column_families = {"default", "columna"}

local db = rocksdb.open_cf(options, "/tmp/rocks_cf.test", column_families)

print("start: put")
for i = 0, 1000 do
  key = format("lrocks_db_key:%d", i)
  value = format("lrocks_db_value:%d", i)
  db:put( "columna" , key, value)
end
print("done: put")

print("start: get")
for i = 0, 1000 do
  key = format("lrocks_db_key:%d", i)
  expected_value = format("lrocks_db_value:%d", i)
  value = db:get("columna", key)
  assert(value == expected_value)
end
print("done: get")

print("delete: start", key)
key = "lrocks_db_key:delete_me"
db:put("columna" , key, "delete")
db:remove( "columna" , key)
value = db:get("columna" , key)
print("delete: end", key, type(value), value, "-")
assert(value == nil)
db:close()

