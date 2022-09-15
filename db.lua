local rocksdb = require("nwcrocks")

for k,v in pairs(rocksdb) do
  print(k..": "..tostring(v))
end



local db = rocksdb.open("/tmp/rocksdb.test")
db:close()

