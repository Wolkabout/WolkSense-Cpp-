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

#ifndef READINGSBUFFER_H
#define READINGSBUFFER_H

#include "Buffer.h"
#include "Reading.h"

#include <memory>
#include <sstream>
#include <string>

namespace wolkabout
{
/**
 * @brief The ReadingsBuffer class holds stored readings, and formats data for sending to MQTT
 *        according to WolkAbout protocol
 */
class ReadingsBuffer
{
public:
    /**
     * @brief ReadingsBuffer Constructs empty ReadingsBuffer
     */
    ReadingsBuffer();
    virtual ~ReadingsBuffer();

    /**
     * @brief isEmpty Returns whether ReadingBuffer contains any Reading
     * @return
     */
    bool isEmpty() const;

    /**
     * @brief addReading Adds Reading into ReadingsBuffer with specified Posix time as timestamp
     * @param time Posix time when value was read
     * @param type Type of Reading
     * @param value Value read
     */
    void addReading(const Reading::Type& type, const std::string& value, unsigned long time = 0);

    /**
     * @brief getFormatedData
     * @return std::string containing Reading(s) serialized appropriate to WolkAbout Cloud
     */
    std::string getFormatedData();

private:
    static constexpr const char* TIME_PREFIX = "R";
    static constexpr const char* RTC = "RTC ";
    static constexpr const char* MESSAGE_START = "READINGS ";
    static constexpr const char* PREFIX_DELIMITER = ":";
    static constexpr const char* READING_DELIMITER = ",";
    static constexpr const char* TIME_DELIMITER = "|";
    static constexpr const char* MESSAGE_END = ";";

    Buffer<std::shared_ptr<Reading>> m_buffer;

    void appendReadings(const std::vector<std::shared_ptr<Reading>>& readings, std::stringstream& data);

    unsigned long getPosixTime();
};
}

#endif
