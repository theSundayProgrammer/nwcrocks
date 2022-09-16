
local rocksdb = require("nwcrocks")


local options = {
    increase_parallelism = 1,
    create_if_missing = true,
    create_missing_column_families = true
}

local column_families = {"default", "columna"}

local db = rocksdb.open_cf(options, "/tmp/rocksdb.test", column_families)


db:close()

