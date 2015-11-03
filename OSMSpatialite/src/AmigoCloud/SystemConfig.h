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


#ifndef amigocloud_SystemConfig_h
#define amigocloud_SystemConfig_h

#include <string>

namespace AmigoCloud
{

class SystemConfig
{
public:
    static void setCacheDir(const std::string &dir);
    static std::string &getCacheDir();

    static void setFilesDir(const std::string &dir);
    static std::string &getFilesDir();
    
    static void setDevEnvironment(bool dev);
    static bool getDevEnvironment();
    
    static std::string getBaseURL();
    static std::string getBaseURLBase();
    static void setBaseURLBase(const std::string &url);

    static void setOfflineMode(bool enabled) {_offlineMode = enabled;}
    static bool isOfflineMode() {return _offlineMode;}

    static void setOnLine(bool online) {_online = online;}
    static bool isOnLine() {return _online && !_offlineMode;}

    static bool isInFollowMode() { return _followMode; }
    static void setFollowMode(bool enabled) { _followMode = enabled; }

    static void syncWiFiOnly(bool enable);
    static bool isSyncWiFiOnly();

    static void setIgnoreSSLCertificate(bool ignore);
    static bool getIgnoreSSLCertificate();

    /* 
    * setConnectionType()
    * connection :
    *   0 - No Internet,
    *   1 - 3G, 
    *   2 - WiFi
    */
    static void setConnectionType(int connection);
    static int  getConnectionType();

    static std::string loadTrackingIDS();
    static std::string getMixpanelID();
    static std::string getGoogleAnalyticsID();
    static std::string getRavenID();

private:

    static std::string _parseTrackingMixpanelJSON(const std::string &json);
    static std::string _parseTrackingGoogleAnalyticsJSON(const std::string &json);
    static std::string _parseTrackingRavenJSON(const std::string &json);

    static std::string _cacheDir;
    static std::string _filesDir;
    static bool _devEnvironment;
    static bool   _online;
    static bool _offlineMode;
    static bool _followMode;
    static bool _ignoreSSL;
};

}

#endif
