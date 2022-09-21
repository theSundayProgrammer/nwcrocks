print("***** start: writebatch test *****")

local rocksdb = require("nwcrocks")
local format = string.format

local options = {
    increase_parallelism = 1,
    create_if_missing = true,
    create_missing_column_families = true
}

local column_families = {"default", "columna"}

local db = rocksdb.open_cf(options, "/tmp/rocks_cf.test", column_families)
print("**Opened database-batch test *****")
local writebatch = db:batch_begin("columna")
print("**begin -batch test *****")
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

