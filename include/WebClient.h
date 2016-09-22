/*
 * Copyright 2016 WolkAbout Technology s.r.o.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef WOLKSENSEWEBCLIENT_H
#define WOLKSENSEWEBCLIENT_H

#include "Device.h"
#include "restclient-cpp/connection.h"

#include <string>

namespace wolkabout
{
/**
 * @brief The WebClient class Provides method for activating device on WolkAbout Cloud
 */
class WebClient
{
public:
    /**
     * @brief The DeviceType enum Indicates type of wolkabout::Device to be activated
     */
    enum class DeviceType
    {
        WOLKSENSOR,
        HEXIWEAR,
        GENERIC
    };

    /**
     * @brief The HttpResponseCode enum Standard HTTP request response codes
     */
    enum HttpResponseCode
    {
        OK = 200,
        UNAUTHORIZED = 401,
        CONFLICT = 409
    };

    /**
     * @brief getDeviceTypeString Returns DeviceType as std::string
     * @param deviceType DeviceType to be converted to std::string
     * @return std::string of DeviceType
     */
    static std::string getDeviceTypeString(const DeviceType& deviceType);

    /**
     * @brief activateDeviceForUser Activates wolkabout::Device on WolkAbout Cloud
     * @param userEmail Email of account on which is wolkabout::Device to be activated
     * @param userPassword Account password
     * @param deviceName Name of wolkabout::Device to be activated
     * @return wolkabout::Device containing serial and password assigned to activated device
     */
    static Device activateDeviceForUser(const std::string& userEmail, const std::string& userPassword,
                                        const std::string& deviceName);

private:
    static constexpr const char* HOST = "https://api.wolksense.com/api";

    static std::string getAccessToken(const std::string& userEmail, const std::string& userPassword);

    static std::string getRandomSerial(const std::string& accessToken, const DeviceType& deviceType);

    static Device activateDevice(const std::string& accessToken, const std::string& deviceSerial,
                                 const std::string& deviceName);

    static RestClient::Connection getRestClientConnection(const std::string& accessToken = "");
};
}

#endif
