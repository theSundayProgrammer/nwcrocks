#include "db.h"
#include "rocksdb/utilities/backup_engine.h"

using ROCKSDB_NAMESPACE::DB;
using ROCKSDB_NAMESPACE::Options;
using ROCKSDB_NAMESPACE::BackupEngine;
using ROCKSDB_NAMESPACE::ReadOptions;
using ROCKSDB_NAMESPACE::Status;

using ROCKSDB_NAMESPACE::BackupEngine;
using ROCKSDB_NAMESPACE::BackupEngineOptions;
using ROCKSDB_NAMESPACE::BackupEngineReadOnly;
using ROCKSDB_NAMESPACE::BackupInfo;
using ROCKSDB_NAMESPACE::Env;
namespace {
    int _create_new_backup(lua_State *L);
    int _purge_old_backups(lua_State *L);
    int _restore_db_from_latest_backup(lua_State *L);
    int _get_backup_info_count(lua_State *L);
    int _get_backup_info(lua_State *L);
    int _close(lua_State *L);
    const  char* table="backup_engine_table";

    static const struct luaL_Reg reg[] = {
        { "create_new_backup", _create_new_backup },
#if 0
        { "purge_old_backups", _purge_old_backups },
        { "restore_db_from_latest_backup", _restore_db_from_latest_backup },
#endif
        { "get_backup_info_count", _get_backup_info_count },
        { "get_backup_info", _get_backup_info },
        { "close", _close },
        { "__gc", _close },
        { NULL, NULL }
    };

    struct backup_engine_t {
        BackupEngine* backup_engine;
        DB* db;
    };

    backup_engine_t *_get_backup_engine(lua_State *L, int index) {
        backup_engine_t *o = (backup_engine_t*)
            luaL_checkudata(L, index, "backup_engine");
        luaL_argcheck(L, o != NULL && o->backup_engine!= NULL, index, "backup_engine expected");
        return o;
    }


    int _create_new_backup(lua_State *L) {
        backup_engine_t *be = _get_backup_engine(L, 1);
        Status s =   be->backup_engine->CreateNewBackup(be->db);
        if(!s.ok()) {
            luaL_error(L, "unable to create backup engine");
            return 0;
        }
        return 1;
    }
#if 0
    int _purge_old_backups(lua_State *L) {
        backup_engine_t *be = lrocksdb_get_backup_engine(L, 1);
        uint32_t num_backups_to_keep = luaL_checknumber(L, 2);
        char *err = NULL;
        rocksdbbackup_engine_purge_old_backups(be->backup_engine, num_backups_to_keep, &err);
        if(err) {
            luaL_error(L, err);
            free(err);
            return 0;
        }
        return 1;
    }

    int _restore_db_from_latest_backup(lua_State *L) {
        backup_engine_t *be = lrocksdb_get_backup_engine(L, 1);
        const char* db_dir = luaL_checkstring(L, 2);
        const char* wal_dir = luaL_checkstring(L, 3);
        lrocksdb_restoreoptions_t *ro = lrocksdb_get_restoreoptions(L, 4);
        char *err = NULL;
        rocksdbbackup_engine_restore_db_from_latest_backup(be->backup_engine, db_dir,
                wal_dir, ro->restoreoptions, &err);
        if(err != NULL) {
            luaL_error(L, err);
            free(err);
            return 0;
        }
        return 1;
    }

#endif
    int _get_backup_info_count(lua_State *L) {
        backup_engine_t *be = _get_backup_engine(L, 1);
        int index = luaL_checkint(L, 2) - 1; //keeping with Lua indices start at 1
        std::vector<BackupInfo> backup_info;
        be->backup_engine->GetBackupInfo(&backup_info);
        int count = backup_info.size();
        lua_pushnumber(L, count);
        return 1;
    }
    int _get_backup_info(lua_State *L) {
        backup_engine_t *be = _get_backup_engine(L, 1);
        int index = luaL_checkint(L, 2) - 1; //keeping with Lua indices start at 1
        std::vector<BackupInfo> backup_info;
        be->backup_engine->GetBackupInfo(&backup_info);
        int count = backup_info.size();
        if(index < 0 || index >= count) {
            luaL_error(L, "index out of range");
            return 0;
        }
        lua_newtable(L);
        lua_pushnumber(L,backup_info[index].timestamp);
        lua_setfield(L, -2, "timestamp");
        lua_pushnumber(L, backup_info[index].backup_id);
        lua_setfield(L, -2, "id");
        lua_pushnumber(L, backup_info[index].size);
        lua_setfield(L, -2, "size");
        lua_pushnumber(L, backup_info[index].number_files);
        lua_setfield(L, -2, "number_files");
        return 1;
    }

    int _close(lua_State *L) {
        backup_engine_t *be = _get_backup_engine(L, 1);
        if(be->backup_engine != NULL) {
            delete be->backup_engine;
            be->backup_engine = NULL;
        }
        return 1;
    }
}
static bool init_backup(lua_State* L) {
    static bool init = [=]() {
        lrocks::createmeta(L, table, reg);
        return true;
    }();
    return init;
}
int create_backup_engine( lua_State *L) {
    if (!init_bakup(L))
        return 0;
    BackupEngine* backup_engine;
    Status s = BackupEngine::Open(Env::Default(),
            BackupEngineOptions(path),
            &backup_engine);
    assert(s.ok());
    if(!s.ok()) {
        luaL_error(L, "unable to create backup engine");
        return 0;
    }
    backup_engine_t *b = new (lua_newuserdata(L, sizeof(backup_engine_t))) backup_engine_t ;
    b->backup_engine = backup_engine ;
    b->db = db;
    //push meta table onto stack
    luaL_getmetatable(L, table);
    //assign meta table to user data and return
    lua_setmetatable(L, -2);
    return 1;
}
    int backup_engine(
        lua_State *L, 
        ROCKSDB_NAMESPACE::DB* db,
        std::string const& path
        ) {
        if (!init_bakup(L))
            return 0;
        BackupEngine* backup_engine;
        Status s = BackupEngine::Open(Env::Default(),
                BackupEngineOptions(path),
                &backup_engine);
        assert(s.ok());
        if(!s.ok()) {
            luaL_error(L, "unable to create backup engine");
            return 0;
        }
        backup_engine->CreateNewBackup(db);
        s= assert(s.ok());
        backup_engine_t *b = new (lua_newuserdata(L, sizeof(backup_engine_t))) backup_engine_t ;
        b->backup_engine = backup_engine ;
        b->db = db;
        //push meta table onto stack
        luaL_getmetatable(L, table);
        //assign meta table to user data and return
        lua_setmetatable(L, -2);
        return 1;
    }
#if 0
int main() {
  assert(s.ok());

  // create backup
  BackupEngine* backup_engine;
  s = BackupEngine::Open(Env::Default(),
                         BackupEngineOptions("/tmp/rocksdb_example_backup"),
                         &backup_engine);
  assert(s.ok());

  backup_engine->CreateNewBackup(db);
  assert(s.ok());

  std::vector<BackupInfo> backup_info;
  backup_engine->GetBackupInfo(&backup_info);

  s = backup_engine->VerifyBackup(1);
  assert(s.ok());

  // Put key-value
  db->Put(WriteOptions(), "key2", "value2");
  assert(s.ok());

  db->Close();
  delete db;
  db = nullptr;

  // restore db to backup 1
  BackupEngineReadOnly* backup_engine_ro;
  s = BackupEngineReadOnly::Open(
      Env::Default(), BackupEngineOptions("/tmp/rocksdb_example_backup"),
      &backup_engine_ro);
  assert(s.ok());

  s = backup_engine_ro->RestoreDBFromBackup(1, "/tmp/rocksdb_example",
                                            "/tmp/rocksdb_example");
  assert(s.ok());

  // open db again
  s = DB::Open(options, kDBPath, &db);
  assert(s.ok());

  std::string value;
  s = db->Get(ReadOptions(), "key1", &value);
  assert(!s.IsNotFound());

  s = db->Get(ReadOptions(), "key2", &value);
  assert(s.IsNotFound());

  delete backup_engine;
  delete backup_engine_ro;
  delete db;

  return 0;
}
#endif
