#define LROCKSDB_VERSION "nwc-rocksdb 0.0.1"
#define LROCKSDB_COPYRIGHT "Copyright (C) 2022 Joe Mariadassou"
#define LROCKSDB_DESCRIPTION "RocksDB C++ Lua binding"
#include "db.h"
#include <cstdio>
#include <string>


using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::PinnableSlice;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;
using ROCKSDB_NAMESPACE::WriteBatch;
using ROCKSDB_NAMESPACE::WriteOptions;
using ROCKSDB_NAMESPACE::ColumnFamilyHandle;

 ROCKSDB_NAMESPACE::Options options_from_table(lua_State *L, int index) ;
    int open_db(lua_State *L);
    int open_cf(lua_State *L);
    namespace {
        int get(lua_State* L) ;
        int put(lua_State* L) ;
        int close(lua_State *L);
        const struct luaL_Reg  db_reg[] = {
            { "get", get },
            { "put", put },
            { "close", close },
            { NULL, NULL }
        };

        const struct luaL_Reg  funcs[] = {
            { "open", open_db },
            { "open_cf", open_cf },
            { NULL, NULL }
        };

        struct rocks_db {
            DB* db=nullptr;
            bool open=false;
            Options options;
            ~rocks_db () {
                delete db;
            }
        };

        int close(lua_State *L) {
            int index = 1;
            rocks_db *d = (rocks_db*) luaL_checkudata(L, index, "db");
            luaL_argcheck(L, d != NULL && d->db != NULL, index, "db expected");
            d->~rocks_db(); ;

            return 1;
        }
        int put(lua_State* L) {
            using ROCKSDB_NAMESPACE::Slice;
            using lrocks::get_str;
            int argc=0;
            rocks_db *d = (rocks_db*) luaL_checkudata(L, ++argc, "db");
            std::string key = get_str(L, ++argc);
            std::string value = get_str(L, ++argc);
            Status s = d->db->Put(WriteOptions(), Slice(key), Slice(value));
            if(!s.ok()) {
                fprintf(stderr,"status = %s\n", s.getState());
                luaL_error(L, "failed to put");
                return 0;
            }
            return 1;
        }
        int get(lua_State* L) {
            using ROCKSDB_NAMESPACE::Slice;
            using lrocks::get_str;
            int argc=0;
            rocks_db *d = (rocks_db*) luaL_checkudata(L, ++argc, "db");
            std::string key = get_str(L, ++argc);
            std::string value ;
            Status s = d->db->Get(ReadOptions(), Slice(key), &value);
            if(!s.ok()) {
                fprintf(stderr,"status = %s\n", s.getState());
                luaL_error(L, "failed to get");
                return 0;
            }
            if(!value.empty() ) {
                lua_pushlstring(L, value.data(), value.size());
            }
            else {
                lua_pushnil(L);
            }
            return 1;
        }
    }
 int open_db(lua_State *L) {
     DB* db;
     int argc = 0;
     Options options = options_from_table(L, ++argc);
     // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
     options.IncreaseParallelism();
     options.OptimizeLevelStyleCompaction();
     // create the DB if it's not already present

     //fprintf(stderr," create = %s\n", options.create_if_missing ? "true": "false");
     //fprintf(stderr," create = %s\n", options.create_missing_column_families? "true": "false");
     // open DB

     const char *path = luaL_checkstring(L, ++argc);

     Status s = DB::Open(options, path, &db);
     //assert(s.ok());
     if(!s.ok()) {
        fprintf(stderr,"status = %s\n", s.getState());
         luaL_error(L, "failed to open");
         return 0;
     }
     rocks_db *d = new (lua_newuserdata(L, sizeof(rocks_db))) rocks_db();
     d->options=options;
     d->db = db;
     d->open = true;
     lrocks::setmeta(L, "db");

     return 1;
 }




#ifdef __cplusplus
extern "C"
#endif
DLL_PUBLIC int luaopen_nwcrocks(lua_State *L) {
    lua_newtable(L);

    /* register classes */

    lrocks::createmeta(L, "db", db_reg);
    lrocks::setfuncs(L, funcs, 0);

    lua_pushliteral(L, LROCKSDB_VERSION);
    lua_setfield(L, -2, "_VERSION");
    lua_pushliteral(L, LROCKSDB_COPYRIGHT);
    lua_setfield(L, -2, "_COPYRIGHT");
    lua_pushliteral(L, LROCKSDB_VERSION);
    lua_setfield(L, -2, "_DESCRIPTION");

    return 1;
}

