#ifndef _CTI_ROBOT_SHARED_MUTEX_VARIABLE_HPP_
#define _CTI_ROBOT_SHARED_MUTEX_VARIABLE_HPP_

#include <mutex>
/* 
 * --yxl-- 2021/05/30
*/

namespace cti_robot
{
    template <typename T>
    class MutexVariable
    {
        private:

            T info;
            std::mutex m;
        
        public:

            MutexVariable(){}

            MutexVariable(T t)
            {
                info = t;
            }

            void Set(T t)
            {
                std::lock_guard<std::mutex> lk(m);
                info = t;
                return;
            }

            T Get()
            {
                std::lock_guard<std::mutex> lk(m);
                return info;
            }
    };
}

#endif //_CTI_ROBOT_SHARED_MUTEX_VARIABLE_HPP_