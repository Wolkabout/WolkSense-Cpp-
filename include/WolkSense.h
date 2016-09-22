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

#ifndef WOLKSENSE_CPP
#define WOLKSENSE_CPP

#include "Device.h"
#include "PublishingService.h"
#include "Reading.h"
#include "ReadingsBuffer.h"

#include <atomic>
#include <memory>
#include <thread>

namespace wolkabout
{
/**
 * @brief The WolkSense class Provides required functionality to store new readings, and publish
 *        stored readings to WolkAbout Cloud, either concurrently by publishing every N milliseconds,
 *        or manually calling publish method
 */
class WolkSense
{
public:
    /**
     * @brief WolkSense Constructs WolkSense with given wolkabout::Device
     * @param device wolkabout::Device containing serial and password assigned by WolkAbout Cloud
     *        during activation procedure
     */
    WolkSense(Device device);
    virtual ~WolkSense();

    /**
     * @brief addReading Adds Reading into ReadingsBuffer with specified Posix time as timestamp
     * @param time Posix time when value was read
     * @param type Type of Reading::Type
     * @param value Value read
     */
    void addReading(unsigned long time, const Reading::Type& type, const std::string& value);

    /**
     * @brief addReading Adds Reading to be sent into buffer
     * @param type Reading::Type of reading
     * @param value Value read
     */
    void addReading(const Reading::Type& type, const std::string& value);

    /**
     * @brief setServiceListener
     * @param serviceListener
     */
    void setServiceListener(std::weak_ptr<ServiceListener> serviceListener);

    /**
     * @brief startAutoPublish Starts auto publishing of wolkabout::Reading(s) stored in buffer,
     *        with specified interval.
     * @param interval Interval between publish attempts in milliseconds
     */
    void startAutoPublish(unsigned long interval = 1000);

    /**
     * @brief stopAutoPublish Stops auto publishing, if previously started
     */
    void stopAutoPublish();

    /**
     * @brief publishReadings Performs manual publish of stored wolkabout::Reading(s)
     */
    void publishReadings();

private:
    Device m_device;

    std::unique_ptr<PublishingService> m_publishingService;
    std::unique_ptr<ReadingsBuffer> m_readingsBuffer;

    std::atomic<bool> m_isAutoPublishingEnabled;
    std::unique_ptr<std::thread> m_publishingJob;

    void publishReadingsPeriodic();
};
}

#endif
