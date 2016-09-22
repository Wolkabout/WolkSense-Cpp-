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

#ifndef PUBLISHINGSERVICE_H
#define PUBLISHINGSERVICE_H

#include "Device.h"
#include "mqtt/async_client.h"

#include <memory>
#include <string>

namespace wolkabout
{
/**
 * @brief The ServiceListener class provides interface which can be implemented to define custom listener
 *        for events fired by wolkabout::PublishingService
 */
class ServiceListener
{
public:
    virtual ~ServiceListener() {}
    /**
     * @brief messageArrived Callback function invoked on receiving message from MQTT broker
     * @param topic Topic on which is message received
     * @param message Content of message received
     */
    virtual void messageArrived(std::string topic, std::string message) = 0;

    /**
     * @brief connectionLost Callback function invoked on losing connection to MQTT broker
     * @param cause Cause of connection loss
     */
    virtual void connectionLost(std::string cause) = 0;

    /**
     * @brief deliveryComplete Callback function invoked on successful message delivery to MQTT broker
     * @param message Content of message delivered
     */
    virtual void deliveryComplete(std::string message) = 0;
};

/**
 * @brief The PublishingService class exposes interface for communication with MQTT broket, it provides publish method
 *        for publishing messages to MQTT broker, and option to implement custom listener which will receive events
 *        such as message arrival from MQTT broker, loss of connection to MQTT broker, and delivery complete to MQTT
 *        broker.
 */
class PublishingService : public mqtt::callback
{
public:
    /**
     * @brief PublishingService Constructs PublishingService used to publish and/or receive messages, as Device, from
     * MQTT broker
     * @param device Device for which message publishing/receiving is performed
     */
    explicit PublishingService(Device device);

    virtual ~PublishingService();

    /**
     * @brief publish Publishes message as appropriate Device
     * @param message Message to be sent
     */
    void publish(const std::string& message);

    /**
     * @brief setListener Sets MQTT callback listener
     * @param serviceListener Class implementing ServiceListener interface
     */
    void setListener(std::weak_ptr<ServiceListener> serviceListener);

private:
    const Device m_device;

    const std::string TOPIC = "sensors/";
    const std::string CERTIFICATE_NAME = "ca.crt";

    const std::string HOST = "ssl://wolksense.com:8883";
    const int QOS = 1;

    void initMqtt();

    void message_arrived(const std::string& topic, mqtt::message_ptr msg);
    void connection_lost(const std::string& cause);
    void delivery_complete(mqtt::idelivery_token_ptr token);

    std::unique_ptr<mqtt::async_client> m_mqttClient;

    std::weak_ptr<ServiceListener> m_serviceListener;
};
}

#endif
