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


#include <core/SystemConfig.h>
//#include <core/DatabaseManager.h>
#include <stdio.h>
#include <core/RestClient.h>
//#include <core/AmigoConfig.h>

namespace AmigoCloud
{   

#define TRACKING_IDS_TAG "TrackingIDsTag"
#define MIXPANEL_ID_TAG "MixpanelIDTag"
#define GOOGLE_ANALYTICS_ID_TAG "GoogleAnalyticsIDTag"
#define RAVEN_ID_TAG "RavenIDTag"

#define DEFAULT_MIXPANEL_ID ""
#define DEFAULT_GOOGLE_ANALYTICS_ID " "
#define DEFAULT_RAVEN_ID ""
   
std::string SystemConfig::_cacheDir;
std::string SystemConfig::_filesDir;
bool SystemConfig::_devEnvironment=false;
bool SystemConfig::_online = true;
bool SystemConfig::_offlineMode = false;
bool SystemConfig::_followMode = false;
bool SystemConfig::_ignoreSSL = false;

void SystemConfig::setCacheDir(const std::string &dir)
{
    _cacheDir=dir;
}

std::string &SystemConfig::getCacheDir()
{
    return _cacheDir;
}

void SystemConfig::setFilesDir(const std::string &dir)
{
    _filesDir=dir;
}

std::string &SystemConfig::getFilesDir()
{
    return _filesDir;
}

//void SystemConfig::setDevEnvironment(bool dev){
//    _devEnvironment = dev;
//    if(_devEnvironment)
//        setBaseURLBase( BASE_URL_BASE_DEV );
//    else
//        setBaseURLBase( BASE_URL_BASE_PROD );
//}

bool SystemConfig::getDevEnvironment()
{
    return _devEnvironment;
}

//std::string SystemConfig::getBaseURL()
//{
//    return std::string(URL_PREFIX) + getBaseURLBase() + std::string(API_PREFIX);
//}

#define BASE_URL_BASE_TAG "BaseURLBaseTag"

//void SystemConfig::setBaseURLBase(const std::string &url)
//{
//    APIv1::DatabaseManager::instance()->setConfig(BASE_URL_BASE_TAG, url);
//}

//std::string SystemConfig::getBaseURLBase()
//{
//    std::string baseURL = APIv1::DatabaseManager::instance()->getConfig(BASE_URL_BASE_TAG);
//    
//    if(baseURL.empty())
//    {
//        baseURL = BASE_URL_BASE_PROD;
//        APIv1::DatabaseManager::instance()->setConfig(BASE_URL_BASE_TAG, BASE_URL_BASE_PROD);
//    }
//
//    return baseURL;
//}

#define SYNC_WIFI_ONLY_TAG "SyncWiFiOnlyTag"

//void SystemConfig::syncWiFiOnly(bool enable)
//{
//    APIv1::DatabaseManager::instance()->setConfig(SYNC_WIFI_ONLY_TAG, enable?"1":"0");
//}

bool SystemConfig::isSyncWiFiOnly()
{
#if 1
    return false;
#else
    std::string t = APIv1::DatabaseManager::instance()->getConfig(SYNC_WIFI_ONLY_TAG);
    return !t.empty() && t == "1";
#endif
}

void SystemConfig::setIgnoreSSLCertificate(bool ignore)
{
    _ignoreSSL = ignore;
}

bool SystemConfig::getIgnoreSSLCertificate()
{
    return _ignoreSSL;
}

static int sConenctionType=-1;

void SystemConfig::setConnectionType(int connection)
{
    sConenctionType = connection;
}

int SystemConfig::getConnectionType()
{
    return sConenctionType;
}

//std::string SystemConfig::loadTrackingIDS()
//{
//    std::string url;
//    url= getBaseURL() + "utils/tracking/";
//    RestClient rc("core/AmigoCore");
//    RestClient::response resp = rc.get(url);
//    if(!resp.isBad())
//    {
//        if(!resp.body.empty())
//            APIv1::DatabaseManager::instance()->setConfig(TRACKING_IDS_TAG, resp.body);
//        return resp.body;
//    } else
//    {
//        return APIv1::DatabaseManager::instance()->getConfig(TRACKING_IDS_TAG);
//    }
//}

//std::string SystemConfig::_parseTrackingMixpanelJSON(const std::string &json)
//{
//    std::string mixpanelID;
//    if(!json.empty())
//    {
//        Document document;
//        if (document.Parse<0>(json.c_str()).HasParseError())
//        {
//            return mixpanelID;
//        }
//        mixpanelID = getJSONString(document, "mixpanel_project_token", DEFAULT_MIXPANEL_ID);
//        if(!mixpanelID.empty())
//            APIv1::DatabaseManager::instance()->setConfig(MIXPANEL_ID_TAG, mixpanelID);
//
//    } else
//    {
//        mixpanelID = APIv1::DatabaseManager::instance()->getConfig(MIXPANEL_ID_TAG);
//        if(mixpanelID.empty())
//            mixpanelID = DEFAULT_MIXPANEL_ID;
//    }
//    return mixpanelID;
//}

//std::string SystemConfig::_parseTrackingGoogleAnalyticsJSON(const std::string &json)
//{
//    std::string gaID;
//    if(!json.empty())
//    {
//        Document document;
//        if (document.Parse<0>(json.c_str()).HasParseError())
//        {
//            return gaID;
//        }
//        gaID = getJSONString(document, "google_analytics_mobile_account", DEFAULT_GOOGLE_ANALYTICS_ID);
//        if(!gaID.empty())
//            APIv1::DatabaseManager::instance()->setConfig(GOOGLE_ANALYTICS_ID_TAG, gaID);
//
//    } else
//    {
//        gaID = APIv1::DatabaseManager::instance()->getConfig(GOOGLE_ANALYTICS_ID_TAG);
//        if(gaID.empty())
//            gaID = DEFAULT_GOOGLE_ANALYTICS_ID;
//    }
//    return gaID;
//}

//std::string SystemConfig::_parseTrackingRavenJSON(const std::string &json)
//{
//    std::string ravenID;
//    if(!json.empty())
//    {
//        Document document;
//        if (document.Parse<0>(json.c_str()).HasParseError())
//        {
//            return ravenID;
//        }
//        ravenID = getJSONString(document, "raven_mobile_dsn", DEFAULT_RAVEN_ID);
//        if(!ravenID.empty())
//            APIv1::DatabaseManager::instance()->setConfig(RAVEN_ID_TAG, ravenID);
//
//    } else
//    {
//        ravenID = APIv1::DatabaseManager::instance()->getConfig(RAVEN_ID_TAG);
//        if(ravenID.empty())
//            ravenID = DEFAULT_RAVEN_ID;
//    }
//    return ravenID;
//}

//std::string SystemConfig::getMixpanelID()
//{
//    std::string json = APIv1::DatabaseManager::instance()->getConfig(TRACKING_IDS_TAG);
//    return _parseTrackingMixpanelJSON(json);
//}

//std::string SystemConfig::getGoogleAnalyticsID()
//{
//    std::string json = APIv1::DatabaseManager::instance()->getConfig(TRACKING_IDS_TAG);
//    return _parseTrackingGoogleAnalyticsJSON(json);
//}

//std::string SystemConfig::getRavenID()
//{
//    std::string json = APIv1::DatabaseManager::instance()->getConfig(TRACKING_IDS_TAG);
//    return _parseTrackingRavenJSON(json);
//   
//}

}