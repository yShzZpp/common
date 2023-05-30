#include <csignal>
#include <execinfo.h>
#include "cti_save_crash_log.hpp"
#include "spdlog/spdlog.h"
#include <cpprest/json.h>
#include <iomanip>
#include <sstream>
#include <ctime>
#include <string>
#include <unistd.h>
#include <exception>
#include <sys/mman.h>

namespace cti
{
    namespace robot
    {
        struct HandlerInfo
        {
            std::string robot_num;
            std::string process_name;
            std::string home;
            ShutdownFunction process_shutdown;
        }gCtiRobotHandlerInfo;

        std::string gVersion;
        
        void signal_handler(int signo)
        {
            char buff[64] = {0};
            sprintf(buff,"cat /proc/%d/maps", getpid());
            int ret = system((const char*) buff);

            void *buffer[32];
            char **strings;
            char log[1024 * 10] = {0};

            int nptrs = backtrace(buffer, 32);//#define BACKTRACE_SIZE   16

            strings = backtrace_symbols(buffer, nptrs);
            if (strings == NULL) 
            {
                perror("backtrace_symbols");
                exit(EXIT_FAILURE);
            }

            time_t now = std::time(nullptr);

            std::stringstream transTime;
            transTime << std::put_time(std::localtime(&now), "%F %T");
            std::string current_time = transTime.str();

            int len = 0;
            len += sprintf(log + len, "机器编号: %s\r\n", 
                                        gCtiRobotHandlerInfo.robot_num.c_str());
            len += sprintf(log + len, "版本信息: %s\r\n", 
                                        gVersion.c_str());
            len += sprintf(log + len, "进程名: %s\r\n", 
                                        gCtiRobotHandlerInfo.process_name.c_str());
            len += sprintf(log + len, "崩溃时间: %s\r\n", 
                                        current_time.c_str());
            len += sprintf(log + len, "崩溃信息: the abnormal signal No : %d \r\n", signo);
            for (int i = 1; i < nptrs; i++)
            {
                len += sprintf(log + len, "[%02d] %s\r\n", i, strings[i]);
            }

            auto message = web::json::value::object({
                        std::make_pair("msgtype", web::json::value("markdown")),
                        std::make_pair("markdown", web::json::value::object({
                            std::make_pair("content", web::json::value::string(log))
                        }))
                    });

            time_t timep;
            struct tm *p = NULL;
            time(&timep); 
            p = localtime(&timep);

            char FileName[256] = {0};
            sprintf(FileName, "%s/.ros/crash_%s_%s.log.%04d_%02d_%02d_%02d_%02d_%02d", 
                                gCtiRobotHandlerInfo.home.c_str(),
                                gCtiRobotHandlerInfo.robot_num.c_str(),
                                gCtiRobotHandlerInfo.process_name.c_str(),
                                (1900 + p->tm_year), 
                                p->tm_mon + 1, 
                                p->tm_mday,
                                p->tm_hour,
                                p->tm_min,
                                p->tm_sec);

            FILE * pFile;
            pFile = fopen(FileName,"w+");
            if (pFile != NULL)
            {
                fputs(message.serialize().c_str(),pFile);
                fclose(pFile);
            }

            free(strings);

            gCtiRobotHandlerInfo.process_shutdown(FileName);

            if (signo > 0)
            {
                signal(signo, SIG_DFL); /* 恢复信号默认处理 */
                raise(signo);           /* 重新发送信号 */
            }

            return;
        }

        void terminate_handle()
        {
            SPDLOG_ERROR("some throw not catch, shutdown ros node!");
            signal_handler(-1);
        }

        std::string findUbuntuPackageVersion(const std::string& name) noexcept try
        {
            if (name.empty()) throw std::runtime_error("invalid package name.");
            int fd = ::open("/var/lib/dpkg/status", O_RDONLY);
            if (fd == -1) throw std::runtime_error("cann\'t open dpkg\'s status file.");
            size_t size = ::lseek(fd, 0, SEEK_END);
            char* addr = (char*)::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
            close(fd);
            if (addr == reinterpret_cast<char*>(-1)) throw std::runtime_error("mmap file fail, " + std::generic_category().message(errno));

            if (auto start = std::strstr(reinterpret_cast<char*>(addr), name.data()))
            {
                auto end = std::strstr(start, "\n\n");
                auto v = std::strstr(start, "Version: ");

                if (end && v && end > v)
                {
                    char version[256];
                    if (sscanf(v, "Version: %[^\n]\n", version) == 1)
                    {
                        return std::string(version);
                    }
                }
            }

            ::munmap(addr, size);
            throw std::runtime_error("not fount version info.");
        }
        catch(std::exception& ex)
        {
            SPDLOG_ERROR("find ubuntu package version failed {}", ex.what());
            return "Unknown";
        }

        void registerSignalHandler(std::string robot_num, std::string process_name, ShutdownFunction process_shutdown)
        {
            gCtiRobotHandlerInfo.home = std::getenv("HOME");
            gCtiRobotHandlerInfo.robot_num = robot_num;
            gCtiRobotHandlerInfo.process_name = process_name;
            gCtiRobotHandlerInfo.process_shutdown = process_shutdown;

            gVersion = findUbuntuPackageVersion("br-robot-release");

            SPDLOG_INFO("Register Signal Handler: {} of robot {} and path:{}/.ros/ ", process_name, robot_num, gCtiRobotHandlerInfo.home);

            signal(SIGABRT, signal_handler); //程序的异常终止，如调用 abort. --6
            signal(SIGSEGV, signal_handler); //无效内存访问 --11
            signal(SIGBUS, signal_handler);  //非法内存访问 --7
            signal(SIGFPE, signal_handler);  //浮点数运算异常 --8
            signal(SIGILL, signal_handler);  //检测非法指令 --4
            
            std::set_terminate(terminate_handle); //当throw的异常没有捕获的时候
            return;
        }
    }
}