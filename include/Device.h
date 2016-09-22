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

#ifndef WOLKSENSEDEVICE_H
#define WOLKSENSEDEVICE_H

#include <string>

namespace wolkabout
{
/**
 * @brief The Device class holds device serial and password assigned, by WolkAbout Cloud,
 *        when activated on WolkAbout Cloud
 */
class Device
{
public:
    /**
     * @brief Device Constructs WolkAbout Device using given serial and password
     * @param serial Device serial obtained from WolkAbout Cloud prior to registration
     * @param password Device password assigned by WolkAbout Cloud after registration
     */
    Device(std::string serial, std::string password);

    virtual ~Device();

    /**
     * @brief getSerial Returns device serial assigned by WolkAbout Cloud
     * @return WolkAbout Device serial
     */
    const std::string& getSerial() const noexcept;

    /**
     * @brief getPassword Returns device password assigned by WolkAbout Cloud
     * @return WolkAbout Device password
     */
    const std::string& getPassword() const noexcept;

private:
    std::string m_serial;

    std::string m_password;
};
}
#endif
