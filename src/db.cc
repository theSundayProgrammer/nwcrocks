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
namespace {
    int open_db(lua_State *L);
    int open_cf(lua_State *L);
    int close(lua_State *L);
    const struct luaL_Reg  db_reg[] = {
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

struct rocks_cf { 
    DB* db=nullptr;
    bool open=false;
    Options options;
    std::vector<std::string> cf_names;
    std::vector<ColumnFamilyHandle*> handles;
    ColumnFamilyHandle* get_handle(const std::string& str){
        ColumnFamilyHandle  *handle = nullptr;
        auto iter_handle = handles.begin();
        for( auto iter_name = cf_names.begin();
                iter_name != cf_names.end() && *iter_name != str;
                ++iter_name) 
                    ++iter_handle;
         if (iter_handle != handles.end())
             handle = *iter_handle;
         return handle;
        
    }


    ~rocks_cf () {
        for (auto handle : handles) {
            Status s = db->DestroyColumnFamilyHandle(handle);
            assert(s.ok());
        }
        delete db;
    }
};
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
int open_cf(lua_State* L){
    DB* db;
    int argc = 0;
    using ROCKSDB_NAMESPACE::ColumnFamilyDescriptor;
    Options options = options_from_table(L, ++argc);
    // Optimize RocksDB. This is the easiest way to get RocksDB to perform well
    options.IncreaseParallelism();
    options.OptimizeLevelStyleCompaction();
    // create the DB if it's not already present

    //fprintf(stderr," create = %s\n", options.create_if_missing ? "true": "false");
    //fprintf(stderr," create = %s\n", options.create_missing_column_families? "true": "false");
    // open DB

    const char *path = luaL_checkstring(L, ++argc);

    lua_pushvalue(L, ++argc);
    lua_pushnil(L);
    // open DB with two column families
    std::vector<ColumnFamilyDescriptor> column_families;
    // have to open default column family
    std::vector<std::string> cf_names;
    while (lua_next(L, -2))
    {
        lua_pushvalue(L, -2);
        int index = luaL_checkint(L, -1);
        const char *value = lua_tostring(L, -2);
        cf_names.push_back(value);
        column_families.push_back(
                ColumnFamilyDescriptor( value, ROCKSDB_NAMESPACE::ColumnFamilyOptions())
                );
        //fprintf(stderr,"%d=%s\n", index,value);
        lua_pop(L, 2);
    }
    lua_pop(L, 1);
    /* int options */
    std::vector<ColumnFamilyHandle*> handles;
    Status s = DB::Open(options, path, column_families, &handles, &db);
    if(!s.ok()) {
        fprintf(stderr,"status = %s\n", s.getState());
        luaL_error(L, "failed to open");
        return 0;
    }
    rocks_cf *d = new (lua_newuserdata(L, sizeof(rocks_cf))) rocks_cf();
    d->options=options;
    d->db = db;
    d->open = true;
    d->handles.swap(handles);
    d->cf_names.swap(cf_names);
    assert(cf_names.size() == handles.size());
    lrocks::setmeta(L, "db");

    return 1;
}

    int close(lua_State *L) {
        int index = 1;
        rocks_db *d = (rocks_db*) luaL_checkudata(L, index, "db");
        luaL_argcheck(L, d != NULL && d->db != NULL, index, "db expected");
        d->~rocks_db(); ;
        
        return 1;
    }



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

