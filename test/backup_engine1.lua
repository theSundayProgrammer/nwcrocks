print("***** start: backup_engine test *****")
local rocksdb = require("nwcrocks")
local format = string.format
local db_path = "/tmp/rocks_db.test"

local options = {
  increase_parallelism = 1,
  create_if_missing = true
}

local backup_engine = rocksdb.create_backup_engine(db_path.."-backup")
print("***** opened backap *****")
local id =0
for k,v in ipairs (backup_engine:get_info()) do
    for k1,v1 in pairs(v) do
        print (k1 .. " = " ..v1)
    end
    if id < v.id then 
        id = v.id
    end
end
print ("v= ", id)
backup_engine:restore_latest(id,db_path,db_path)


print("***** restored  *****")
backup_engine:close()
print("***** done: backup_engine test *****")

