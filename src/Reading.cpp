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

#include "Reading.h"

#include <chrono>
#include <utility>

namespace wolkabout
{
Reading::Reading(unsigned long timestamp, Reading::Type type, std::string value)
: m_timestamp(std::move(timestamp)), m_type(std::move(type)), m_value(std::move(value))
{
}

unsigned long Reading::getTimestamp() const
{
    return m_timestamp;
}

Reading::Type Reading::getType() const
{
    return m_type;
}

const std::string& Reading::getValue() const
{
    return m_value;
}

std::string Reading::getPrefix(const Reading& reading)
{
    switch (reading.getType())
    {
    case Type::ACCELERATION:
        return "ACL";
    case Type::GYRO:
        return "GYR";
    case Type::MAGNET:
        return "MAG";
    case Type::LIGHT:
        return "LT";
    case Type::TEMPERATURE:
        return "T";
    case Type::HUMIDITY:
        return "H";
    case Type::PRESSURE:
        return "P";
    case Type::BATTERY:
        return "B";
    case Type::HEARTRATE:
        return "BPM";
    case Type::STEPS:
        return "STP";
    case Type::CALORIES:
        return "KCAL";
    default:
        return "UNKNOWN";
    }
}
}
