/*
Copyright (c) 2019, ROSIN-project
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
contributors may be used to endorse or promote products derived from
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RXROS_INCLUDE_RXROS_H_
#define RXROS_INCLUDE_RXROS_H_

#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <cassert>
#include <mutex>
#include <linux/input.h>
#include <rxcpp/rx.hpp>
#include <ros/ros.h>
#include <ros/console.h>


namespace rxros
{
    class node: public ros::NodeHandle
    {
    private:
        node() = default;

    public:
        ~node() = default;

        // subscribe and advertiseService are overloaded ros::Nodehandle functions for subscribing to a topic and service.
        // The special about these two functions is that they allow the callback to be a std::functions.
        // This means that it will be possible to subscribe to a topic using a lambda expression as a callback.
        // Ideas borrowed from https://github.com/OTL/roscpp14
        template<class T>
        ros::Subscriber subscribe(const std::string& topic, uint32_t queue_size, const std::function<void(const T&)>& callback) {
            return ros::NodeHandle::subscribe<T>(topic, queue_size, static_cast<boost::function<void(const T&)>>(callback));
        }

        template<class T>
        ros::ServiceServer advertiseService(const std::string& service, const std::function<bool(typename T::Request&, typename T::Response&)> callback) {
            return ros::NodeHandle::advertiseService<typename T::Request, typename T::Response>(service, static_cast<boost::function<bool(typename T::Request&, typename T::Response&)>>(callback));
        }

        static auto get_handle() {
            static node self;
            return self;
        }
    }; // end of class node


    class exception
    {
    private:
        exception() = default;

    public:
        ~exception() = default;
        static auto system_error(const int errCode, const std::string &msg) {
            return std::make_exception_ptr(std::system_error(std::error_code(errCode, std::generic_category()), msg));
        }
    }; // end of class exception


    class logging: public std::ostringstream
    {
    private:
        enum LogLevel {DEBUG, INFO, WARN, ERROR, FATAL};
        LogLevel logLevel;

    public:
        logging() = default;
        ~logging() override {
            switch(logLevel) {
                case DEBUG:
                    ROS_DEBUG("%s", str().c_str());
                    break;
                case INFO:
                    ROS_INFO("%s", str().c_str());
                    break;
                case WARN:
                    ROS_WARN("%s", str().c_str());
                    break;
                case ERROR:
                    ROS_ERROR("%s", str().c_str());
                    break;
                case FATAL:
                    ROS_FATAL("%s", str().c_str());
                    break;
                default:
                    ROS_FATAL("Ups!!!!");
                    break;
            }
        }

        logging& debug() {
            logLevel = DEBUG;
            return *this;
        }

        logging& info() {
            logLevel = INFO;
            return *this;
        }

        logging& warn() {
            logLevel = WARN;
            return *this;
        }

        logging& error() {
            logLevel = ERROR;
            return *this;
        }

        logging& fatal() {
            logLevel = FATAL;
            return *this;
        }
    }; // end of class logging


    class parameter
    {
    private:
        parameter() = default;

    public:
        ~parameter() = default;

        template<typename T>
        static auto get(const std::string& name, const T& default_value) {
            T param;
            node::get_handle().param<T>(name, param, default_value);
            return param;
        }

        static auto get(const std::string& name, const int default_value) {
            int param;
            node::get_handle().param(name, param, default_value);
            return param;
        }

        static auto get(const std::string& name, const double default_value) {
            double param;
            node::get_handle().param(name, param, default_value);
            return param;
        }

        static auto get(const std::string& name, const char* default_value) {
            return get<std::string>(name, default_value);
        }

        static auto get(const std::string& name, const std::string& default_value) {
            return get<std::string>(name, default_value);
        }
    }; // end of class parameter


    namespace observable
    {
        template<class T>
        static auto from_topic(const std::string& topic, const uint32_t queueSize = 10)
        {
            auto observable = rxcpp::observable<>::create<T>(
                [=](rxcpp::subscriber<T> subscriber) {
                    auto callback = [=](const T &val) {
                        static std::mutex mutex;
                        std::lock_guard<std::mutex> guard(mutex);
                        subscriber.on_next(val);};
                    ros::Subscriber ros_subscriber(node::get_handle().subscribe<T>(topic, queueSize, callback));
                    ros::waitForShutdown();});
            return observable.subscribe_on(rxcpp::synchronize_new_thread());
        }

        template<class T>
        static auto from_device(const std::string& device_name)
        {
            auto observable = rxcpp::observable<>::create<T>(
                [=](rxcpp::subscriber<T> subscriber) {
                    int fd = open(device_name.c_str(), O_RDONLY | O_NONBLOCK);
                    if (fd < 0)
                        subscriber.on_error(rxros::exception::system_error(errno, std::string("Cannot open device ") + device_name));
                    else {
                        fd_set readfds; // initialize file descriptor set.
                        FD_ZERO(&readfds);
                        FD_SET(fd, &readfds);
                        T event{};
                        bool errReported = false;
                        while (ros::ok()) {
                            int rc = select(fd + 1, &readfds, nullptr, nullptr, nullptr);  // wait for input on keyboard device
                            if (rc == -1 && errno == EINTR) { // select was interrupted. This is not an error but we exit the loop
                                subscriber.on_completed();
                                close(fd);
                                break;
                            }
                            else if (rc == -1 || rc == 0) { // select failed and we issue an error.
                                subscriber.on_error(rxros::exception::system_error(errno, std::string("Failed to read device ") + device_name));
                                close(fd);
                                errReported = true;
                                break;
                            }
                            else if (FD_ISSET(fd, &readfds)) {
                                ssize_t sz = read(fd, &event, sizeof(T)); // read element from device.
                                if (sz == -1) {
                                    subscriber.on_error(rxros::exception::system_error(errno, std::string("Failed to read device ") + device_name));
                                    close(fd);
                                    errReported = true;
                                    break;
                                }
                                else if (sz == sizeof(T)) {
                                    subscriber.on_next(event); // populate the event on the
                                }
                            }
                        }
                        if (!errReported) {
                            subscriber.on_completed();
                        }
                    }
                });
            return observable.subscribe_on(rxcpp::synchronize_new_thread());
        }

        // Parse the robot.yaml file and create an observable stream for the configuration of the sensors and actuators
        static auto from_yaml(const std::string& aNamespace)
        {
            return rxcpp::observable<>::create<XmlRpc::XmlRpcValue>(
                [=](rxcpp::subscriber<XmlRpc::XmlRpcValue> subscriber) {
                    XmlRpc::XmlRpcValue robot_config;
                    node::get_handle().getParam(aNamespace, robot_config);
                    assert (robot_config.getType() == XmlRpc::XmlRpcValue::TypeArray);
                    for (int i = 0; i < robot_config.size(); i++) {
                        XmlRpc::XmlRpcValue device_config(robot_config[i]);
                        subscriber.on_next(device_config);
                    }
                    subscriber.on_completed();
                });
        }
    } // end of namespace observable
} // end of namespace rxros


namespace rxros
{
    namespace operators
    {
        auto sample_with_frequency(const double frequency) {
            return [=](auto&& source) {
                const std::chrono::milliseconds durationInMs(static_cast<long>(1000.0/frequency));
                return rxcpp::observable<>::interval(durationInMs).with_latest_from(
                    [=](const auto intervalObsrv, const auto sourceObsrv) { return sourceObsrv; }, source);};}


        template<class Coordination>
        auto sample_with_frequency(const double frequency, Coordination coordination) {
            return [=](auto&& source) {
                const std::chrono::milliseconds durationInMs(static_cast<long>(1000.0/frequency));
                return rxcpp::observable<>::interval(durationInMs, coordination).with_latest_from(
                    [=](const auto intervalObsrv, const auto sourceObsrv) { return sourceObsrv; }, source);};}


        template<typename T>
        auto publish_to_topic(const std::string &topic, const uint32_t queue_size = 10) {
            return [=](auto&& source) {
                ros::Publisher publisher(rxros::node::get_handle().advertise<T>(topic, queue_size));
                source.observe_on(rxcpp::synchronize_new_thread()).subscribe(
                    [=](const T& msg) {publisher.publish(msg);});
                return source;};}


        template<typename T>
        auto call_service(const std::string& service) {
            return [=](auto&& source) {
                return rxcpp::observable<>::create<T>(
                    [=](rxcpp::subscriber<T> subscriber) {
                        ros::ServiceClient service_client(rxros::node::get_handle().serviceClient<T>(service));
                        source.subscribe(
                            [=](const T& msg) {
                                T res = std::move(msg);
                                if (service_client.call(res))
                                    subscriber.on_next(res);},
                            [=](const std::exception_ptr error){subscriber.on_error(error);},
                            [=](){subscriber.on_completed();});});};}
    } // end namespace operators
} // end namespace rxros


#endif // RXROS_INCLUDE_RXROS_H_
