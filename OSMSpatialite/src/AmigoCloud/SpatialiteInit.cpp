/*
 *
 * AmigoMobile
 *
 * Copyright (c) 2011-2015 AmigoCloud Inc., All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library.
 *
 */


#include "SpatialiteInit.h"

#include <sqlite3.h>
#include <spatialite.h>
#include <string.h>
#include "Logger.h"

namespace AmigoCloud
{
    namespace SpatialiteInit
    {
        
        static std::string TAG("SpatialiteInit");
        
        static int createGeometryColumns(void *_sqlite)
        {
            sqlite3 *sqlite = (sqlite3*)_sqlite;
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the GEOMETRY_COLUMNS table */
            strcpy(sql, "CREATE TABLE geometry_columns (\n");
            strcat(sql, "f_table_name TEXT NOT NULL,\n");
            strcat(sql, "f_geometry_column TEXT NOT NULL,\n");
            strcat(sql, "geometry_type INTEGER NOT NULL,\n");
            strcat(sql, "coord_dimension INTEGER NOT NULL,\n");
            strcat(sql, "srid INTEGER NOT NULL,\n");
            strcat(sql, "spatial_index_enabled INTEGER NOT NULL,\n");
            strcat(sql, "CONSTRAINT pk_geom_cols PRIMARY KEY ");
            strcat(sql, "(f_table_name, f_geometry_column),\n");
            strcat(sql, "CONSTRAINT fk_gc_srs FOREIGN KEY ");
            strcat(sql, "(srid) REFERENCES spatial_ref_sys (srid),\n");
            strcat(sql, "CONSTRAINT ck_gc_rtree CHECK ");
            strcat(sql, "(spatial_index_enabled IN (0,1,2)))");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "table successfully created");
            /* creating an INDEX corresponding to the SRID FK */
            strcpy(sql, "CREATE INDEX idx_srid_geocols ON geometry_columns\n");
            strcat(sql, "(srid) ");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            /* creating the GEOMETRY_COLUMNS triggers */
            strcpy(sql, "CREATE TRIGGER geometry_columns_f_table_name_insert\n");
            strcat(sql, "BEFORE INSERT ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'insert on geometry_columns violates constraint: ");
            strcat(sql, "f_table_name value must not contain a single quote')\n");
            strcat(sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'insert on geometry_columns violates constraint: ");
            strcat(sql, "f_table_name value must not contain a double quote')\n");
            strcat(sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'insert on geometry_columns violates constraint: \n");
            strcat(sql, "f_table_name value must be lower case')\n");
            strcat(sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_f_table_name_insert' successfully created");
            strcpy(sql, "CREATE TRIGGER geometry_columns_f_table_name_update\n");
            strcat(sql, "BEFORE UPDATE OF 'f_table_name' ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'update on geometry_columns violates constraint: ");
            strcat(sql, "f_table_name value must not contain a single quote')\n");
            strcat(sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'update on geometry_columns violates constraint: ");
            strcat(sql, "f_table_name value must not contain a double quote')\n");
            strcat(sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'update on geometry_columns violates constraint: ");
            strcat(sql, "f_table_name value must be lower case')\n");
            strcat(sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_f_table_name_update' successfully created");
            strcpy(sql, "CREATE TRIGGER geometry_columns_f_geometry_column_insert\n");
            strcat(sql, "BEFORE INSERT ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'insert on geometry_columns violates constraint: ");
            strcat(sql, "f_geometry_column value must not contain a single quote')\n");
            strcat(sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'insert on geometry_columns violates constraint: \n");
            strcat(sql, "f_geometry_column value must not contain a double quote')\n");
            strcat(sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'insert on geometry_columns violates constraint: ");
            strcat(sql, "f_geometry_column value must be lower case')\n");
            strcat(sql,
                   "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_f_geometry_column_insert' successfully created");
            strcpy(sql, "CREATE TRIGGER geometry_columns_f_geometry_column_update\n");
            strcat(sql,
                   "BEFORE UPDATE OF 'f_geometry_column' ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'update on geometry_columns violates constraint: ");
            strcat(sql, "f_geometry_column value must not contain a single quote')\n");
            strcat(sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'update on geometry_columns violates constraint: ");
            strcat(sql, "f_geometry_column value must not contain a double quote')\n");
            strcat(sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'update on geometry_columns violates constraint: ");
            strcat(sql, "f_geometry_column value must be lower case')\n");
            strcat(sql,
                   "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_f_geometry_column_update' successfully created");
            strcpy(sql, "CREATE TRIGGER geometry_columns_geometry_type_insert\n");
            strcat(sql, "BEFORE INSERT ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql, "SELECT RAISE(ABORT,'geometry_type must be one of ");
            strcat(sql, "0,1,2,3,4,5,6,7,");
            strcat(sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat(sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat(sql, "3000,3001,3002,3003,3004,3005,3006,3007')\n");
            strcat(sql, "WHERE NOT(NEW.geometry_type IN (0,1,2,3,4,5,6,7,");
            strcat(sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat(sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat(sql, "3000,3001,3002,3003,3004,3005,3006,3007));\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_geometry_type_insert' successfully created");
            
            strcpy(sql, "CREATE TRIGGER geometry_columns_geometry_type_update\n");
            strcat(sql, "BEFORE UPDATE OF 'geometry_type' ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql, "SELECT RAISE(ABORT,'geometry_type must be one of ");
            strcat(sql, "0,1,2,3,4,5,6,7,");
            strcat(sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat(sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat(sql, "3000,3001,3002,3003,3004,3005,3006,3007')\n");
            strcat(sql, "WHERE NOT(NEW.geometry_type IN (0,1,2,3,4,5,6,7,");
            strcat(sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat(sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat(sql, "3000,3001,3002,3003,3004,3005,3006,3007));\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_geometry_type_update' successfully created");
            strcpy(sql, "CREATE TRIGGER geometry_columns_coord_dimension_insert\n");
            strcat(sql, "BEFORE INSERT ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'coord_dimension must be one of 2,3,4')\n");
            strcat(sql, "WHERE NOT(NEW.coord_dimension IN (2,3,4));\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_coord_dimension_insert' successfully created");
            strcpy(sql, "CREATE TRIGGER geometry_columns_coord_dimension_update\n");
            strcat(sql, "BEFORE UPDATE OF 'coord_dimension' ON 'geometry_columns'\n");
            strcat(sql, "FOR EACH ROW BEGIN\n");
            strcat(sql,
                   "SELECT RAISE(ABORT,'coord_dimension must be one of 2,3,4')\n");
            strcat(sql, "WHERE NOT(NEW.coord_dimension IN (2,3,4));\n");
            strcat(sql, "END");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return 0;
            }
            //		updateSpatiaLiteHistory(sqlite, "geometry_columns", NULL,
            //		                        "trigger 'geometry_columns_coord_dimension_update' successfully created");
            return 1;
        }
        
        static int
        create_views_geometry_columns (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIEWS_GEOMETRY_COLUMNS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "views_geometry_columns (\n");
            strcat (sql, "view_name TEXT NOT NULL,\n");
            strcat (sql, "view_geometry TEXT NOT NULL,\n");
            strcat (sql, "view_rowid TEXT NOT NULL,\n");
            strcat (sql, "f_table_name TEXT NOT NULL,\n");
            strcat (sql, "f_geometry_column TEXT NOT NULL,\n");
            strcat (sql, "read_only INTEGER NOT NULL,\n");
            strcat (sql, "CONSTRAINT pk_geom_cols_views PRIMARY KEY ");
            strcat (sql, "(view_name, view_geometry),\n");
            strcat (sql, "CONSTRAINT fk_views_geom_cols FOREIGN KEY ");
            strcat (sql, "(f_table_name, f_geometry_column) ");
            strcat (sql, "REFERENCES geometry_columns ");
            strcat (sql, "(f_table_name, f_geometry_column) ");
            strcat (sql, "ON DELETE CASCADE,\n");
            strcat (sql, "CONSTRAINT ck_vw_rdonly CHECK (read_only IN ");
            strcat (sql, "(0,1)))");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating an INDEX supporting the GEOMETRY_COLUMNS FK */
            strcpy (sql, "CREATE INDEX IF NOT EXISTS ");
            strcat (sql, "idx_viewsjoin ON views_geometry_columns\n");
            strcat (sql, "(f_table_name, f_geometry_column)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIEWS_GEOMETRY_COLUMNS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_view_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: \n");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_view_name_update\n");
            strcat (sql, "BEFORE UPDATE OF 'view_name' ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_view_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_view_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_geometry' ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_view_rowid_update\n");
            strcat (sql, "BEFORE UPDATE OF 'view_rowid' ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_rowid value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_rowid value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_rowid LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "view_rowid value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_rowid <> lower(NEW.view_rowid);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_view_rowid_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "view_rowid value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_rowid LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: \n");
            strcat (sql, "view_rowid value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_rowid LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "view_rowid value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_rowid <> lower(NEW.view_rowid);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_f_table_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: \n");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgc_f_table_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_table_name' ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS vwgc_f_geometry_column_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: \n");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS vwgc_f_geometry_column_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_geometry_column' ON 'views_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_virts_geometry_columns (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIRTS_GEOMETRY_COLUMNS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "virts_geometry_columns (\n");
            strcat (sql, "virt_name TEXT NOT NULL,\n");
            strcat (sql, "virt_geometry TEXT NOT NULL,\n");
            strcat (sql, "geometry_type INTEGER NOT NULL,\n");
            strcat (sql, "coord_dimension INTEGER NOT NULL,\n");
            strcat (sql, "srid INTEGER NOT NULL,\n");
            strcat (sql, "CONSTRAINT pk_geom_cols_virts PRIMARY KEY ");
            strcat (sql, "(virt_name, virt_geometry),\n");
            strcat (sql, "CONSTRAINT fk_vgc_srid FOREIGN KEY ");
            strcat (sql, "(srid) REFERENCES spatial_ref_sys (srid))");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating an INDEX supporting the SPATIAL_REF_SYS FK */
            strcpy (sql, "CREATE INDEX IF NOT EXISTS ");
            strcat (sql, "idx_virtssrid ON virts_geometry_columns\n");
            strcat (sql, "(srid)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIRTS_GEOMETRY_COLUMNS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_virt_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns violates constraint: \n");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_virt_name_update\n");
            strcat (sql, "BEFORE UPDATE OF 'virt_name' ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_virt_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_virt_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_geometry' ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_geometry_type_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql, "SELECT RAISE(ABORT,'geometry_type must be one of ");
            strcat (sql, "0,1,2,3,4,5,6,7,");
            strcat (sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat (sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat (sql, "3000,3001,3002,3003,3004,3005,3006,3007')\n");
            strcat (sql, "WHERE NOT(NEW.geometry_type IN (0,1,2,3,4,5,6,7,");
            strcat (sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat (sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat (sql, "3000,3001,3002,3003,3004,3005,3006,3007));\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_geometry_type_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'geometry_type' ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql, "SELECT RAISE(ABORT,'geometry_type must be one of ");
            strcat (sql, "0,1,2,3,4,5,6,7,");
            strcat (sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat (sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat (sql, "3000,3001,3002,3003,3004,3005,3006,3007')\n");
            strcat (sql, "WHERE NOT(NEW.geometry_type IN (0,1,2,3,4,5,6,7,");
            strcat (sql, "1000,1001,1002,1003,1004,1005,1006,1007,");
            strcat (sql, "2000,2001,2002,2003,2004,2005,2006,2007,");
            strcat (sql, "3000,3001,3002,3003,3004,3005,3006,3007));\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_coord_dimension_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'coord_dimension must be one of 2,3,4')\n");
            strcat (sql, "WHERE NOT(NEW.coord_dimension IN (2,3,4));\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgc_coord_dimension_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'coord_dimension' ON 'virts_geometry_columns'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'coord_dimension must be one of 2,3,4')\n");
            strcat (sql, "WHERE NOT(NEW.coord_dimension IN (2,3,4));\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_geometry_columns_statistics (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the GEOMETRY_COLUMNS_STATISTICS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "geometry_columns_statistics (\n");
            strcat (sql, "f_table_name TEXT NOT NULL,\n");
            strcat (sql, "f_geometry_column TEXT NOT NULL,\n");
            strcat (sql, "last_verified TIMESTAMP,\n");
            strcat (sql, "row_count INTEGER,\n");
            strcat (sql, "extent_min_x DOUBLE,\n");
            strcat (sql, "extent_min_y DOUBLE,\n");
            strcat (sql, "extent_max_x DOUBLE,\n");
            strcat (sql, "extent_max_y DOUBLE,\n");
            strcat (sql, "CONSTRAINT pk_gc_statistics PRIMARY KEY ");
            strcat (sql, "(f_table_name, f_geometry_column),\n");
            strcat (sql, "CONSTRAINT fk_gc_statistics FOREIGN KEY ");
            strcat (sql, "(f_table_name, f_geometry_column) REFERENCES ");
            strcat (sql, "geometry_columns (f_table_name, f_geometry_column) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the GEOMETRY_COLUMNS_STATISTICS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcs_f_table_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_statistics violates constraint: \n");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcs_f_table_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_table_name' ON 'geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcs_f_geometry_column_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_statistics violates constraint: \n");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcs_f_geometry_column_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_geometry_column' ON 'geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_statistics violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_views_geometry_columns_statistics (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIEWS_GEOMETRY_COLUMNS_STATISTICS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "views_geometry_columns_statistics (\n");
            strcat (sql, "view_name TEXT NOT NULL,\n");
            strcat (sql, "view_geometry TEXT NOT NULL,\n");
            strcat (sql, "last_verified TIMESTAMP,\n");
            strcat (sql, "row_count INTEGER,\n");
            strcat (sql, "extent_min_x DOUBLE,\n");
            strcat (sql, "extent_min_y DOUBLE,\n");
            strcat (sql, "extent_max_x DOUBLE,\n");
            strcat (sql, "extent_max_y DOUBLE,\n");
            strcat (sql, "CONSTRAINT pk_vwgc_statistics PRIMARY KEY ");
            strcat (sql, "(view_name, view_geometry),\n");
            strcat (sql, "CONSTRAINT fk_vwgc_statistics FOREIGN KEY ");
            strcat (sql, "(view_name, view_geometry) REFERENCES ");
            strcat (sql, "views_geometry_columns (view_name, view_geometry) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIEWS_GEOMETRY_COLUMNS_STATISTICS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcs_view_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_statistics violates constraint: \n");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcs_view_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_name' ON 'views_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcs_view_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_statistics violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcs_view_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_geometry' ON 'views_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_statistics violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_statistics violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_virts_geometry_columns_statistics (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIRTS_GEOMETRY_COLUMNS_STATISTICS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "virts_geometry_columns_statistics (\n");
            strcat (sql, "virt_name TEXT NOT NULL,\n");
            strcat (sql, "virt_geometry TEXT NOT NULL,\n");
            strcat (sql, "last_verified TIMESTAMP,\n");
            strcat (sql, "row_count INTEGER,\n");
            strcat (sql, "extent_min_x DOUBLE,\n");
            strcat (sql, "extent_min_y DOUBLE,\n");
            strcat (sql, "extent_max_x DOUBLE,\n");
            strcat (sql, "extent_max_y DOUBLE,\n");
            strcat (sql, "CONSTRAINT pk_vrtgc_statistics PRIMARY KEY ");
            strcat (sql, "(virt_name, virt_geometry),\n");
            strcat (sql, "CONSTRAINT fk_vrtgc_statistics FOREIGN KEY ");
            strcat (sql, "(virt_name, virt_geometry) REFERENCES ");
            strcat (sql, "virts_geometry_columns (virt_name, virt_geometry) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIRTS_GEOMETRY_COLUMNS_STATISTICS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcs_virt_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_statistics violates constraint: \n");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcs_virt_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_name' ON 'virts_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcs_virt_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_statistics violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcs_virt_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_geometry' ON 'virts_geometry_columns_statistics'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_statistics violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_statistics violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_geometry_columns_field_infos (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the GEOMETRY_COLUMNS_FIELD_INFOS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "geometry_columns_field_infos (\n");
            strcat (sql, "f_table_name TEXT NOT NULL,\n");
            strcat (sql, "f_geometry_column TEXT NOT NULL,\n");
            strcat (sql, "ordinal INTEGER NOT NULL,\n");
            strcat (sql, "column_name TEXT NOT NULL,\n");
            strcat (sql, "null_values INTEGER NOT NULL,\n");
            strcat (sql, "integer_values INTEGER NOT NULL,\n");
            strcat (sql, "double_values INTEGER NOT NULL,\n");
            strcat (sql, "text_values INTEGER NOT NULL,\n");
            strcat (sql, "blob_values INTEGER NOT NULL,\n");
            strcat (sql, "max_size INTEGER,\n");
            strcat (sql, "integer_min INTEGER,\n");
            strcat (sql, "integer_max INTEGER,\n");
            strcat (sql, "double_min DOUBLE,\n");
            strcat (sql, "double_max DOUBLE,\n");
            strcat (sql, "CONSTRAINT pk_gcfld_infos PRIMARY KEY ");
            strcat (sql, "(f_table_name, f_geometry_column, ordinal, column_name),\n");
            strcat (sql, "CONSTRAINT fk_gcfld_infos FOREIGN KEY ");
            strcat (sql, "(f_table_name, f_geometry_column) REFERENCES ");
            strcat (sql, "geometry_columns (f_table_name, f_geometry_column) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the GEOMETRY_COLUMNS_FIELD_INFOS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcfi_f_table_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcfi_f_table_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_table_name' ON 'geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS gcfi_f_geometry_column_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS gcfi_f_geometry_column_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_geometry_column' ON 'geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_field_infos violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_views_geometry_columns_field_infos (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIEWS_COLUMNS_FIELD_INFOS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "views_geometry_columns_field_infos (\n");
            strcat (sql, "view_name TEXT NOT NULL,\n");
            strcat (sql, "view_geometry TEXT NOT NULL,\n");
            strcat (sql, "ordinal INTEGER NOT NULL,\n");
            strcat (sql, "column_name TEXT NOT NULL,\n");
            strcat (sql, "null_values INTEGER NOT NULL,\n");
            strcat (sql, "integer_values INTEGER NOT NULL,\n");
            strcat (sql, "double_values INTEGER NOT NULL,\n");
            strcat (sql, "text_values INTEGER NOT NULL,\n");
            strcat (sql, "blob_values INTEGER NOT NULL,\n");
            strcat (sql, "max_size INTEGER,\n");
            strcat (sql, "integer_min INTEGER,\n");
            strcat (sql, "integer_max INTEGER,\n");
            strcat (sql, "double_min DOUBLE,\n");
            strcat (sql, "double_max DOUBLE,\n");
            strcat (sql, "CONSTRAINT pk_vwgcfld_infos PRIMARY KEY ");
            strcat (sql, "(view_name, view_geometry, ordinal, column_name),\n");
            strcat (sql, "CONSTRAINT fk_vwgcfld_infos FOREIGN KEY ");
            strcat (sql, "(view_name, view_geometry) REFERENCES ");
            strcat (sql, "views_geometry_columns (view_name, view_geometry) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIEWS_COLUMNS_FIELD_INFOS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcfi_view_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcfi_view_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_name' ON 'views_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcfi_view_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcfi_view_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_geometry' ON 'views_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_virts_geometry_columns_field_infos (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIRTS_GEOMETRY_COLUMNS_FIELD_INFOS table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "virts_geometry_columns_field_infos (\n");
            strcat (sql, "virt_name TEXT NOT NULL,\n");
            strcat (sql, "virt_geometry TEXT NOT NULL,\n");
            strcat (sql, "ordinal INTEGER NOT NULL,\n");
            strcat (sql, "column_name TEXT NOT NULL,\n");
            strcat (sql, "null_values INTEGER NOT NULL,\n");
            strcat (sql, "integer_values INTEGER NOT NULL,\n");
            strcat (sql, "double_values INTEGER NOT NULL,\n");
            strcat (sql, "text_values INTEGER NOT NULL,\n");
            strcat (sql, "blob_values INTEGER NOT NULL,\n");
            strcat (sql, "max_size INTEGER,\n");
            strcat (sql, "integer_min INTEGER,\n");
            strcat (sql, "integer_max INTEGER,\n");
            strcat (sql, "double_min DOUBLE,\n");
            strcat (sql, "double_max DOUBLE,\n");
            strcat (sql, "CONSTRAINT pk_vrtgcfld_infos PRIMARY KEY ");
            strcat (sql, "(virt_name, virt_geometry, ordinal, column_name),\n");
            strcat (sql, "CONSTRAINT fk_vrtgcfld_infos FOREIGN KEY ");
            strcat (sql, "(virt_name, virt_geometry) REFERENCES ");
            strcat (sql, "virts_geometry_columns (virt_name, virt_geometry) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIRTS_GEOMETRY_COLUMNS_FIELD_INFOS triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcfi_virt_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcfi_virt_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_name' ON 'virts_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcfi_virt_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcfi_virt_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_geometry' ON 'virts_geometry_columns_field_infos'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_field_infos violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_field_infos violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_geometry_columns_times (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the GEOMETRY_COLUMNS_TIME table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "geometry_columns_time (\n");
            strcat (sql, "f_table_name TEXT NOT NULL,\n");
            strcat (sql, "f_geometry_column TEXT NOT NULL,\n");
            strcat (sql,
                    "last_insert TIMESTAMP NOT NULL DEFAULT '0000-01-01T00:00:00.000Z',\n");
            strcat (sql,
                    "last_update TIMESTAMP NOT NULL DEFAULT '0000-01-01T00:00:00.000Z',\n");
            strcat (sql,
                    "last_delete TIMESTAMP NOT NULL DEFAULT '0000-01-01T00:00:00.000Z',\n");
            strcat (sql, "CONSTRAINT pk_gc_time PRIMARY KEY ");
            strcat (sql, "(f_table_name, f_geometry_column),\n");
            strcat (sql, "CONSTRAINT fk_gc_time FOREIGN KEY ");
            strcat (sql, "(f_table_name, f_geometry_column) ");
            strcat (sql, "REFERENCES geometry_columns ");
            strcat (sql, "(f_table_name, f_geometry_column) ");
            strcat (sql, "ON DELETE CASCADE)");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the GEOMETRY_COLUMNS_TIME triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gctm_f_table_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_time'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_time violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_time violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_time violates constraint: \n");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gctm_f_table_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_table_name' ON 'geometry_columns_time'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_time violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_time violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_time violates constraint: ");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS gctm_f_geometry_column_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_time'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_time violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_time violates constraint: \n");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_time violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS gctm_f_geometry_column_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_geometry_column' ON 'geometry_columns_time'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_time violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_time violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_time violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_geometry_columns_auth (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the GEOMETRY_COLUMNS_AUTH table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "geometry_columns_auth (\n");
            strcat (sql, "f_table_name TEXT NOT NULL,\n");
            strcat (sql, "f_geometry_column TEXT NOT NULL,\n");
            strcat (sql, "read_only INTEGER NOT NULL,\n");
            strcat (sql, "hidden INTEGER NOT NULL,\n");
            strcat (sql, "CONSTRAINT pk_gc_auth PRIMARY KEY ");
            strcat (sql, "(f_table_name, f_geometry_column),\n");
            strcat (sql, "CONSTRAINT fk_gc_auth FOREIGN KEY ");
            strcat (sql, "(f_table_name, f_geometry_column) ");
            strcat (sql, "REFERENCES geometry_columns ");
            strcat (sql, "(f_table_name, f_geometry_column) ");
            strcat (sql, "ON DELETE CASCADE,\n");
            strcat (sql, "CONSTRAINT ck_gc_ronly CHECK (read_only IN ");
            strcat (sql, "(0,1)),\n");
            strcat (sql, "CONSTRAINT ck_gc_hidden CHECK (hidden IN ");
            strcat (sql, "(0,1)))");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the GEOMETRY_COLUMNS_AUTH triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcau_f_table_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_auth violates constraint: \n");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS gcau_f_table_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_table_name' ON 'geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_table_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_table_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_table_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_table_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.f_table_name <> lower(NEW.f_table_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS gcau_f_geometry_column_insert\n");
            strcat (sql, "BEFORE INSERT ON 'geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_auth violates constraint: \n");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql,
                    "CREATE TRIGGER IF NOT EXISTS gcau_f_geometry_column_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'f_geometry_column' ON 'geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_geometry_column value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.f_geometry_column LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on geometry_columns_auth violates constraint: ");
            strcat (sql, "f_geometry_column value must be lower case')\n");
            strcat (sql,
                    "WHERE NEW.f_geometry_column <> lower(NEW.f_geometry_column);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_views_geometry_columns_auth (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIEWS_GEOMETRY_COLUMNS_AUTH table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "views_geometry_columns_auth (\n");
            strcat (sql, "view_name TEXT NOT NULL,\n");
            strcat (sql, "view_geometry TEXT NOT NULL,\n");
            strcat (sql, "hidden INTEGER NOT NULL,\n");
            strcat (sql, "CONSTRAINT pk_vwgc_auth PRIMARY KEY ");
            strcat (sql, "(view_name, view_geometry),\n");
            strcat (sql, "CONSTRAINT fk_vwgc_auth FOREIGN KEY ");
            strcat (sql, "(view_name, view_geometry) ");
            strcat (sql, "REFERENCES views_geometry_columns ");
            strcat (sql, "(view_name, view_geometry) ");
            strcat (sql, "ON DELETE CASCADE,\n");
            strcat (sql, "CONSTRAINT ck_vwgc_hidden CHECK (hidden IN ");
            strcat (sql, "(0,1)))");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIEWS_GEOMETRY_COLUMNS_AUTH triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcau_view_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_auth violates constraint: \n");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcau_view_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_name' ON 'views_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_name <> lower(NEW.view_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcau_view_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'views_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_auth violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vwgcau_view_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'view_geometry'  ON 'views_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_auth violates constraint: \n");
            strcat (sql, "view_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.view_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on views_geometry_columns_auth violates constraint: ");
            strcat (sql, "view_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.view_geometry <> lower(NEW.view_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_virts_geometry_columns_auth (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VIRTS_GEOMETRY_COLUMNS_AUTH table */
            strcpy (sql, "CREATE TABLE IF NOT EXISTS ");
            strcat (sql, "virts_geometry_columns_auth (\n");
            strcat (sql, "virt_name TEXT NOT NULL,\n");
            strcat (sql, "virt_geometry TEXT NOT NULL,\n");
            strcat (sql, "hidden INTEGER NOT NULL,\n");
            strcat (sql, "CONSTRAINT pk_vrtgc_auth PRIMARY KEY ");
            strcat (sql, "(virt_name, virt_geometry),\n");
            strcat (sql, "CONSTRAINT fk_vrtgc_auth FOREIGN KEY ");
            strcat (sql, "(virt_name, virt_geometry) ");
            strcat (sql, "REFERENCES virts_geometry_columns ");
            strcat (sql, "(virt_name, virt_geometry) ");
            strcat (sql, "ON DELETE CASCADE,\n");
            strcat (sql, "CONSTRAINT ck_vrtgc_hidden CHECK (hidden IN ");
            strcat (sql, "(0,1)))");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VIRTS_GEOMETRY_COLUMNS_AUTH triggers */
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcau_virt_name_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_auth violates constraint: \n");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcau_virt_name_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_name' ON 'virts_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_name value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_name value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_name LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_name value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_name <> lower(NEW.virt_name);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcau_virt_geometry_insert\n");
            strcat (sql, "BEFORE INSERT ON 'virts_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_auth violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'insert on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            strcpy (sql, "CREATE TRIGGER IF NOT EXISTS vtgcau_virt_geometry_update\n");
            strcat (sql,
                    "BEFORE UPDATE OF 'virt_geometry' ON 'virts_geometry_columns_auth'\n");
            strcat (sql, "FOR EACH ROW BEGIN\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_geometry value must not contain a single quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%''%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_auth violates constraint: \n");
            strcat (sql, "virt_geometry value must not contain a double quote')\n");
            strcat (sql, "WHERE NEW.virt_geometry LIKE ('%\"%');\n");
            strcat (sql,
                    "SELECT RAISE(ABORT,'update on virts_geometry_columns_auth violates constraint: ");
            strcat (sql, "virt_geometry value must be lower case')\n");
            strcat (sql, "WHERE NEW.virt_geometry <> lower(NEW.virt_geometry);\n");
            strcat (sql, "END");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_geometry_columns_views (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the VECTOR_LAYERS view */
            strcpy (sql, "CREATE VIEW  IF NOT EXISTS ");
            strcat (sql, "vector_layers AS\n");
            strcat (sql, "SELECT 'SpatialTable' AS layer_type, ");
            strcat (sql, "f_table_name AS table_name, ");
            strcat (sql, "f_geometry_column AS geometry_column, ");
            strcat (sql, "geometry_type AS geometry_type, ");
            strcat (sql, "coord_dimension AS coord_dimension, ");
            strcat (sql, "srid AS srid, ");
            strcat (sql, "spatial_index_enabled AS spatial_index_enabled\n");
            strcat (sql, "FROM geometry_columns\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'SpatialView' AS layer_type, ");
            strcat (sql, "a.view_name AS table_name, ");
            strcat (sql, "a.view_geometry AS geometry_column, ");
            strcat (sql, "b.geometry_type AS geometry_type, ");
            strcat (sql, "b.coord_dimension AS coord_dimension, ");
            strcat (sql, "b.srid AS srid, ");
            strcat (sql, "b.spatial_index_enabled AS spatial_index_enabled\n");
            strcat (sql, "FROM views_geometry_columns AS a\n");
            strcat (sql, "LEFT JOIN geometry_columns AS b ON (");
            strcat (sql, "Upper(a.f_table_name) = Upper(b.f_table_name) AND ");
            strcat (sql, "Upper(a.f_geometry_column) = Upper(b.f_geometry_column))\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'VirtualShape' AS layer_type, ");
            strcat (sql, "virt_name AS table_name, ");
            strcat (sql, "virt_geometry AS geometry_column, ");
            strcat (sql, "geometry_type AS geometry_type, ");
            strcat (sql, "coord_dimension AS coord_dimension, ");
            strcat (sql, "srid AS srid, ");
            strcat (sql, "0 AS spatial_index_enabled\n");
            strcat (sql, "FROM virts_geometry_columns");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VECTOR_LAYERS_AUTH view */
            strcpy (sql, "CREATE VIEW  IF NOT EXISTS ");
            strcat (sql, "vector_layers_auth AS\n");
            strcat (sql, "SELECT 'SpatialTable' AS layer_type, ");
            strcat (sql, "f_table_name AS table_name, ");
            strcat (sql, "f_geometry_column AS geometry_column, ");
            strcat (sql, "read_only AS read_only, ");
            strcat (sql, "hidden AS hidden\n");
            strcat (sql, "FROM geometry_columns_auth\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'SpatialView' AS layer_type, ");
            strcat (sql, "a.view_name AS table_name, ");
            strcat (sql, "a.view_geometry AS geometry_column, ");
            strcat (sql, "b.read_only AS read_only, ");
            strcat (sql, "a.hidden AS hidden\n");
            strcat (sql, "FROM views_geometry_columns_auth AS a\n");
            strcat (sql, "JOIN views_geometry_columns AS b ON (");
            strcat (sql, "Upper(a.view_name) = Upper(b.view_name) AND ");
            strcat (sql, "Upper(a.view_geometry) = Upper(b.view_geometry))\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'VirtualShape' AS layer_type, ");
            strcat (sql, "virt_name AS table_name, ");
            strcat (sql, "virt_geometry AS geometry_column, ");
            strcat (sql, "1 AS read_only, ");
            strcat (sql, "hidden AS hidden\n");
            strcat (sql, "FROM virts_geometry_columns_auth");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VECTOR_LAYERS_STATISTICS view */
            strcpy (sql, "CREATE VIEW IF NOT EXISTS ");
            strcat (sql, "vector_layers_statistics AS\n");
            strcat (sql, "SELECT 'SpatialTable' AS layer_type, ");
            strcat (sql, "f_table_name AS table_name, ");
            strcat (sql, "f_geometry_column AS geometry_column, ");
            strcat (sql, "last_verified AS last_verified, ");
            strcat (sql, "row_count AS row_count, ");
            strcat (sql, "extent_min_x AS extent_min_x, ");
            strcat (sql, "extent_min_y AS extent_min_y, ");
            strcat (sql, "extent_max_x AS extent_max_x, ");
            strcat (sql, "extent_max_y AS extent_max_y\n");
            strcat (sql, "FROM geometry_columns_statistics\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'SpatialView' AS layer_type, ");
            strcat (sql, "view_name AS table_name, ");
            strcat (sql, "view_geometry AS geometry_column, ");
            strcat (sql, "last_verified AS last_verified, ");
            strcat (sql, "row_count AS row_count, ");
            strcat (sql, "extent_min_x AS extent_min_x, ");
            strcat (sql, "extent_min_y AS extent_min_y, ");
            strcat (sql, "extent_max_x AS extent_max_x, ");
            strcat (sql, "extent_max_y AS extent_max_y\n");
            strcat (sql, "FROM views_geometry_columns_statistics\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'VirtualShape' AS layer_type, ");
            strcat (sql, "virt_name AS table_name, ");
            strcat (sql, "virt_geometry AS geometry_column, ");
            strcat (sql, "last_verified AS last_verified, ");
            strcat (sql, "row_count AS row_count, ");
            strcat (sql, "extent_min_x AS extent_min_x, ");
            strcat (sql, "extent_min_y AS extent_min_y, ");
            strcat (sql, "extent_max_x AS extent_max_x, ");
            strcat (sql, "extent_max_y AS extent_max_y\n");
            strcat (sql, "FROM virts_geometry_columns_statistics");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            /* creating the VECTOR_LAYERS_FIELD_INFOS view */
            strcpy (sql, "CREATE VIEW IF NOT EXISTS ");
            strcat (sql, "vector_layers_field_infos AS\n");
            strcat (sql, "SELECT 'SpatialTable' AS layer_type, ");
            strcat (sql, "f_table_name AS table_name, ");
            strcat (sql, "f_geometry_column AS geometry_column, ");
            strcat (sql, "ordinal AS ordinal, ");
            strcat (sql, "column_name AS column_name, ");
            strcat (sql, "null_values AS null_values, ");
            strcat (sql, "integer_values AS integer_values, ");
            strcat (sql, "double_values AS double_values, ");
            strcat (sql, "text_values AS text_values, ");
            strcat (sql, "blob_values AS blob_values, ");
            strcat (sql, "max_size AS max_size, ");
            strcat (sql, "integer_min AS integer_min, ");
            strcat (sql, "integer_max AS integer_max, ");
            strcat (sql, "double_min AS double_min, ");
            strcat (sql, "double_max double_max\n");
            strcat (sql, "FROM geometry_columns_field_infos\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'SpatialView' AS layer_type, ");
            strcat (sql, "view_name AS table_name, ");
            strcat (sql, "view_geometry AS geometry_column, ");
            strcat (sql, "ordinal AS ordinal, ");
            strcat (sql, "column_name AS column_name, ");
            strcat (sql, "null_values AS null_values, ");
            strcat (sql, "integer_values AS integer_values, ");
            strcat (sql, "double_values AS double_values, ");
            strcat (sql, "text_values AS text_values, ");
            strcat (sql, "blob_values AS blob_values, ");
            strcat (sql, "max_size AS max_size, ");
            strcat (sql, "integer_min AS integer_min, ");
            strcat (sql, "integer_max AS integer_max, ");
            strcat (sql, "double_min AS double_min, ");
            strcat (sql, "double_max double_max\n");
            strcat (sql, "FROM views_geometry_columns_field_infos\n");
            strcat (sql, "UNION\n");
            strcat (sql, "SELECT 'VirtualShape' AS layer_type, ");
            strcat (sql, "virt_name AS table_name, ");
            strcat (sql, "virt_geometry AS geometry_column, ");
            strcat (sql, "ordinal AS ordinal, ");
            strcat (sql, "column_name AS column_name, ");
            strcat (sql, "null_values AS null_values, ");
            strcat (sql, "integer_values AS integer_values, ");
            strcat (sql, "double_values AS double_values, ");
            strcat (sql, "text_values AS text_values, ");
            strcat (sql, "blob_values AS blob_values, ");
            strcat (sql, "max_size AS max_size, ");
            strcat (sql, "integer_min AS integer_min, ");
            strcat (sql, "integer_max AS integer_max, ");
            strcat (sql, "double_min AS double_min, ");
            strcat (sql, "double_max double_max\n");
            strcat (sql, "FROM virts_geometry_columns_field_infos");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        static int
        create_sql_statements_log (sqlite3 * sqlite)
        {
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            /* creating the SQL_STATEMENTS_LOG table */
            strcpy (sql, "CREATE TABLE  IF NOT EXISTS ");
            strcat (sql, "sql_statements_log (\n");
            strcat (sql, "id INTEGER PRIMARY KEY AUTOINCREMENT,\n");
            strcat (sql,
                    "time_start TIMESTAMP NOT NULL DEFAULT '0000-01-01T00:00:00.000Z',\n");
            strcat (sql,
                    "time_end TIMESTAMP NOT NULL DEFAULT '0000-01-01T00:00:00.000Z',\n");
            strcat (sql, "user_agent TEXT NOT NULL,\n");
            strcat (sql, "sql_statement TEXT NOT NULL,\n");
            strcat (sql, "success INTEGER NOT NULL DEFAULT 0,\n");
            strcat (sql, "error_cause TEXT NOT NULL DEFAULT 'ABORTED',\n");
            strcat (sql, "CONSTRAINT sqllog_success CHECK ");
            strcat (sql, "(success IN (0,1)))");
            ret = sqlite3_exec (sqlite, sql, NULL, NULL, &errMsg);
            if (ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free (errMsg);
                return 0;
            }
            return 1;
        }
        
        int createAdvancedMetaData (void *p_sqlite)	
        {
            /* creating the advanced MetaData tables */
            sqlite3 *sqlite = (sqlite3 *) p_sqlite;
            if(!create_views_geometry_columns(sqlite))
                return 0;
            if(!create_virts_geometry_columns(sqlite))
                return 0;
            if(!create_geometry_columns_statistics(sqlite))
                return 0;
            if(!create_views_geometry_columns_statistics(sqlite))
                return 0;
            if(!create_virts_geometry_columns_statistics(sqlite))
                return 0;
            if(!create_geometry_columns_field_infos(sqlite))
                return 0;
            if(!create_views_geometry_columns_field_infos(sqlite))
                return 0;
            if(!create_virts_geometry_columns_field_infos(sqlite))
                return 0;
            if(!create_geometry_columns_times(sqlite))
                return 0;
            if(!create_geometry_columns_auth(sqlite))
                return 0;
            if(!create_views_geometry_columns_auth(sqlite))
                return 0;
            if(!create_virts_geometry_columns_auth(sqlite))
                return 0;
            if(!create_geometry_columns_views(sqlite))
                return 0;
            if(!create_sql_statements_log(sqlite))
                return 0;
            
            return 1;
        }
        
        bool initSpatialMetaData(void *_sqlite)
        {
            sqlite3 *sqlite = (sqlite3*)_sqlite;
            char sql[4186];
            char *errMsg = NULL;
            int ret;
            
            /* creating the SPATIAL_REF_SYS table */
            strcpy(sql, "CREATE TABLE IF NOT EXISTS spatial_ref_sys (\n");
            strcat(sql, "srid INTEGER NOT NULL PRIMARY KEY,\n");
            strcat(sql, "auth_name TEXT NOT NULL,\n");
            strcat(sql, "auth_srid INTEGER NOT NULL,\n");
            strcat(sql, "ref_sys_name TEXT NOT NULL DEFAULT 'Unknown',\n");
            strcat(sql, "proj4text TEXT NOT NULL,\n");
            strcat(sql, "srtext TEXT NOT NULL DEFAULT 'Undefined')");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_W(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return false;
            }
            strcpy(sql, "CREATE UNIQUE INDEX idx_spatial_ref_sys \n");
            strcat(sql, "ON spatial_ref_sys (auth_srid, auth_name)");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_W(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return false;
            }
            //		updateSpatiaLiteHistory(sqlite, "spatial_ref_sys", NULL,
            //		                        "table successfully created");
            
            /* creating the GEOMETRY_COLUMNS table */
            if(!createGeometryColumns(sqlite))
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, "createGeometryColumns Failed");
                sqlite3_free(errMsg);
                return false;
            }
            
            /* creating the GEOM_COLS_REF_SYS view */
            strcpy(sql, "CREATE VIEW geom_cols_ref_sys AS\n");
            strcat(sql, "SELECT f_table_name, f_geometry_column, geometry_type,\n");
            strcat(sql, "coord_dimension, spatial_ref_sys.srid AS srid,\n");
            strcat(sql, "auth_name, auth_srid, ref_sys_name, proj4text, srtext\n");
            strcat(sql, "FROM geometry_columns, spatial_ref_sys\n");
            strcat(sql, "WHERE geometry_columns.srid = spatial_ref_sys.srid");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            //		updateSpatiaLiteHistory(sqlite, "geom_cols_ref_sys", NULL,
            //		                        "view 'geom_cols_ref_sys' successfully created");
            if(ret != SQLITE_OK)
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return false;
            }
            
            if(!createAdvancedMetaData(sqlite))
            {
                AMIGO_LOG_E(TAG, "SQL error: %s: %s\n", sql, errMsg);
                sqlite3_free(errMsg);
                return false;
            }
            
            /* creating the SpatialIndex VIRTUAL TABLE */
            strcpy(sql, "CREATE VIRTUAL TABLE SpatialIndex ");
            strcat(sql, "USING VirtualSpatialIndex()");
            ret = sqlite3_exec(sqlite, sql, NULL, NULL, &errMsg);
            
            return true;
        }
    }
}