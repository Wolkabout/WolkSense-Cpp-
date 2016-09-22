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

#ifndef SAMPLESERVICELISTENER_H
#define SAMPLESERVICELISTENER_H

#include "PublishingService.h"

#include <string>

/**
 * @brief The SampleServiceListener class Implements wolkabout::ServiceListener
 *        with example methods for handling callback events
 */
class SampleServiceListener : public wolkabout::ServiceListener
{
public:
    /**
     * @brief SampleServiceListener Default constructor
     */
    SampleServiceListener();

    virtual ~SampleServiceListener();

    void messageArrived(std::string topic, std::string message) override;

    void connectionLost(std::string cause) override;

    void deliveryComplete(std::string message) override;
};

#endif
