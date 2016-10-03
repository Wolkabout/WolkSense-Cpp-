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

#include "WolkSense.h"

#include <chrono>
#include <thread>

namespace wolkabout
{
char getSign(int value)
{
    return value > 0 ? '+' : '-';
}

WolkSense::WolkSense(Device device) : m_device(std::move(device))
{
    m_publishingService = std::unique_ptr<PublishingService>(new PublishingService(m_device));
    m_readingsBuffer = std::unique_ptr<ReadingsBuffer>(new ReadingsBuffer());

    m_isAutoPublishingEnabled = false;
}

WolkSense::~WolkSense()
{
    if (m_publishingJob != nullptr && m_publishingJob->joinable())
    {
        m_isAutoPublishingEnabled = false;
        m_publishingJob->join();
    }
}

void WolkSense::addReading(const Reading::Type& type, const std::string& value, unsigned long time)
{
    m_readingsBuffer->addReading(type, value, time);
}

void WolkSense::addReading(const Reading::Type& type, const int value, unsigned long time)
{
    m_readingsBuffer->addReading(type, std::to_string(value), time);
}

void WolkSense::addReading(const Reading::Type& type, const Direction& value, unsigned long time)
{
    if (type == Reading::Type::ACCELERATION || type == Reading::Type::GYRO || type == Reading::Type::MAGNET)
    {
        char buffer[24];
        snprintf(buffer, 24, "%c%02d%c%02d%c%02d", getSign(value.x), std::abs(value.x), getSign(value.y),
                 std::abs(value.y), getSign(value.z), std::abs(value.z));

        m_readingsBuffer->addReading(type, std::string(buffer), time);
    }
}

void WolkSense::setServiceListener(std::weak_ptr<ServiceListener> serviceListener)
{
    m_publishingService->setListener(serviceListener);
}

void WolkSense::startAutoPublish(unsigned long interval)
{
    m_isAutoPublishingEnabled = true;

    m_publishingJob = std::unique_ptr<std::thread>(new std::thread([this, interval](void) {

        while (m_isAutoPublishingEnabled)
        {
            try
            {
                publishReadingsPeriodic();

                std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            }
            catch (mqtt::exception&)
            {
                m_isAutoPublishingEnabled = false;
            }
        }
    }));
}

void WolkSense::stopAutoPublish()
{
    if (m_publishingJob && m_publishingJob->joinable())
    {
        m_isAutoPublishingEnabled = false;
        m_publishingJob->join();
    }
}

void WolkSense::publishReadings()
{
    if (m_readingsBuffer->isEmpty() || m_isAutoPublishingEnabled)
    {
        return;
    }

    m_publishingService->publish(m_readingsBuffer->getFormatedData());
}

void WolkSense::publishReadingsPeriodic()
{
    if (m_readingsBuffer->isEmpty())
    {
        return;
    }

    m_publishingService->publish(m_readingsBuffer->getFormatedData());
}
}
