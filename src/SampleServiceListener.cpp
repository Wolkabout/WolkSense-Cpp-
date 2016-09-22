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

#include "SampleServiceListener.h"

#include <iostream>

SampleServiceListener::SampleServiceListener()
{
}
SampleServiceListener::~SampleServiceListener()
{
}

void SampleServiceListener::messageArrived(std::string topic, std::string message)
{
    std::cout << "Message arrived: " << message << " on topic " << topic << std::endl;
}

void SampleServiceListener::connectionLost(std::string cause)
{
    std::cout << "Connection lost: " << cause << std::endl;
}
void SampleServiceListener::deliveryComplete(std::string message)
{
    std::cout << "Delivery complete: " << message << std::endl;
}
