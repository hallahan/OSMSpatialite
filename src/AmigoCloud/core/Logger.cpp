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


#include <core/Logger.h>
#include <core/Timestamp.h>

#include <stdio.h>

#ifdef __ANDROID__
#include <android/log.h>
#define printf(...) __android_log_print(ANDROID_LOG_INFO, "log", __VA_ARGS__)
#endif

#include <stdarg.h>

// #define DEBUG_LOG

namespace AmigoCloud
{
    namespace Logger
    {
        //    #define SHOW_DELTA_TIME
        
        static int DEFAULT_DEBUG_LOG_LEVEL = Control::WARNING;
        
        std::map<std::string, int> Control::_debugLevels;
        
        class LoggerInit
        {
        public:
            LoggerInit()
            {
                // Control::setDebugLevel("OfflineArea", Control::INFO);
                // Control::setDebugLevel("VBO", Control::INFO);
                // Control::setDebugLevel("VersionMigration", Control::INFO);
                // Control::setDebugLevel("SocketIO", Control::INFO);
                
                //                 Control::setDebugLevel("AmigoCloudAPI", Control::INFO);
                //                Control::setDebugLevel("ProjectAPI", Control::INFO);
                // Control::setDebugLevel("UserAPI", Control::INFO);
                //                Control::setDebugLevel("GlobeAPI", Control::INFO);
                // Control::setDebugLevel("DatasetAPI", Control::INFO);
                //                Control::setDebugLevel("SupportFilesAPI", Control::INFO);
                //                Control::setDebugLevel("BaselayerAPI", Control::INFO);
                
                //                Control::setDebugLevel("Thread", Control::INFO);
                //                Control::setDebugLevel("ThreadPool", Control::INFO);
                
                //                Control::setDebugLevel("Mutex", Control::INFO);
                //                Control::setDebugLevel("TouchManager", Control::INFO);
                
                //                Control::setDebugLevel("ScopedPrint", Control::INFO);
                //                Control::setDebugLevel("Changeset", Control::VERBOSE);
                //                Control::setDebugLevel("Database", Control::INFO);
                
                // Control::setDebugLevel("User", Control::INFO);
                //                Control::setDebugLevel("Dataset", Control::INFO);
                //                Control::setDebugLevel("RelatedTables", Control::INFO);
                //                Control::setDebugLevel("DBTable", Control::INFO);
                //                Control::setDebugLevel("Generalization", Control::INFO);
                // Control::setDebugLevel("ProjectDB", Control::INFO);
                //                Control::setDebugLevel("QueryHelper", Control::INFO);
                
                //                Control::setDebugLevel("DatabaseManager", Control::INFO);
                //                Control::setDebugLevel("Project", Control::INFO);
                // Control::setDebugLevel("AmigoRest", Control::INFO);
                // Control::setDebugLevel("RestClient", Control::INFO);
                //                Control::setDebugLevel("JSONUtils", Control::INFO);
                //                Control::setDebugLevel("NetworkCache", Control::INFO);
                //                Control::setDebugLevel("Forms", Control::INFO);
                
                //                Control::setDebugLevel("SupportFiles", Control::VERBOSE);
                //                Control::setDebugLevel("AmigoApplication", Control::INFO);
                
                //	              Control::setDebugLevel("ThickPolyLine", Control::INFO);
                // Control::setDebugLevel("VectorLayer", Control::INFO);
                //                Control::setDebugLevel("VectorLayerEditor", Control::VERBOSE);
                // Control::setDebugLevel("VectorTile", Control::VERBOSE);
                // Control::setDebugLevel("VectorDataCache", Control::VERBOSE);
                //                  Control::setDebugLevel("VectorCache", Control::INFO);
                // Control::setDebugLevel("Style", Control::VERBOSE);
                //                Control::setDebugLevel("TransformParser", Control::VERBOSE);
                // Control::setDebugLevel("GPSLayer", Control::INFO);
                //                Control::setDebugLevel("BinT", Control::INFO);
                
                //                Control::setDebugLevel("Points", Control::INFO);
                //                Control::setDebugLevel("VectorShape", Control::INFO);
                // Control::setDebugLevel("ShapeEditor", Control::INFO);
                //                Control::setDebugLevel("UIBar", Control::INFO);
                //                Control::setDebugLevel("UIButton", Control::INFO);
                //                Control::setDebugLevel("UIHandler", Control::INFO);
                
                //                Control::setDebugLevel("Globe", Control::INFO);
                //                Control::setDebugLevel("ImageUtils", Control::INFO);
                // Control::setDebugLevel("PatchGenerator", Control::INFO);
                //  Control::setDebugLevel("tCache", Control::INFO);
                // Control::setDebugLevel("fCache", Control::INFO);
                // Control::setDebugLevel("GlobeSceneHandler", Control::INFO);
                //                Control::setDebugLevel("GlobeHandler", Control::INFO);
                // Control::setDebugLevel("GlobeStateSet", Control::INFO);
                //                Control::setDebugLevel("RasterLayer", Control::INFO);
                //                Control::setDebugLevel("Baselayers", Control::INFO);
                //                Control::setDebugLevel("ResetManager", Control::INFO);
                //	                Control::setDebugLevel("EventHandler", Control::INFO);
                //                Control::setDebugLevel("GlobePicker", Control::INFO);
                //                Control::setDebugLevel("BitmapHandler", Control::INFO);
                
            }
        };
        static LoggerInit loggerInit;
        
        void Control::setDebugLevel(const std::string &tag, int level)
        {
            _debugLevels[tag]=level;
        }
        
        int Control::getDebugLevel(const std::string &tag)
        {
            const char *ptr = tag.c_str();
            if(ptr==NULL)
                return DEFAULT_DEBUG_LOG_LEVEL;
            std::map<std::string, int>::iterator it = _debugLevels.find(tag);
            if(it!=_debugLevels.end())
                return (*it).second;
            else
                return DEFAULT_DEBUG_LOG_LEVEL;
        }
        
        static char *file;
        static int line;
        static Timestamp lastTimestamp;
        
//        static double getDeltaTime()
//        {
//            double dt = Timestamp::deltaNowMs(lastTimestamp);
//            lastTimestamp = Timestamp();
//            return dt/1000.0;
//        }
        
#define BUFFER_SIZE 2048
        
        static void debugLogFormatV(const std::string &tag, const char *format,...) {
#ifdef DEBUG_LOG
            printf("DEBUG_AMIGO_LOG_V '%s'\n", tag.c_str());
#endif
            int level = Control::getDebugLevel(tag);
            if(level > Control::VERBOSE)
                return;
            
            char buffer[BUFFER_SIZE]="";
            va_list args;
            va_start (args, format);
            vsnprintf (buffer, BUFFER_SIZE-1, format, args);
#ifndef SHOW_DELTA_TIME
            printf("<%s:%d>: Info: %s", file, line, buffer);
#else
            printf("<%s:%d>:%.3f Info: %s", file, line, getDeltaTime(), buffer);
#endif
            va_end(args);
        }
        
        DebugLogFormatV debugLogSetupV(const char *file0,int line0) {
            file = (char*)file0;
            line = line0;
            return debugLogFormatV;
        }
        
        //#define BUFFER_SIZE 2048
        
        static void debugLogFormatI(const std::string &tag, const char *format,...) {
#ifdef DEBUG_LOG
            printf("DEBUG_AMIGO_LOG_I '%s'\n", tag.c_str());
#endif
            
            int level = Control::getDebugLevel(tag);
            if(level > Control::INFO)
                return;
            
            char buffer[BUFFER_SIZE]="";
            va_list args;
            va_start (args, format);
            vsnprintf (buffer, BUFFER_SIZE-1, format, args);
#ifndef SHOW_DELTA_TIME
            printf("<%s:%d>: Info: %s", file, line, buffer);
#else
            printf("<%s:%d>:%.3f Info: %s", file, line, getDeltaTime(), buffer);
#endif
            va_end(args);
        }
        
        DebugLogFormatI debugLogSetupI(const char *file0,int line0) {
            file = (char*)file0;
            line = line0;
            return debugLogFormatI;
        }
        
        static void debugLogFormatW(const std::string &tag, const char *format,...) {
#ifdef DEBUG_LOG
            printf("DEBUG_AMIGO_LOG_W '%s'\n", tag.c_str());
#endif
            
            int level = Control::getDebugLevel(tag);
            if(level > Control::WARNING)
                return;
            
            char buffer[BUFFER_SIZE]="";
            va_list args;
            va_start (args, format);
            vsnprintf (buffer, BUFFER_SIZE-1, format, args);
#ifndef SHOW_DELTA_TIME
            printf("<%s:%d>: Warning: %s", file, line, buffer);
#else
            printf("<%s:%d>:%.3f Warning: %s", file, line, getDeltaTime(), buffer);
#endif
            va_end(args);
        }
        
        DebugLogFormatW debugLogSetupW(const char *file0,int line0) {
            file = (char*)file0;
            line = line0;
            return debugLogFormatW;
        }
        
        static void debugLogFormatE(const std::string &tag, const char *format,...) {
#ifdef DEBUG_LOG
            printf("DEBUG_AMIGO_LOG_E '%s'\n", tag.c_str());
#endif
            int level = Control::getDebugLevel(tag);
            if(level > Control::ERROR)
                return;
            
            char buffer[BUFFER_SIZE]="";
            va_list args;
            va_start (args, format);
            vsnprintf (buffer, BUFFER_SIZE-1, format, args);
#ifndef SHOW_DELTA_TIME
            printf("<%s:%d>: Error: %s", file, line, buffer);
#else
            printf("<%s:%d>:%.3f Error: %s", file, line, getDeltaTime(), buffer);
#endif
            va_end(args);
        }
        
        DebugLogFormatE debugLogSetupE(const char *file0,int line0) {
            file = (char*)file0;
            line = line0;
            return debugLogFormatE;
        }
    }
}