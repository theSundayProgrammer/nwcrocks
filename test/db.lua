local rocksdb = require("nwcrocks")

for k,v in pairs(rocksdb) do
  print(k..": "..tostring(v))
end

local options = {
  increase_parallelism = 1,
  create_if_missing = true
}


local db = rocksdb.open(options, "/tmp/rocksdb.test")


db:close()

