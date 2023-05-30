
#include <mutex>
#include <unordered_map>
#include "CtiContainerInjector.hpp"

namespace cti_robot
{
namespace common
{
    static std::shared_ptr<IInjector> globalInjector;
    namespace impl
    {
        using namespace std::literals;
        class GeneralInjector : public IInjector
        {
        private:
            std::recursive_mutex m_mutex;
            std::unordered_map<const char*, boost::any> m_contains;
        public:
            void lock() override
            {
                m_mutex.lock();
            }
            void unlock() override
            {
                m_mutex.unlock();
            }
            boost::any resolveAny(const std::type_info& type) override
            {
                std::lock_guard<std::recursive_mutex> __lock(m_mutex);
                if (m_contains.find(type.name()) != m_contains.end())
                {
                    return m_contains[type.name()];
                }
                throw std::runtime_error("can\'t resolve type `"s + type.name() + "`");
            }
            void assembleAny(const std::type_info& type, boost::any object) override
            {
                std::lock_guard<std::recursive_mutex> __lock(m_mutex);
                if (object.empty())
                {
                    m_contains.erase(type.name());
                }
                else
                {
                    m_contains[type.name()] = object;
                }
            }
        };
    }

    std::shared_ptr<IInjector> getContainer() noexcept
    {
        if (!globalInjector)
        {
            auto injector = std::make_shared<impl::GeneralInjector>();
            globalInjector = std::dynamic_pointer_cast<IInjector>(injector);
        }
        return globalInjector;
    }

    void destoryContainer() noexcept
    {
        globalInjector.reset();
    }
}
}