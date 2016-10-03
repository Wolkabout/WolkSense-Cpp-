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

#include "Device.h"
#include "Reading.h"
#include "SampleServiceListener.h"
#include "WebClient.h"
#include "WebClientException.h"
#include "WolkSense.h"

#include <iostream>
#include <memory>
#include <string>

wolkabout::Device activateDeviceForUser(const std::string& userEmail, const std::string& userPassword,
                                        const std::string& deviceName);

void publishSampleReadings(const wolkabout::Device device);

int main(int argc, char** argv)
{
    if (argc == 4)
    {
        auto userEmail = argv[1];
        auto userPassword = argv[2];
        auto deviceName = argv[3];

        try
        {
            auto device = activateDeviceForUser(userEmail, userPassword, deviceName);

            publishSampleReadings(device);
        }
        catch (wolkabout::WebClientException& exception)
        {
            std::cout << exception.getMessage();
            return -1;
        }
        catch (mqtt::exception& exception)
        {
            std::cout << exception.get_message() << std::endl;
            return -1;
        }
    }
    else if (argc == 3)
    {
        auto deviceSerial = argv[1];
        auto devicePassword = argv[2];

        wolkabout::Device device(deviceSerial, devicePassword);

        try
        {
            publishSampleReadings(device);
        }
        catch (mqtt::exception& exception)
        {
            std::cout << exception.get_message() << std::endl;
            return -1;
        }
    }
    else
    {
        std::cout << "*- WolkConnect-Cpp- example -*" << std::endl;
        std::cout << "Usage:" << std::endl;
        std::cout << "Activating device:" << std::endl;
        std::cout << "\t" << argv[0] << " {userEmail} {userPassword} {deviceName}" << std::endl << std::endl;

        std::cout << "Publishing sample readings: " << std::endl;
        std::cout << "\t" << argv[0] << " {deviceSerial} {devicePassword}" << std::endl;
    }

    return 0;
}

wolkabout::Device activateDeviceForUser(const std::string& userEmail, const std::string& userPassword,
                                        const std::string& deviceName)
{
    try
    {
        wolkabout::Device device = wolkabout::WebClient::activateDeviceForUser(userEmail, userPassword, deviceName);

        std::cout << "Activated device " << deviceName << " for account " << userEmail << std::endl;

        std::cout << "Device serial   : " << device.getSerial() << std::endl;
        std::cout << "Device password : " << device.getPassword() << std::endl;

        return device;
    }
    catch (wolkabout::WebClientException&)
    {
        throw;
    }
}

void publishSampleReadings(const wolkabout::Device device)
{
    auto wolksense = std::unique_ptr<wolkabout::WolkSense>(new wolkabout::WolkSense(device));
    auto listener = std::shared_ptr<SampleServiceListener>(new SampleServiceListener());

    wolksense->setServiceListener(listener);

    wolksense->startAutoPublish();

    std::cout << std::endl << "Press enter to publish first set of readings" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    wolksense->addReading(wolkabout::Reading::HUMIDITY, 80, 1474286400);
    wolksense->addReading(wolkabout::Reading::TEMPERATURE, 264, 1474286400);
    wolksense->addReading(wolkabout::Reading::STEPS, 12, 1474286400);
    wolksense->addReading(wolkabout::Reading::LIGHT, 150, 1474286400);
    wolksense->addReading(wolkabout::Reading::ACCELERATION, wolkabout::Direction{5, -5, 5}, 1474286400);

    std::cout << std::endl << "Press enter to publish second set of readings" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    wolksense->addReading(wolkabout::Reading::HUMIDITY, 79);
    wolksense->addReading(wolkabout::Reading::TEMPERATURE, 275);
    wolksense->addReading(wolkabout::Reading::STEPS, 14);
    wolksense->addReading(wolkabout::Reading::LIGHT, 167);
    wolksense->addReading(wolkabout::Reading::ACCELERATION, wolkabout::Direction{1, -3, 2});

    std::cout << std::endl << "Press enter to publish last set of readings" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    wolksense->stopAutoPublish();
    wolksense->addReading(wolkabout::Reading::HUMIDITY, 67);
    wolksense->addReading(wolkabout::Reading::TEMPERATURE, 282);
    wolksense->addReading(wolkabout::Reading::STEPS, 22);
    wolksense->addReading(wolkabout::Reading::LIGHT, 200);
    wolksense->addReading(wolkabout::Reading::ACCELERATION, wolkabout::Direction{2, -4, 3});
    wolksense->publishReadings();

    std::cout << std::endl << "Press enter to exit" << std::endl;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
