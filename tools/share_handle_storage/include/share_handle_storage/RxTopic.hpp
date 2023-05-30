#ifndef _CTI_ROBOT_RX_RX_TOPIC_HPP_
#define _CTI_ROBOT_RX_RX_TOPIC_HPP_

#include "ros/ros.h"
#include "rxcpp/rx.hpp"

namespace cti_robot
{
namespace rx
{
    template<class T>
    class RxTopic
    {
        private:

            rxcpp::subjects::subject<T> mSubject;

            ros::Subscriber mSubHandler;

        public:

            RxTopic()
            {
            }

            void Init(std::string name, unsigned int cache)
            {
                ros::NodeHandle n;
                mSubHandler = n.subscribe<T>(name, cache, &RxTopic::callback, this);
            }

            void Unint()
            {
                mSubject.get_subscriber().on_completed();
                mSubHandler.shutdown();
            }

            rxcpp::observable<T> GetObservable() const
            {
                return mSubject.get_observable();
            }
        
        private:

            void callback(const T msg)
            {
                mSubject.get_subscriber().on_next(msg);
            }
    };
}
}

#endif //_CTI_ROBOT_RX_RX_TOPIC_HPP_