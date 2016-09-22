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

#include "WebClient.h"

#include "Device.h"
#include "WebClientException.h"
#include "json/json.h"
#include "restclient-cpp/connection.h"

#include <cstdio>

using json = nlohmann::json;

namespace wolkabout
{
Device WebClient::activateDeviceForUser(const std::string& userEmail, const std::string& userPassword,
                                        const std::string& deviceName)
{
    auto accessToken = getAccessToken(userEmail, userPassword);

    auto deviceSerial = getRandomSerial(accessToken, DeviceType::GENERIC);

    return activateDevice(accessToken, deviceSerial, deviceName);
}

std::string WebClient::getAccessToken(const std::string& userEmail, const std::string& userPassword)
{
    RestClient::Connection connection = getRestClientConnection();

    const int jsonMaxBodySize = 200;
    char body[jsonMaxBodySize];

    std::snprintf(body, jsonMaxBodySize, R"({"email":"%s","password":"%s"})", userEmail.c_str(), userPassword.c_str());

    RestClient::Response response = connection.post("/signIn", body);

    if (response.code == HttpResponseCode::UNAUTHORIZED)
    {
        throw WebClientException("Invalid credentials\n");
    }
    else if (response.code != HttpResponseCode::OK)
    {
        throw WebClientException("Error authenticating user\n");
    }

    json jsonBody = json::parse(response.body);

    return jsonBody["accessToken"].get<std::string>();
}

std::string WebClient::getRandomSerial(const std::string& accessToken, const DeviceType& deviceType)
{
    RestClient::Connection connection = getRestClientConnection(accessToken);

    RestClient::Response response = connection.get("/v3/devices/random_serial?type=" + getDeviceTypeString(deviceType));

    if (response.code == HttpResponseCode::UNAUTHORIZED)
    {
        throw WebClientException("User is not authorized to get a random serial for device\n");
    }
    else if (response.code != HttpResponseCode::OK)
    {
        throw WebClientException("Error getting random serial for device\n");
    }

    json jsonBody = json::parse(response.body);

    return jsonBody["serial"].get<std::string>();
}

Device WebClient::activateDevice(const std::string& accessToken, const std::string& deviceSerial,
                                 const std::string& deviceName)
{
    RestClient::Connection connection = getRestClientConnection(accessToken);

    const int jsonMaxBodySize = 100;
    char body[jsonMaxBodySize];

    std::snprintf(body, jsonMaxBodySize, R"({"name":"%s"})", deviceName.c_str());

    RestClient::Response response = connection.post("/v2/devices/" + deviceSerial, body);

    if (response.code == HttpResponseCode::UNAUTHORIZED)
    {
        throw WebClientException("User is not authorized to activate device\n");
    }
    else if (response.code == HttpResponseCode::CONFLICT)
    {
        throw WebClientException("Device with name " + deviceName + " already exists.\n");
    }
    else if (response.code != HttpResponseCode::OK)
    {
        throw WebClientException("Error activating device\n");
    }

    json jsonBody = json::parse(response.body);

    auto serial = deviceSerial;
    auto password = jsonBody["password"].get<std::string>();

    return Device(serial, password);
}

RestClient::Connection WebClient::getRestClientConnection(const std::string& accessToken)
{
    RestClient::Connection connection(std::string(WebClient::HOST));

    connection.AppendHeader("Content-Type", "application/json");
    connection.AppendHeader("Cache-Control", "no-cache");

    if (!accessToken.empty())
    {
        connection.AppendHeader("Authorization", accessToken);
    }

    return connection;
}

std::string WebClient::getDeviceTypeString(const DeviceType& deviceType)
{
    switch (deviceType)
    {
    case DeviceType::GENERIC:
        return "";

    case DeviceType::HEXIWEAR:
        return "HEXIWEAR";

    case DeviceType::WOLKSENSOR:
        return "WOLKSENSOR";

    default:
        return "";
    }
}
}
