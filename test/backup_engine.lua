print("***** start: backup_engine test *****")
local rocksdb = require("nwcrocks")
local format = string.format
local db_path = "/tmp/rocks_db.test"

local options = {
  increase_parallelism = 1,
  create_if_missing = true
}

local db = rocksdb.open(options, db_path)
print("***** opened database *****")
local backup_engine = db:backup(db_path.."-backup")
assert(backup_engine:purge(2))
for k,v in ipairs (backup_engine:get_info()) do
    for k1,v1 in pairs(v) do
        print (k1 .. " = " ..v1)
    end
end


print("***** backed up  *****")
backup_engine:close()
print("***** done: backup_engine test *****")

