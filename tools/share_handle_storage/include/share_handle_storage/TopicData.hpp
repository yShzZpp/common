#ifndef _CTI_ROBOT_TOPIC_DATA_MODEL_HPP_
#define _CTI_ROBOT_TOPIC_DATA_MODEL_HPP_

#include <ros/ros.h>
#include "ShareMutexVariable.hpp"
/* 
 * --yxl-- 2021/06/16
*/

namespace cti_robot
{
    template <class T>
    class TopicData
    {
        private:

            ros::Subscriber mSubHandler;
            MutexVariable<T> mT;
            std::function<void (T)> mHandler = nullptr;

        public: 

            TopicData(std::string topic_name, unsigned int cache_num, std::function<void (T t)> f = nullptr)
            {
                ros::NodeHandle n;
                mSubHandler = n.subscribe<T>(topic_name, cache_num, &TopicData::callback, this);
                mHandler = std::move(f);
            }

            void CleanData() {
                T t;
                mT.Set(t);
            }

            void SetData(T t)
            {
                mT.Set(t);
            }

            T GetData(){
                return mT.Get();
            }

            void Release(){
                mSubHandler.shutdown();
                mHandler = nullptr;
            }

            void SetHandler(std::function<void (T)> f)
            {
                mHandler = std::move(f);
            }
        
        private:

            void callback(const T msg){
                mT.Set(msg);
                if (mHandler != nullptr) mHandler(msg);
            }
    };
}

#endif //_CTI_ROBOT_TOPIC_DATA_MODEL_HPP_