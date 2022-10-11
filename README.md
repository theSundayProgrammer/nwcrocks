lua-rocksdb
=
Lua binding for RocksDB

Installation
===
* Install RocksDB (make shared_lib, make static_lib), download from: https://github.com/facebook/rocksdb
* Update the Makefile if needed to point at the correct include/lib paths

Example
===

```lua
  local rocksdb = require("rocksdb")
  local options = {
    create_if_missing = true
  }
  local db = rocksdb.open(options, "/tmp/test.rocksdb")
  db:put( "key", "value")
  print(db:get( "key"))
  db:close()
```

Documentation
===
This implementation differs from [Zaherm's version (https://github.com/zaherm/lua-rocksdb) in a few ways:
- There is no options object. The options is specified as a table on openning a database
- Supports Column Families which are identing as string names. "Default" is a required first column name.


rocksdb.open(options, db_path)
=====
  * **options** - table of options as listed below; may be an empty table
  * **db_path** - string. path
  * returns *rocksdb* object (referenced below as **db**)

options
=====
    Listed below are the available options. Checks RocksDB docs to find out what they mean.
    Invalid option keys are ignored. 
    * "increase_parallelism" = <int>
    *  "info_log_level"= between 0 and 5 as listed below
            -  0: DEBUG;
            -  1: INFO;
            -  2: WARN;
            -  3: ERROR;
            -  4: FATAL;
            -  5: HEADER;
    * "max_open_files" = <int>
    * "num_levels" = <int> 
    * "level0_file_num_compaction_trigger" = <int>
    * "level0_slowdown_writes_trigger" = <int>
    * "level0_stop_writes_trigger" = <int>
    * "target_file_size_multiplier" = <int>
    * "max_write_buffer_number" = <int>
    * "min_write_buffer_number_to_merge" = <int>
    * "max_write_buffer_number_to_maintain" = <int>
    * "max_background_compactions" = <int>
    * "max_background_flushes" = <int>
    * "table_cache_numshardbits" = <int>
    * "use_fsync" = <int>
    * "access_hint_on_compaction_start" = between 0 and 4 as listed below
              0: NONE ;
              1: NORMAL;
              2: SEQUENTIAL;
              3: WILLNEED ;
    * "disable_auto_compactions" = <int>
    * "report_bg_io_stats" = <int>
    * "compression" = between 0 and 9 as listed below
                0 : No Compression;
                1 : Snappy Compression;
                2 : Zlib Compression;
                3 : BZip2 Compression;
                4 : LZ4 Compression;
                5 : LZ4H CCompression;
                6 : Xpress Compression;
                7 : ZSTD;
                8 : ZSTDNotFinal Compression;
                9 : Disable CompressionOption;
    * __bool options__ 
    * "create_if_missing" = <bool>
    * "create_missing_column_families" = <bool>
    * "error_if_exists" = <bool>
    * "paranoid_checks" = <bool>
    * "allow_mmap_reads" = <bool>
    * "allow_mmap_writes" = <bool>
    * "is_fd_close_on_exec" = <bool>
    * "advise_random_on_open" = <bool>
    * "use_adaptive_mutex" = <bool>
    * "inplace_update_support" = <bool>
    *  __uint64 options__
    * "optimize_for_point_lookup" = <unit64>
    * "optimize_level_style_compaction" = <unit64>
    * "optimize_universal_style_compaction" = <unit64>
    * "max_total_wal_size" = <uint64>
    * "target_file_size_base" = <uint64>
    * "max_bytes_for_level_base" = <uint64>
    * "WAL_ttl_seconds" = <uint64>
    * "WAL_size_limit_MB" = <uint64>
    * "bytes_per_sync" = <uint64>
    * "max_sequential_skip_in_iterations" = <uint64>
    * "delete_obsolete_files_period_micros" = <uint64>

db:put( key, value)
=====
Execute a put command, key-value (both of string type).

  * **key** - string
  * **value** - string

db:get( key)
=====
Gets a value from the DB.

  * **key** - string



db:close()
=====
Closes the instance (it doesn't destroy the DB!)

rocksdb.backup_engine( backup_path)
=====
Creates a backup engine object.

  * **backup_path** - string - backup path
  * returns *backup_engine* object

db:create_new_backup()
=====
Creates a back from *db*.

  * returns true on success, error on failure

backup_engine:purge_old_backups(number)
=====
Purge *number* of old backups.


backup_engine:get_backup_info()
=====
  * returns **table** of <key,value> pairs

_keys_

* **id**
* **timestamp**
* **size**
* **number_files**


backup_engine:restore_db_from_latest_backup(db_path, wal_dir )
=====
Restores DB from the latest backup.

* **db_path** (string)
* **wal_dir** (string)
Returns an error on failure

backup_engine:close()
=====
Close/free the backup_engine instance

db:batch_begin()
=====
Creates a *writebatch* instance.

writebatch:put(key, value)
=====
Puts a *key* with its *value* into the batch (both are strings).

writebatch:count()
=====
Returns the *count* of key-values pairs in the batch.

writebatch:write()
=====
Write the batch 

writebatch:clear()
=====
Clears the writebatch.

writebatch:destroy()
=====
Frees the writebatch instance.


db:iterator()
=====
Returns an iterator object.

iterator:valid()
=====
Returns _true_ if the iterator is valid, _false_ otherwise.

iterator:seek_to_first(), iterator:seek_to_last()
=====
Seek to first/last iterm.


iterator:seek(key)
=====
Seek specific _key_.

iterator:next(), iterator:prev()
=====
Move to next/prev item.

iterator:key(), iterator:value()
=====
Returns _string_ key/value.

iterator:get_error()
=====
Returns _string_ error message of the iterator.

iterator:destroy()
=====
Frees the iterator instance.
