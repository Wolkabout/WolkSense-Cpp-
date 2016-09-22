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

#ifndef READINGTYPE_H
#define READINGTYPE_H

#include <string>

namespace wolkabout
{
/**
 * @brief The Reading class Holds reading data, such as Posix time when the value was read,
 *        type of sensor read, and value read.
 */
class Reading
{
public:
    /**
     * @brief The Type enum Indicates type of sensor reading
     */
    enum Type
    {
        ACCELERATION,
        GYRO,
        MAGNET,
        LIGHT,
        TEMPERATURE,
        HUMIDITY,
        PRESSURE,
        BATTERY,
        HEARTRATE,
        STEPS,
        CALORIES
    };

    /**
     * @brief Reading Constructs Reading of Type type, and value read at timestamp
     * @param timestamp Posix time when the reading happened
     * @param type Indicates type of reading
     * @param value Sensor value read
     */
    Reading(unsigned long timestamp, Reading::Type type, std::string value);

    /**
     * @brief getTimestamp
     * @return Timestamp of Reading
     */
    unsigned long getTimestamp() const;

    /**
     * @brief getType
     * @return Type of Reading
     */
    Reading::Type getType() const;

    /**
     * @brief getValue
     * @return Returns read value
     */
    const std::string& getValue() const;

    /**
     * @brief getPrefix
     * @param reading
     * @return Reading Type as std::string
     */
    static std::string getPrefix(const Reading& reading);

private:
    const unsigned long m_timestamp;
    const Reading::Type m_type;
    const std::string m_value;
};
}

#endif
