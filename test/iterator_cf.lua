#!/usr/bin/env luajit
local rocksdb = require("nwcrocks")

local format = string.format

local options = {
    increase_parallelism = 1,
    create_if_missing = true,
    create_missing_column_families = true
}

local column_families = {"default", "columna"}

local db = rocksdb.open_cf(options, "/tmp/rocks_iterator_cf.test", column_families)



local key, value, expected_value

for i = 0, 1000 do
  key = format("lrocks_db_key:%d", i)
  value = format("lrocks_db_value:%d", i)
  db:put( "columna", key, value)
end

local iterator = db:iterator("columna")
assert(iterator:valid() == false)
print("created new iterator")
iterator:seek_to_first()
assert(iterator:valid() == true)

print("seek to first")
local k =0
while iterator:valid() do
  key = iterator:key()
  value = iterator:value()
  iterator:next()
  k = k + 1
end
assert (k == 1001)
print("finished traverse")
iterator:destroy()
db:close()

