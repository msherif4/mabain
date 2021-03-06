#include "mabain/db.h"
#include "mabain/mb_data.h"
#include "mabain_c_interface.h"

extern "C" {

void* mbOpen(const char *db_dir, int writer_mode)
{
    if (!db_dir) return NULL;
    int options = mabain::CONSTS::ReaderOptions();
    if (writer_mode) options |= mabain::CONSTS::WriterOptions();
    mabain::DB *db = new mabain::DB(db_dir, options, 9223372036854775807LL, 9223372036854775807LL);
    if (!db->is_open()) {
        std::cerr << "failed to open DB " << db_dir << ": " << db->StatusStr() << "\n";
    }
    return (void*) db;
}

void mbClose(void *db)
{
    if (!db) return;
    mabain::DB *dbi = (mabain::DB *) db;
    dbi->Close();
}

int mbAdd(void *db, const char *key, int key_len, const char *data, int data_len)
{
    if (!db) return -1;
    mabain::DB *dbi = (mabain::DB *) db;
    return dbi->Add(key, key_len, data, data_len);
}

int mbRemove(void *db, const char *key, int key_len)
{
    if (!db) return -1;
    mabain::DB *dbi = (mabain::DB *) db;
    return dbi->Remove(key, key_len);
}

int mbFind(void *db, const char *key, int len, mb_query_result *result)
{
    if (!db) return -1;
    mabain::DB *dbi = (mabain::DB *) db;
    mabain::MBData mbd;
    int rval = dbi->Find(key, len, mbd);
    if(result) {
        if(rval == 0) {
            result->data = (char*) mbd.buff;
            result->len  = mbd.data_len;
            result->data[mbd.data_len] = '\0';
            mbd.buff = NULL;
        } else {
            result->data = (char*) malloc(1);
            result->data[0] = '\0';
        }
    }
    return rval;
}

}