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

#ifndef BUFFER_H
#define BUFFER_H

#include <condition_variable>
#include <mutex>
#include <queue>

namespace wolkabout
{
/**
 * @brief The Buffer class Concurrent buffer consists of one push queue, onto
 *        which new queue items are pushed, and one pop queue, from which
 *        queue items are poped
 *        Content of these two queues can be swaped as needed.
 */
template <class T> class Buffer
{
public:
    /**
     * @brief Buffer Constructs Buffer with empty push, and pop queues
     */
    Buffer();
    virtual ~Buffer();

    /**
     * @brief push Places T onto push queue
     * @param item T to be placed onto push queue
     */
    void push(T item);

    /**
     * @brief pop Acquires item from pop queue
     * @return First T in pop queue
     */
    T pop();

    /**
     * @brief isEmpty Returns whether push queue contains any T item
     * @return bool indicating if push queue is empty
     */
    bool isEmpty() const;

    /**
     * @brief swapBuffers Exchanges content of push, and pop queues
     */
    void swapBuffers();

private:
    std::queue<T> m_pushQueue;
    std::queue<T> m_popQueue;

    mutable std::mutex m_lock;
    std::condition_variable m_condition;
};

template <class T> Buffer<T>::Buffer()
{
}

template <class T> Buffer<T>::~Buffer()
{
}

template <class T> void Buffer<T>::push(T item)
{
    std::unique_lock<std::mutex> unique_lock(m_lock);

    m_pushQueue.push(item);

    m_condition.notify_one();
}

template <class T> T Buffer<T>::pop()
{
    if (m_popQueue.empty())
    {
        return nullptr;
    }

    T item = m_popQueue.front();
    m_popQueue.pop();
    return item;
}

template <class T> void Buffer<T>::swapBuffers()
{
    std::unique_lock<std::mutex> unique_lock(m_lock);

    if (m_pushQueue.empty())
    {
        m_condition.wait(unique_lock);
    }

    std::swap(m_pushQueue, m_popQueue);
}

template <class T> bool Buffer<T>::isEmpty() const
{
    std::unique_lock<std::mutex> unique_lock(m_lock);

    return m_pushQueue.empty();
}
}

#endif
