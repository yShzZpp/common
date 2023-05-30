
#ifndef _CTI_ROBOT_SHARED_HANDLE_STORAGE_HPP_
#define _CTI_ROBOT_SHARED_HANDLE_STORAGE_HPP_

#include <map>
#include <memory>
// #include <utility>
#include <typeinfo>
#include <mutex>
// #include "spdlog/spdlog.h"

/* 
 * --yxl-- 2021/01/04
*/

namespace cti_robot
{
    class SharedHandleStorage
    {
        public:
            
            template <typename T>
            auto getHandle() -> std::shared_ptr<std::decay_t<T>>
            {
                std::lock_guard<std::recursive_mutex> lock(mStorageMutex);
                // SPDLOG_INFO("getHandle type name: {}, {}", mStorage.size(), typeid(std::shared_ptr<std::decay_t<T>>).name());
                if (mStorage.find(typeid(std::shared_ptr<std::decay_t<T>>).name()) != mStorage.end())
                {
                    // SPDLOG_INFO("getHandle get it");
                    return std::static_pointer_cast<std::decay_t<T>>(mStorage[typeid(std::shared_ptr<std::decay_t<T>>).name()]);
                }
                else
                {
                    // SPDLOG_INFO("getHandle get nullptr");
                    return std::shared_ptr<std::decay_t<T>>(nullptr);
                }
            }

            template <typename T>
            void saveHandle(T t)
            {
                std::lock_guard<std::recursive_mutex> lock(mStorageMutex);
                // SPDLOG_INFO("saveHandle type name: {}, {}", mStorage.size(), typeid(std::decay_t<T>).name());
                if (mStorage.find(typeid(std::decay_t<T>).name()) != mStorage.end())
                {
                    // SPDLOG_INFO("getHandle replace it");
                    mStorage[typeid(std::decay_t<T>).name()] = std::static_pointer_cast<void>(t);
                }
                else
                {
                    // SPDLOG_INFO("getHandle insert it");
                    mStorage.insert(std::pair<const char*, std::shared_ptr<void>>(typeid(std::decay_t<T>).name(),
                                                                                  std::static_pointer_cast<void>(t)));
                }
                return;
            }

            template <typename T>
            void deleteHandle()
            {
                std::lock_guard<std::recursive_mutex> lock(mStorageMutex);
                auto iter = mStorage.find(typeid(std::shared_ptr<std::decay_t<T>>).name());
                if (iter != mStorage.end())
                {
                    mStorage.erase(iter);
                }
                return;
            }

            SharedHandleStorage(SharedHandleStorage const &) = delete;

            void operator=(SharedHandleStorage const &) = delete;

            static SharedHandleStorage & GetStorageHandle()
            {
                static SharedHandleStorage mCase;
                return mCase;
            }

            void emptyHandle()
            {
                std::map<const char*, std::shared_ptr<void>> empty;
                mStorage.swap(empty);
                return;
            }

        private:

            SharedHandleStorage(){};
        
        private:

            std::recursive_mutex mStorageMutex;
            std::map<const char*, std::shared_ptr<void>> mStorage;
    };
}

template <typename U>
auto SHGetHandle() -> std::shared_ptr<std::decay_t<U>>
{
    return cti_robot::SharedHandleStorage::GetStorageHandle().getHandle<std::decay_t<U>>();
}

template <typename T>
void SHSaveHandle(T t)
{
    return cti_robot::SharedHandleStorage::GetStorageHandle().saveHandle(t);
}

template <typename T>
void SHDeleteHandle()
{
    return cti_robot::SharedHandleStorage::GetStorageHandle().deleteHandle<std::decay_t<T>>();
}

template <typename T>
void SHFreeHandle()
{
    if (auto tmp_ptr = SHGetHandle<T>())
    {
        SHDeleteHandle<T>();
        tmp_ptr.reset();
    }
    return;
}

#endif