/**
 * @author Alejandro Solozabal
 *
 * @file sqlite_db.cpp
 *
 */

/*******************************************************************
 * Defines
 *******************************************************************/
#include "sqlite_db.hpp"

/*******************************************************************
 * Global variables
 *******************************************************************/
sqlite3 *db = NULL;

/*******************************************************************
 * Function definition
 *******************************************************************/
int init_sqlite_db()
{
    int rc;

    /* Open connection with DB only once */
    if(!db)
    {
        rc = sqlite3_open("/etc/kinectalarm/detections.db", &db);

        if (rc != SQLITE_OK) {
            LOG(LOG_ERR,"Cannot open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return -1;
        }
    }

    return 0;
}

int deinit_sqlite_db()
{
    sqlite3_close(db);
    db = NULL;
    return 0;
}

int create_det_table_sqlite_db()
{
    char *err_msg = 0;

    /* Create SQL statement */
    char *sql = "CREATE TABLE IF NOT EXISTS DETECTIONS("  \
    "ID			INTEGER		PRIMARY KEY," \
    "DATE			DATETIME	NOT NULL," \
    "DURATION		INTEGER		NOT NULL," \
    "FILENAME_IMG	CHAR(50)	NOT NULL," \
    "FILENAME_VID	CHAR(50)	NOT NULL);";

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if(rc != SQLITE_OK) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    return 0;
}

int insert_entry_det_table_sqlite_db(unsigned int id, unsigned int datetime, unsigned int duration, char *filename_img, char *filename_vid)
{
    char *err_msg = 0;

    /* Create SQL statement */
    char sql[255];
    sprintf(sql,"INSERT INTO DETECTIONS (ID,DATE,DURATION,FILENAME_IMG,FILENAME_VID) "  \
        "VALUES (%u, %u, %u, '%s', '%s'); ",id,datetime,duration,filename_img,filename_vid);

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if( rc != SQLITE_OK ) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

static int number_entries_det_table_sqlite_db_cb(void *param, int argc, char **argv, char **azColName) {
    int *number_entries = (int*) param;

    if(argc>0)
        *number_entries = atoi(argv[0]);

    return 0;
}

int number_entries_det_table_sqlite_db(int *number_entries)
{
    char *err_msg = 0;
    /* Create SQL statement */
    char sql[255];
    sprintf(sql,"SELECT count(*) FROM detections;");

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, number_entries_det_table_sqlite_db_cb, number_entries, &err_msg);

    if( rc != SQLITE_OK ) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int get_entry_det_table_sqlite_db(uint32_t id, uint32_t *timestamp, uint32_t *duration, char *filename_img, char *filename_vid)
{
    char sql[255];
    sprintf(sql,"SELECT * FROM DETECTIONS WHERE id=%d;",id);

    sqlite3_stmt *pStmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);

    if (rc != SQLITE_OK ) {

        LOG(LOG_ERR,"Failed to prepare statement\n");
        LOG(LOG_ERR,"Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    rc = sqlite3_step(pStmt);

    int bytes = 0;

    if (rc == SQLITE_ROW) {

        if(timestamp)
            *timestamp = sqlite3_column_int(pStmt, 1);

        if(duration)
            *duration = sqlite3_column_int(pStmt, 2);

        if(filename_img) {
            const unsigned char *text = sqlite3_column_text(pStmt, 3);
            size_t len = strlen((char*)text);
            strncpy(filename_img,(const char*)text,len);
            filename_img[len] = '\0';
        }

        if(filename_vid) {
            const unsigned char *text = sqlite3_column_text(pStmt, 4);
            size_t len = strlen((char*)text);
            strncpy(filename_vid,(const char*)text,len);
            filename_vid[len] = '\0';
        }

    }
    else
        return -1;

    sqlite3_finalize(pStmt);

    return 0;
}

int delete_entry_det_table_sqlite_db(int id)
{
    char *err_msg = 0;
    /* Create SQL statement */
    char sql[255];
    sprintf(sql,"DELETE FROM detections WHERE id=%d;",id);

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if( rc != SQLITE_OK ) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int delete_all_entries_det_table_sqlite_db()
{
    char *err_msg = 0;
    /* Create SQL statement */
    char sql[255];
    sprintf(sql,"DELETE FROM detections;");

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if( rc != SQLITE_OK ) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int create_status_table_sqlite_db()
{
    char *err_msg = 0;

    /* Create SQL statement */
    char *sql = "CREATE TABLE IF NOT EXISTS STATUS("  \
        "ID					INTEGER		PRIMARY KEY," \
        "TILT				INTEGER		NOT NULL," \
        "BRIGHTNESS			INTEGER		NOT NULL," \
        "CONTRAST			INTEGER		NOT NULL," \
        "DET_ACTIVE			INTEGER		NOT NULL," \
        "LVW_ACTIVE			INTEGER		NOT NULL," \
        "DET_THRESHOLD		INTEGER		NOT NULL," \
        "DET_SENSITIVITY	INTEGER		NOT NULL," \
        "DET_NUM_SHOTS		INTEGER		NOT NULL," \
        "DET_FPS			INTEGER		NOT NULL," \
        "DET_CURR_DET_ID	INTEGER		NOT NULL," \
        "LVW_FPS			INTEGER		NOT NULL );";

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if(rc != SQLITE_OK) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }

    return 0;
}

int number_entries_status_table_sqlite_db(int *number_entries)
{
    char sql[255];
    sprintf(sql,"SELECT count(*) FROM STATUS;");

    sqlite3_stmt *pStmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);

    if (rc != SQLITE_OK ) {
        LOG(LOG_ERR,"Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    rc = sqlite3_step(pStmt);

    int bytes = 0;

    if (rc == SQLITE_ROW) {
        if(number_entries)
            *number_entries = sqlite3_column_int(pStmt, 0);
    }
    else
        return -1;

    sqlite3_finalize(pStmt);
    return 0;
}

int insert_entry_status_table_sqlite_db(struct status_table *status)
{
    char *err_msg = 0;

    /* Create SQL statement */
    char sql[512];
    sprintf(sql,"INSERT INTO STATUS (ID,TILT,BRIGHTNESS,CONTRAST,DET_ACTIVE,LVW_ACTIVE,DET_THRESHOLD," \
            "DET_SENSITIVITY,DET_NUM_SHOTS,DET_FPS,DET_CURR_DET_ID,LVW_FPS) "  \
        "VALUES (%u, %d, %d, %d, %u, %u, %u, %u, %u, %u, %u, %u);",status->id,status->tilt,status->contrast, \
                                                        status->brightness,status->det_active, \
                                                        status->lvw_active,status->det_threshold, \
                                                        status->det_sensitivity,status->det_num_shots, \
                                                        status->det_fps,status->det_curr_det_id, \
                                                        status->lvw_fps);

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if( rc != SQLITE_OK ) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int get_entry_status_table_sqlite_db(struct status_table *status)
{
    char sql[255];
    sprintf(sql,"SELECT * FROM STATUS WHERE id=%d;",status->id);

    sqlite3_stmt *pStmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &pStmt, 0);

    if (rc != SQLITE_OK ) {

        LOG(LOG_ERR,"Failed to prepare statement\n");
        LOG(LOG_ERR,"Cannot open database: %s\n", sqlite3_errmsg(db));

        sqlite3_close(db);

        return 1;
    }

    rc = sqlite3_step(pStmt);

    int bytes = 0;

    if (rc == SQLITE_ROW) {
        status->tilt            = sqlite3_column_int(pStmt, 1);
        status->brightness      = sqlite3_column_int(pStmt, 2);
        status->contrast        = sqlite3_column_int(pStmt, 3);
        status->det_active      = sqlite3_column_int(pStmt, 4);
        status->lvw_active      = sqlite3_column_int(pStmt, 5);
        status->det_threshold   = sqlite3_column_int(pStmt, 6);
        status->det_sensitivity = sqlite3_column_int(pStmt, 7);
        status->det_num_shots   = sqlite3_column_int(pStmt, 8);
        status->det_fps         = sqlite3_column_int(pStmt, 9);
        status->det_curr_det_id = sqlite3_column_int(pStmt, 10);
        status->lvw_fps         = sqlite3_column_int(pStmt, 11);
    }
    else
        return -1;

    sqlite3_finalize(pStmt);
    return 0;
}

int update_entry_status_table_sqlite_db(struct status_table *status)
{
    char *err_msg = 0;

    /* Create SQL statement */
    char sql[512];
    sprintf(sql,"UPDATE STATUS SET " \
            "TILT=%d,BRIGHTNESS=%d,CONTRAST=%d,DET_ACTIVE=%u,LVW_ACTIVE=%u,DET_THRESHOLD=%u," \
            "DET_SENSITIVITY=%u,DET_NUM_SHOTS=%u,DET_FPS=%u,DET_CURR_DET_ID=%u,LVW_FPS=%u "  \
            "WHERE ID=0;",	status->tilt,status->brightness, \
                            status->contrast,status->det_active, \
                            status->lvw_active,status->det_threshold, \
                            status->det_sensitivity,status->det_num_shots, \
                            status->det_fps,status->det_curr_det_id, \
                            status->lvw_fps);

    /* Execute SQL statement */
    int rc = sqlite3_exec(db, sql, NULL, 0, &err_msg);

    if( rc != SQLITE_OK ) {
        LOG(LOG_ERR,"SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}
