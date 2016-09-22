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

#include "ReadingsBuffer.h"

#include <algorithm>
#include <map>
#include <memory>
#include <utility>
#include <vector>

namespace wolkabout
{
ReadingsBuffer::ReadingsBuffer()
{
}
ReadingsBuffer::~ReadingsBuffer()
{
}

bool ReadingsBuffer::isEmpty() const
{
    return m_buffer.isEmpty();
}

void ReadingsBuffer::addReading(const Reading::Type& type, const std::string& value)
{
    auto reading = std::make_shared<Reading>(getPosixTime(), type, value);

    m_buffer.push(reading);
}

void ReadingsBuffer::addReading(unsigned long time, const Reading::Type& type, const std::string& value)
{
    auto reading = std::make_shared<Reading>(time, type, value);

    m_buffer.push(reading);
}

std::string ReadingsBuffer::getFormatedData()
{
    m_buffer.swapBuffers();

    std::vector<std::shared_ptr<Reading>> readings;

    std::shared_ptr<Reading> reading;
    while ((reading = m_buffer.pop()) != nullptr)
    {
        readings.push_back(reading);
    }

    std::map<unsigned long, std::vector<std::shared_ptr<Reading>>> groupedReadings;

    std::for_each(readings.cbegin(), readings.cend(), [&groupedReadings](std::shared_ptr<Reading> reading) {

        unsigned long timeStamp = reading->getTimestamp();

        if (groupedReadings.find(timeStamp) == groupedReadings.end())
        {
            groupedReadings.emplace(std::pair<unsigned long, std::vector<std::shared_ptr<Reading>>>(timeStamp, {}));
        }

        groupedReadings.at(timeStamp).push_back(reading);
    });

    std::stringstream data;
    data << RTC << getPosixTime() << MESSAGE_END;

    data << MESSAGE_START;

    for (auto it = groupedReadings.begin(), end = groupedReadings.end(); it != end; ++it)
    {
        unsigned long time = it->first;

        data << TIME_PREFIX << PREFIX_DELIMITER << time << READING_DELIMITER;

        appendReadings(it->second, data);

        if (std::next(it, 1) != end)
        {
            data << TIME_DELIMITER;
        }
    }

    data << MESSAGE_END;
    return data.str();
}

void ReadingsBuffer::appendReadings(const std::vector<std::shared_ptr<Reading>>& readings, std::stringstream& data)
{
    auto isFirstReading = true;

    for (const auto& it : readings)
    {
        Reading reading = *it.get();

        if (!isFirstReading)
        {
            data << READING_DELIMITER;
        }

        isFirstReading = false;
        data << Reading::getPrefix(reading) << PREFIX_DELIMITER << reading.getValue();
    }
}

unsigned long ReadingsBuffer::getPosixTime()
{
    auto timeSinceEpoch = std::chrono::system_clock::now().time_since_epoch();
    return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count());
}
}
