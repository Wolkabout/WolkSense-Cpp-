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
 * @brief The Direction struct
 *
 * This struct is used for reading types that have x, y and z axises like: ACCELERATION, GYRO, and MAGNET.
 */
struct Direction
{
    /**
     * @brief Direction constructor.
     * @param xx x value
     * @param yy y value
     * @param zz z value
     */
    Direction(int xx = 0, int yy = 0, int zz = 0) : x(xx), y(yy), z(zz) {}
    int x;
    int y;
    int z;
};

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
     * @brief Adds Reading into ReadingsBuffer with specified Posix time as timestamp
     * @param type Type of Reading::Type
     * @param value Value read
     * @param time Posix time when value was read
     */
    void addReading(const Reading::Type& type, const std::string& value, unsigned long time = 0);

    /**
     * @brief Adds Reading into ReadingsBuffer with specified Posix time as timestamp
     * @param type Type of Reading::Type
     * @param value Value read
     * @param time Posix time when value was read
     */
    void addReading(const Reading::Type& type, const int value, unsigned long time = 0);

    /**
     * @brief Adds Reading into ReadingsBuffer with specified Posix time as timestamp
     * @param type Type of Reading::Type
     * @param value Direction value that is used for types: ACCELERATION, GYRO, and MAGNET
     * @param time Posix time when value was read
     */
    void addReading(const Reading::Type& type, const Direction& value, unsigned long time = 0);

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
