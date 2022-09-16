#include "db.h"

 ROCKSDB_NAMESPACE::Options options_from_table(lua_State *L, int index) {
 ROCKSDB_NAMESPACE::Options options;
  lua_pushvalue(L, index);
  lua_pushnil(L);
  while (lua_next(L, -2))
  {
  int opt_int;
  uint64_t opt_uint64;
  unsigned char opt_bool;
    lua_pushvalue(L, -2);
    const char *key = lua_tostring(L, -1);
    /* int options */
    if(strcmp(key, "increase_parallelism") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.IncreaseParallelism( opt_int);
    }
    else if(strcmp(key, "info_log_level") == 0) {
      opt_int = luaL_checkint(L, -2);
     // options.info_log_level = opt_int;
    }
    else if(strcmp(key, "max_open_files") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.max_open_files = opt_int;
    }
    else if(strcmp(key, "num_levels") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.num_levels = opt_int;
    }
    else if(strcmp(key, "level0_file_num_compaction_trigger") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.level0_file_num_compaction_trigger= opt_int;
    }
    else if(strcmp(key, "level0_slowdown_writes_trigger") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.level0_slowdown_writes_trigger= opt_int;
    }
    else if(strcmp(key, "level0_stop_writes_trigger") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.level0_stop_writes_trigger= opt_int;
    }
    else if(strcmp(key, "target_file_size_multiplier") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.target_file_size_multiplier= opt_int;
    }
    else if(strcmp(key, "max_write_buffer_number") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.max_write_buffer_number= opt_int;
    }
    else if(strcmp(key, "min_write_buffer_number_to_merge") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.min_write_buffer_number_to_merge= opt_int;
    }
    else if(strcmp(key, "max_write_buffer_number_to_maintain") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.max_write_buffer_number_to_maintain= opt_int;
    }
    else if(strcmp(key, "max_background_compactions") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.max_background_compactions= opt_int;
    }
    else if(strcmp(key, "max_background_flushes") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.max_background_flushes= opt_int;
    }
    else if(strcmp(key, "table_cache_numshardbits") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.table_cache_numshardbits= opt_int;
    }
    else if(strcmp(key, "use_fsync") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.use_fsync= opt_int;
    }
    else if(strcmp(key, "access_hint_on_compaction_start") == 0) {
      opt_int = luaL_checkint(L, -2);
      //options.access_hint_on_compaction_start= opt_int;
    }
    else if(strcmp(key, "disable_auto_compactions") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.disable_auto_compactions= opt_int;
    }
    else if(strcmp(key, "report_bg_io_stats") == 0) {
      opt_int = luaL_checkint(L, -2);
      options.report_bg_io_stats= opt_int;
    }
    else if(strcmp(key, "compression") == 0) {
      opt_int = luaL_checkint(L, -2);
     // options.compression= opt_int;
    }
    else if(strcmp(key, "compaction_style") == 0) {
      opt_int = luaL_checkint(L, -2);
      //options.compaction_style= opt_int;
    }
    /* bool options */
    else if(strcmp(key, "create_if_missing") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.create_if_missing= opt_bool;
    }
    else if(strcmp(key, "create_missing_column_families") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.create_missing_column_families= opt_bool;
    }
    else if(strcmp(key, "error_if_exists") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.error_if_exists= opt_bool;
    }
    else if(strcmp(key, "paranoid_checks") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.paranoid_checks= opt_bool;
    }
    else if(strcmp(key, "allow_mmap_reads") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.allow_mmap_reads= opt_bool;
    }
    else if(strcmp(key, "allow_mmap_writes") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.allow_mmap_writes= opt_bool;
    }
    else if(strcmp(key, "is_fd_close_on_exec") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.is_fd_close_on_exec= opt_bool;
    }
    else if(strcmp(key, "advise_random_on_open") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.advise_random_on_open= opt_bool;
    }
    else if(strcmp(key, "use_adaptive_mutex") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.use_adaptive_mutex= opt_bool;
    }
    else if(strcmp(key, "inplace_update_support") == 0) {
      opt_bool = lua_toboolean(L, -2);
      options.inplace_update_support= opt_bool;
    }
    /* uint64 options */
    else if(strcmp(key, "optimize_for_point_lookup") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.OptimizeForPointLookup(opt_uint64);
    }
    else if(strcmp(key, "optimize_level_style_compaction") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.OptimizeLevelStyleCompaction (opt_uint64);
    }
    else if(strcmp(key, "optimize_universal_style_compaction") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.OptimizeUniversalStyleCompaction (opt_uint64);
    }
    else if(strcmp(key, "max_total_wal_size") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.max_total_wal_size=opt_uint64;
    }
    else if(strcmp(key, "target_file_size_base") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.target_file_size_base=opt_uint64;
    }
    else if(strcmp(key, "max_bytes_for_level_base") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.max_bytes_for_level_base=opt_uint64;
    }
    else if(strcmp(key, "WAL_ttl_seconds") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.WAL_ttl_seconds=opt_uint64;
    }
    else if(strcmp(key, "WAL_size_limit_MB") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.WAL_size_limit_MB=opt_uint64;
    }
    else if(strcmp(key, "bytes_per_sync") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.bytes_per_sync=opt_uint64;
    }
    else if(strcmp(key, "max_sequential_skip_in_iterations") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.max_sequential_skip_in_iterations=opt_uint64;
    }
    else if(strcmp(key, "delete_obsolete_files_period_micros") == 0) {
      opt_uint64 = luaL_checknumber(L, -2);
      options.delete_obsolete_files_period_micros=opt_uint64;
    }
    lua_pop(L, 2);
  }
  lua_pop(L, 1);
  return options;
}
