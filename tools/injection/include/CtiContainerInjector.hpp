
#ifndef _CTI_ROBOT_COMMON_CONTAINERINJECTOR_HPP_
#define _CTI_ROBOT_COMMON_CONTAINERINJECTOR_HPP_

#include <memory>
#include <utility>
#include <typeinfo>
#include <boost/any.hpp>

namespace cti_robot
{
namespace common
{
    class IInjector
    {
        public:
            virtual void lock() = 0;
            virtual void unlock() = 0;

            virtual boost::any resolveAny(const std::type_info& type) = 0;
            virtual void assembleAny(const std::type_info& type, boost::any object) = 0;
            
            template <typename T> T resolve()
            {
                auto obj = resolveAny(typeid(std::remove_reference_t<T>()));
                return boost::any_cast<T>(obj);
            }

            template <typename T> T tryResolve()
            {
                try
                {
                    auto obj = resolveAny(typeid(std::remove_reference_t<T>()));
                    return boost::any_cast<T>(obj);
                }
                catch(...)
                {
                    return T();
                }
            }
            
            template <typename T> void assemble(T&& value)
            {
                assembleAny(typeid(std::remove_reference_t<T>()), value);
            }
            
            template <typename T> void erase()
            {
                assembleAny(typeid(std::remove_reference_t<T>()), boost::any());
            }
    };
    
    std::shared_ptr<IInjector> getContainer() noexcept;
    void destoryContainer() noexcept;
}
}

#endif //_CTI_ROBOT_COMMON_CONTAINERINJECTOR_HPP_
