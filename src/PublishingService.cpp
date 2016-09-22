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

#include "PublishingService.h"

#include <iostream>
#include <string>
#include <utility>

namespace wolkabout
{
PublishingService::PublishingService(Device device) : m_device(std::move(device))
{
}

PublishingService::~PublishingService()
{
}

void PublishingService::publish(const std::string& message)
{
    if (!m_mqttClient)
    {
        initMqtt();
    }

    try
    {
        mqtt::message_ptr pubmsg = std::make_shared<mqtt::message>(message);
        pubmsg->set_qos(QOS);

        m_mqttClient->publish(TOPIC + m_device.getSerial(), pubmsg);
    }
    catch (mqtt::exception&)
    {
        throw;
    }
}

void PublishingService::setListener(std::weak_ptr<ServiceListener> serviceListener)
{
    m_serviceListener = serviceListener;
}

void PublishingService::initMqtt()
{
    m_mqttClient =
      std::unique_ptr<mqtt::async_client>(new mqtt::async_client(HOST, m_device.getSerial().c_str(), NULL));

    mqtt::connect_options connectOptions;
    connectOptions.set_clean_session(true);
    connectOptions.opts_.username = m_device.getSerial().c_str();
    connectOptions.opts_.password = m_device.getPassword().c_str();

    MQTTAsync_SSLOptions sslOptions = MQTTAsync_SSLOptions_initializer;
    sslOptions.enableServerCertAuth = 1;

    m_mqttClient->set_callback(*this);

    const std::string& trustStore = CERTIFICATE_NAME;
    sslOptions.trustStore = trustStore.c_str();

    connectOptions.opts_.ssl = &sslOptions;

    mqtt::itoken_ptr connectToken = m_mqttClient->connect(connectOptions);
    connectToken->wait_for_completion(10000);    // const

    // throw
    if (!connectToken->is_complete())
    {
    }
    if (!m_mqttClient->is_connected())
    {
    }
}

void PublishingService::message_arrived(const std::string& topic, mqtt::message_ptr msg)
{
    if (auto listener = m_serviceListener.lock())
    {
        listener->messageArrived(topic, msg->get_payload());
    }
}

void PublishingService::connection_lost(const std::string& cause)
{
    if (auto listener = m_serviceListener.lock())
    {
        listener->connectionLost(cause);
    }
}

void PublishingService::delivery_complete(mqtt::idelivery_token_ptr tok)
{
    if (auto listener = m_serviceListener.lock())
    {
        listener->deliveryComplete(tok->get_message()->get_payload());
    }
}
}
