/* This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

//#include "serialport/serialport.hpp"
#include "serialport.hpp"

#include <unistd.h>
#include <time.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <poll.h>

namespace ctirobot
{
/// Get CLOCK_MONOTONIC time in nanoseconds
static double NowNanosec() noexcept
{
    timespec tp;
    if (!clock_gettime(CLOCK_MONOTONIC, &tp)) {
        return (tp.tv_nsec + (tp.tv_sec * 1e9));
    } else {
        return 0.0;
    }
}
static int SetBlock(int const fd, bool const block) noexcept
{
    int ret;
    int e;
    // Get the file access mode and the file status flags; arg is ignored
    int ffs = ::fcntl(fd, F_GETFL, 0);
    if (ffs < 0) {
        e = errno;
        goto fail;
    }
    if (!block) {
        ret = ::fcntl(fd, F_SETFL, ffs | O_NONBLOCK);
    } else {
        ffs &= ~ O_NONBLOCK;
        ret = ::fcntl(fd, F_SETFL, ffs);
    }
    if (ret < 0) {
        e = errno;
        goto fail;
    }
    return 0;
fail:
    if (e) {
        return -e;
    } else {
        return -1;
    }
}
SerialPort::SerialPort(QueryMode const& queryMode) noexcept: queryMode(
    queryMode)
{
    this->device.set.deviceName = "/dev/ttyUSB0";
    this->device.set.baudRate = serial::kBaudRate115200;
    this->device.set.parityType = ParityType::NONE;
    this->device.set.flowControl = FlowControl::OFF;
    this->device.set.dataBits = serial::kDataBits8;
    this->device.set.stopBits = serial::kStopBits1;
    this->device.set.timeoutMillisec = 10;
    this->device.setDirty = kDirtyALL;
}
SerialPort::SerialPort(
    std::string const& portName, QueryMode const& queryMode) noexcept:
    queryMode(queryMode)
{
    this->device.set.deviceName = portName;
    this->device.set.baudRate = serial::kBaudRate115200;
    this->device.set.parityType = ParityType::NONE;
    this->device.set.flowControl = FlowControl::OFF;
    this->device.set.dataBits = serial::kDataBits8;
    this->device.set.stopBits = serial::kStopBits1;
    this->device.set.timeoutMillisec = 10;
    this->device.setDirty = kDirtyALL;
}
SerialPort::SerialPort(
    PortSetting const& portSetting, QueryMode const& queryMode) noexcept:
    queryMode(queryMode)
{
    this->device.set = portSetting;
    this->device.setDirty = kDirtyALL;
}
SerialPort::~SerialPort() noexcept
{
    this->close();
}
std::string SerialPort::getPortName() const noexcept
{
    boost::shared_lock<boost::shared_mutex> devReadLock(this->deviceRwlock);
    return this->device.set.deviceName;
}
int SerialPort::getBaudRate() const noexcept
{
    boost::shared_lock<boost::shared_mutex> devReadLock(this->deviceRwlock);
    return this->device.set.baudRate;
}
bool SerialPort::isOpen() const noexcept
{
    boost::shared_lock<boost::shared_mutex> deviceReadLock(
        this->deviceRwlock);
    return (this->device.fd >= 0);
}
void SerialPort::setPortName(std::string const& portName) noexcept
{
    boost::upgrade_lock<boost::shared_mutex> devUplock(
        this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> devWriteLock(devUplock);
    this->device.set.deviceName = portName;
}
void SerialPort::setBaudRate(int const baudRate, bool const update)
{
    boost::upgrade_lock<boost::shared_mutex> devUplock(this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> devWLock(devUplock);
    if (baudRate == this->device.set.baudRate) {
        return;
    }
    switch(baudRate) {
    case serial::kBaudRate50:
    case serial::kBaudRate75:
    case serial::kBaudRate134:
    case serial::kBaudRate150:
    case serial::kBaudRate200:
    case serial::kBaudRate1800:
#   if defined(SERIALPORT_B76800) && (SERIALPORT_B76800)
    case serial::kBaudRate76800:
#   endif
#   if defined(SERIALPORT_B230400) && defined(SERIALPORT_B4000000)
    case serial::kBaudRate230400:
    case serial::kBaudRate460800:
    case serial::kBaudRate500000:
    case serial::kBaudRate576000:
    case serial::kBaudRate921600:
    case serial::kBaudRate1000000:
    case serial::kBaudRate1152000:
    case serial::kBaudRate1500000:
    case serial::kBaudRate2000000:
    case serial::kBaudRate2500000:
    case serial::kBaudRate3000000:
    case serial::kBaudRate3500000:
    case serial::kBaudRate4000000:
#   endif
    case serial::kBaudRate110:
    case serial::kBaudRate300:
    case serial::kBaudRate600:
    case serial::kBaudRate1200:
    case serial::kBaudRate2400:
    case serial::kBaudRate4800:
    case serial::kBaudRate9600:
    case serial::kBaudRate19200:
    case serial::kBaudRate38400:
    case serial::kBaudRate57600:
    case serial::kBaudRate115200: {
        this->device.set.baudRate = baudRate;
        this->device.setDirty |= kDirtyBaudRate;
        if (update && this->__isOpen()) {
            this->__updatePortSetting();
        }
    } break;
    default:
        std::cerr << "ERROR: " << __FILE__ << "+" << __LINE__
            << ": does not support baudRate " << baudRate << '\n';
        throw std::logic_error("invalid baud rate");
    }
}
bool SerialPort::setRate(double const rate) noexcept
{
    std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
    if (!this->eventDriven) {
        return false;
    }
    this->eventDriven->rate = ::fabs(rate);
    if (::fabs(rate) >= 1e-6) {
        this->eventDriven->lastCallbackRecv = 0.0;
    } else {
        this->eventDriven->lastCallbackRecv = -1.0;
    }
    return true;
}
void SerialPort::handleIoEvs(short const ioevs) noexcept
{
    if (!ioevs) {
        return;
    }
    if (ioevs & POLLIN) {
        bool callbackable = false;
        // Cache to buffer when buffer enabled
        {
            boost::upgrade_lock<boost::shared_mutex> deviceUplock(
                this->deviceRwlock);
            boost::upgrade_to_unique_lock<boost::shared_mutex>
                deviceWriteLock(deviceUplock);
            std::lock_guard<std::mutex> eventDrivenLock(
                this->eventDrivenMutex);
            if (this->eventDriven->readBuffer) {
                uint32_t const oldsz = this->eventDriven->readBuffer->size();
                int64_t const bufsz =
                    this->eventDriven->readBuffer->capacity() - oldsz;
                if (bufsz > 0) {
                    // Pre size .. max
                    this->eventDriven->readBuffer->resize(
                        this->eventDriven->readBuffer->capacity());
                    int64_t const r = ::read(
                        this->device.fd,
                        this->eventDriven->readBuffer->data() + oldsz,
                        bufsz);
                    // Narrow size
                    if (r <= 0) {
                        this->eventDriven->readBuffer->resize(oldsz);
                    } else {
                        this->eventDriven->readBuffer->resize(oldsz + r);
                    }
                }
                if (this->eventDriven->readBuffer->size() >=
                    this->eventDriven->readBuffer->capacity()) {
                    // Cache full
                    callbackable = true;
                }
            }
        }
        int64_t maxAvailable = 0;
        bool updateTime;
        // Check and setup for callback
        {
            boost::shared_lock<boost::shared_mutex> deviceReadLock(
                this->deviceRwlock);
            std::lock_guard<std::mutex> eventDrivenLock(
                this->eventDrivenMutex);
            updateTime = (::fabs(this->eventDriven->rate) >= 1e-6);
            if (!callbackable) {
                // Cache not full or no cache
                if (!updateTime) {
                    // Cache not full or no cache and no limit rate
                    callbackable = true;
                } else {
                    // Cache not full or no cache and has limit rate
                    double const each = (1.0 / ::fabs(
                        this->eventDriven->rate)) * 1e9;
                    double const nownsec = NowNanosec();
                    if ((nownsec - this->eventDriven->lastCallbackRecv)
                        >= each) {
                        callbackable = true;
                    }
                }
            }
            // if (callbackable) {
            //     if (this->eventDriven->recvMode != RecvMode::Free) {
            //         // Cache full / timeup, and not free
            //         callbackable = false;
            //     } else {
            //         maxAvailable = this->__availableData();
            //     }
            // }
        }
        // // Callback when ok
        // if (callbackable && (maxAvailable >= 0)) {
        //     std::cout<<"call  shouldRect maxAvailable"<< maxAvailable<<std::endl;
        //     int64_t const ret = this->shouldRecv(maxAvailable);
        //     // Check and update time
        //     if (updateTime) {
        //         double const nownsec = NowNanosec();
        //         std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
        //         this->eventDriven->lastCallbackRecv = nownsec;
        //     }
        //     // Check ret
        //     if (ret < 0) {
        //         std::cerr << "ERROR: " << __FILE__ << "+" <<
        //             __LINE__ << ": recv fail: break\n";
        //         return;
        //     }
        // }
        // Check fail
        if (maxAvailable < 0) {
            {
                boost::upgrade_lock<boost::shared_mutex> deviceUplock(
                    this->deviceRwlock);
                boost::upgrade_to_unique_lock<boost::shared_mutex>
                    deviceWriteLock(deviceUplock);
                this->__doclose();
            }
        }
    }

    if (ioevs & POLLOUT) 
    {
        this->shouldSend();
    }

    if (ioevs & POLLERR) 
    {
        std::cout << "SerialPort::handleIoEvs::POLLERR\n";
        boost::upgrade_lock<boost::shared_mutex> deviceUplock(
            this->deviceRwlock);
        boost::upgrade_to_unique_lock<boost::shared_mutex>
            deviceWriteLock(deviceUplock);
        this->__doclose();
    }

    if (ioevs & POLLHUP) 
    {
        std::cout << "SerialPort::handleIoEvs::POLLHUP\n";
    }

    if (ioevs & POLLNVAL) 
    {
        std::cout << "SerialPort::handleIoEvs::POLLNVAL\n";
    }
}
void SerialPort::ioevsloop() noexcept
{
    boost::shared_ptr<EventDriven> eventDriven;
    {
        std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
        eventDriven = this->eventDriven;
        if (!eventDriven) {
            return;
        }
    }
    bool terminateEvDrv;
    {
        std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
        terminateEvDrv = eventDriven->terminate;
    }
    int pollNum;
    constexpr nfds_t nfds = 1;
    struct pollfd fds[nfds];
    bool reset = false;
    std::string portName;
    while (!terminateEvDrv) {
        // Check if can loop
        {
            std::cout<<"---------主线程循环---------"<<std::endl;
            boost::shared_lock<boost::shared_mutex> deviceReadLock(
                this->deviceRwlock);
            if (this->device.fd < 0) 
           {
                std::cout << __FILE__ << "+" << __LINE__ << ": bad fd\n"
                    << std::flush;
                goto maybeerror;
            }
            fds[0].fd = this->device.fd;
            fds[0].events = this->device.ioevs | POLLERR | POLLHUP | POLLNVAL;
        }
        // Loop
        std::cout << __FILE__ << "+" << __LINE__ << "进入 poll 监听事件循环: loop ..\n";
        // 进入 do poll 的循环 直到 __availableData 失败 把 reset 置为 true
        do {
            // Wait for events input
            errno = 0;
            pollNum = ::poll(fds, nfds, 10);
            if (-1 == pollNum) {
                int const e = errno;
                if (EINTR == e) {
                    continue;
                }
                std::cerr << "SerialPort::rwioloop: " << e << ", "
                    << ::strerror(e);
                goto maybeerror;
            } else if (pollNum > 0) {
                this->handleIoEvs(fds[0].revents);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            {
                std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
                terminateEvDrv = eventDriven->terminate;
            }
            {
                boost::shared_lock<boost::shared_mutex> deviceReadLock(
                    this->deviceRwlock);
                reset = this->device.reset;
            }
        } while((!terminateEvDrv) && (!reset));

        std::cout << __FILE__ << "+" << __LINE__ << ":退出 do while loop done\n"
            << std::flush;
        {
            // Fetch terminate again
            std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
            terminateEvDrv = eventDriven->terminate;
        }
        // Show info when reopen
        if (!terminateEvDrv) {
            {
                // Dump deviceName
                boost::shared_lock<boost::shared_mutex> devReadLock(
                    this->deviceRwlock);
                portName = this->device.set.deviceName;
            }
            std::cerr << __FILE__ << "+" << __LINE__ << ": 重新打开串口 TRY REOPEN `" <<
                SerialPort::getFullPortName(portName)
                << "' each 2 seconds ...\n";
        }
    maybeerror:
        {
            // Close first if opened
            boost::upgrade_lock<boost::shared_mutex> deviceUplock(
                this->deviceRwlock);
            boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
                deviceUplock);
            this->__doclose();
        }
        {
            // Fetch terminate again
            std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
            terminateEvDrv =  eventDriven->terminate;
        }
        double lastNanoSec = NowNanosec();
        // 进入另一个while 循环 没2s 钟 尝试 打开 一次串口

        while (!terminateEvDrv) {
            // Wait 2 seconds
            if ((NowNanosec() - lastNanoSec) >= 2e9) {
                {
                    // Dump deviceName
                    boost::shared_lock<boost::shared_mutex> devReadLock(
                        this->deviceRwlock);
                    portName = this->device.set.deviceName;
                }
                boost::upgrade_lock<boost::shared_mutex> deviceUplock(
                    this->deviceRwlock);
                boost::upgrade_to_unique_lock<boost::shared_mutex>
                    deviceWriteLock(deviceUplock);
                if (this->__doopen(portName, this->device.openFlags)) {
                    break;
                }
                lastNanoSec = NowNanosec();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            {
                // Fetch terminate again
                std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
                terminateEvDrv =  eventDriven->terminate;
            }
        }
        // Wait 1 seconds
        for (int i = 0; i < 20 && !terminateEvDrv; ++i) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            {
                // Fetch terminate again
                std::lock_guard<std::mutex> lock(this->eventDrivenMutex);
                terminateEvDrv =  eventDriven->terminate;
            }
        }
    }// 线程主循环

    {
        boost::upgrade_lock<boost::shared_mutex> deviceUplock(
            this->deviceRwlock);
        boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
            deviceUplock);
        this->__doclose();
    }
    std::cout << "SerialPort::ioevsloop: exit\n";
}
bool SerialPort::__doopen(std::string const& d, uint32_t const f) noexcept
{
    uint32_t a = O_RDWR | O_NOCTTY;
    this->device.fd = ::open(SerialPort::getFullPortName(d).c_str(), a);
    if (this->device.fd >= 0) {
        std::cout << "VVVVVVVV open successful f:" << f <<std::endl;
        this->__doSetupDevice();
        return true;
    }
    else
    {
        std::cout << "  open fail f:" << f <<std::endl;
    }

    return false;
}
bool SerialPort::__doSetupDevice() noexcept
{
    int const sbret = SetBlock(this->device.fd, false);
    std::cout << "SerialPort::__doopen: SetBlock " << sbret << '\n';
    // Save the old termios
    ::tcgetattr(this->device.fd, &this->device.oldTermios);
    // Make a working copy
    this->device.currentTermios = this->device.oldTermios;
    // Enable raw access
    ::cfmakeraw(&this->device.currentTermios);
    // Set up other port settings
    this->device.currentTermios.c_cflag |= CREAD|CLOCAL;
    this->device.currentTermios.c_lflag &=
        (~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ISIG));
    this->device.currentTermios.c_iflag &=
        (~(INPCK | IGNPAR | PARMRK | ISTRIP | ICRNL | IXANY));
    this->device.currentTermios.c_oflag &= (~OPOST);
    this->device.currentTermios.c_cc[VMIN] = 0;
    // Is a disable character available on this system?
#   ifdef SERIALPORT_POSIXVDISABLE
    // Some systems allow for per-device disable-characters,
    // so get the proper value for the configured device
    long const vdisable = ::fpathconf(this->device.fd, _PC_VDISABLE);
    this->device.currentTermios.c_cc[VINTR] = vdisable;
    this->device.currentTermios.c_cc[VQUIT] = vdisable;
    this->device.currentTermios.c_cc[VSTART] = vdisable;
    this->device.currentTermios.c_cc[VSTOP] = vdisable;
    this->device.currentTermios.c_cc[VSUSP] = vdisable;
#   endif // SERIALPORT_POSIXVDISABLE
    this->device.setDirty = kDirtyALL;
    this->__updatePortSetting();
    this->device.reset = false;
    return true;
}
void SerialPort::__doclose() noexcept
{
    if (this->device.fd >= 0) {
        ::tcdrain(this->device.fd);// flush
        ::close(this->device.fd);
        this->device.fd = -1;
    }
    this->device.reset = true;
}
int SerialPort::open(
    uint32_t const openFlags,
    bool const asyncSend,
    uint32_t const readBufferSz)
{
    boost::upgrade_lock<boost::shared_mutex> deviceUplock(this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
        deviceUplock);
    std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
    if (this->__isOpen()) {
        return EBUSY;
    }
    if (openFlags == serial::kOpenFlagsNotOpen) {
        return -EINVAL;
    }
    //std::cout<<"call __open in : " <<__FUNCTION__<<std::endl;
    return this->__open(openFlags, asyncSend, readBufferSz);
}
bool SerialPort::__isOpen() const noexcept
{
    return this->device.fd >= 0;
}
int SerialPort::__open(
    uint32_t const openFlags,
    bool const asyncSend,
    uint32_t const readBufferSz)
{
    /// NOTE linux 2.6.21 seems to ignore O_NDELAY flag
    int copenFlag = O_NOCTTY | O_NDELAY;
    if (openFlags & serial::kOpenFlagsReadWrite) {
        copenFlag |= O_RDWR;
    } else if (openFlags & serial::kOpenFlagsRead) {
        copenFlag |= O_RDONLY;
    } else if (openFlags & serial::kOpenFlagsWrite) {
        copenFlag |= O_WRONLY;
    }
    if ((openFlags & serial::kOpenFlagsWrite)
        && (openFlags & serial::kOpenFlagsAppend)) {
        copenFlag |= O_APPEND;
    }
    // O_RDWR | O_NOCTTY | O_NDELAY
    if (!this->__doopen(this->device.set.deviceName, copenFlag)) {
        int const e = errno;
        std::cerr << "SerialPort::__open: WARNING: " << __FILE__ << "+"
            << __LINE__ << ": "
            "open `" << this->device.set.deviceName << "' fail "
            << ::strerror(e) << '\n';
        if (e) {
            return -e;
        } else {
            return -EPERM;
        }
    }
    // Flag the port as opened
    this->device.openFlags = openFlags;
    this->__doSetupDevice();
    return this->__start(asyncSend, readBufferSz);
}
int SerialPort::start(bool const asyncSend, uint32_t const readBufferSz)
{
    boost::upgrade_lock<boost::shared_mutex> deviceUplock(this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
        deviceUplock);
    std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
    if (this->device.fd < 0) {
        std::cerr << "SerialPort::start: WARNING: " << __FILE__ << "+"
            << __LINE__ << ": open device now with rw\n";
        // Open
        return this->__open(serial::kOpenFlagsReadWrite, readBufferSz);
    }
    return this->__start(asyncSend, readBufferSz);
}
int SerialPort::__start(bool const asyncSend, uint32_t const readBufferSz)
noexcept
{
    if ((QueryMode::EventDriven != this->queryMode) || this->eventDriven) {
        return 0;
    }
    this->eventDriven.reset(new EventDriven(readBufferSz));
    if (asyncSend && (this->device.openFlags & serial::kOpenFlagsWrite)) {
        this->device.ioevs = POLLIN | POLLOUT;
    } else {
        this->device.ioevs = POLLIN;
        // this->device.ioevs = POLLIN | POLLOUT;
    }
    this->eventDriven->thread.reset(new std::thread(std::bind(
        &SerialPort::ioevsloop, this)));
    return 0;
}
int SerialPort::close() noexcept
{
    std::cout << "SerialPort::close: stop ..\n" << std::flush;
    this->stop();
    std::cout << "SerialPort::close: stop done\n" << std::flush;
    {
        boost::upgrade_lock<boost::shared_mutex> deviceUplock(
            this->deviceRwlock);
        boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
            deviceUplock);
        this->__doclose();
    }
    std::cout << "SerialPort::close: done\n" << std::flush;
    return 0;
}
int SerialPort::stop() noexcept
{
    std::cout << "SerialPort::stop ..\n" << std::flush;
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (QueryMode::EventDriven != this->queryMode) {
            return 0;
        }
    }
    boost::shared_ptr<EventDriven> eventDriven;
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        eventDriven = this->eventDriven;
        if (!eventDriven) {
            std::cerr << "SerialPort::Stop: WARNING: " << __FILE__ << "+"
                << __LINE__ << ": eventDriven already nil\n";
            return ENODEV;
        }
        this->eventDriven = nullptr;
        eventDriven->terminate = true;
    }
    std::cout << "SerialPort::stop: join ..\n" << std::flush;
    eventDriven->thread->join();
    std::cout << "SerialPort::stop: join done\n" << std::flush;
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        eventDriven->thread = nullptr;
        eventDriven->readBuffer.reset();
        eventDriven->readBuffer = nullptr;
    }
    // Release
    eventDriven = nullptr;
    std::cout << "SerialPort::stop: done\n" << std::flush;
    return 0;
}
int64_t SerialPort::shouldRecv(uint32_t const maxAvailable)
{
    // std::vector<uint8_t> buffer;
    // // Recv to eat all when dft
    // return this->recv(buffer, maxAvailable, 1000);
}
int64_t SerialPort::shouldSend()
{
    //std::cout << "sendable\n";
    this->flush();
    return 0;
}
int64_t SerialPort::hasData() const noexcept
{
    boost::shared_lock<boost::shared_mutex> deviceReadLock(this->deviceRwlock);
    std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
    return this->__availableData();
}
int64_t SerialPort::__availableData() const noexcept
{
    if (this->device.fd < 0) {
        return -ENODEV;
    }
    long bytes = 0;
    if (::ioctl(this->device.fd, FIONREAD, &bytes) == -1) {
        this->device.reset = true;
        int const e = errno;
        std::cerr << "SerialPort::__availableData: ERROR: " << __FILE__
            << "+" << __LINE__ << ": ioctl FIONREAD fail " << ::strerror(e)
            << '\n';
        if (e) {
            return -e;
        } else {
            return -1;
        }
    }
    // if (this->eventDriven && this->eventDriven->readBuffer) {
    //     return bytes + this->eventDriven->readBuffer->size();
    // } else {
    //     return bytes;
    // }
    //return bytes;

    if (this->eventDriven && this->eventDriven->readBuffer) {
        return this->eventDriven->readBuffer->size();
    } 
}
int64_t SerialPort::recv(
    std::vector<uint8_t>& buffer,
    uint32_t const maxRecv,
    int32_t const timeoutMillisec) noexcept
{
    if (maxRecv <= 0) {
        std::cout << "return maxrecv error" << std::endl;
        return 0;
    }
    {
        boost::shared_lock<boost::shared_mutex> deviceReadLock(
            this->deviceRwlock);
        if (this->device.fd < 0) {
             std::cout << "return fd error" << std::endl;
            return 0;
        }
    }
    // Check busy
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            if (this->eventDriven->recvMode != RecvMode::Free) {
                 std::cout << "return -EBUSY" << std::endl;
                return -EBUSY;
            }
            // Get
            this->eventDriven->recvMode = RecvMode::User;
        }
    }
    // Recv to maxRecv or till timeout
    uint64_t offset;
    double nownsec, lastnsec;
    int64_t recvbytes, r;
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven && this->eventDriven->readBuffer) {
            if (this->eventDriven->readBuffer->size() > maxRecv) {
                /*
                 * 1 ensure size after copy is maxRecv
                 * 2 be large enough for cpy(caller should ensure first)
                 */
                buffer.resize(maxRecv);
                ::memcpy(
                    buffer.data(),
                    this->eventDriven->readBuffer->data(),
                    maxRecv);
                /* erase cache count: maxRecv */
                this->eventDriven->readBuffer->erase(
                    this->eventDriven->readBuffer->begin(),
                    this->eventDriven->readBuffer->begin() + maxRecv);
                recvbytes = maxRecv;
                goto restore;
            } else {
                buffer = *(this->eventDriven->readBuffer);/* copy out */
                this->eventDriven->readBuffer->clear();/* clear cache */
                offset = buffer.size();
            }
        } else {
            offset = 0;
        }
    }
    buffer.resize(maxRecv);/* ensure size after read more */
    recvbytes = offset;
    nownsec = NowNanosec();
    lastnsec = (timeoutMillisec < 0) ? -1.0 :
        (nownsec + (1e6 * timeoutMillisec));
    while ((recvbytes < static_cast<int64_t>(maxRecv))
        && ((timeoutMillisec < 0) || (nownsec <= lastnsec))) {
        // Read more
        {
            boost::upgrade_lock<boost::shared_mutex> deviceUplock(
                this->deviceRwlock);
            boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
                deviceUplock);
            r = ::read(
                this->device.fd,
                buffer.data() + recvbytes,
                maxRecv - recvbytes);
            if (r < 0) {
                int const e = errno;
                std::cerr << "SerialPort::recv: ERROR: " << __FILE__ << "+"
                    << __LINE__
                    << ": read fail " << ::strerror(e) << "\n";
                if (e) {
                    recvbytes = -e;
                } else {
                    recvbytes = -1;
                }
                goto restore;
            } else if (r > 0) {
                recvbytes += r;
                continue;/* fast read again */
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (timeoutMillisec >= 0) {
            nownsec = NowNanosec();
        }
    }
    if (recvbytes < static_cast<int64_t>(maxRecv)) {
        buffer.resize(recvbytes);/* incase timeout: size should be recvbytes */
        /**caller manage capacity so no shrink_to_fit
        buffer.shrink_to_fit();
        @note if recvbytes != maxRecv => ETIME so not return -ETIME
        recvbytes = -ETIME*/
    }
restore:
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            // Release
            this->eventDriven->recvMode = RecvMode::Free;
        }
    }
    return recvbytes;
}

//////////////////////////////////

int64_t SerialPort::recvDate(std::vector<uint8_t>& buffer,uint32_t const maxRecv) noexcept
{
    if (maxRecv <= 0) 
    {
        return 0;
    }
    {
        boost::shared_lock<boost::shared_mutex> deviceReadLock(
            this->deviceRwlock);
        if (this->device.fd < 0) {
            return 0;
        }
    }
    // Check busy
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            if (this->eventDriven->recvMode != RecvMode::Free) {
                return -EBUSY;
            }
            // Get
            this->eventDriven->recvMode = RecvMode::User;
        }
    }
    // Recv to maxRecv or till timeout
    uint64_t offset;
    double nownsec, lastnsec;
    int64_t recvbytes, r;
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven && this->eventDriven->readBuffer) 
        {
            if (this->eventDriven->readBuffer->size() > maxRecv) 
            {
                /*
                 * 1 ensure size after copy is maxRecv
                 * 2 be large enough for cpy(caller should ensure first)
                 */
                buffer.resize(maxRecv);
                ::memcpy(
                    buffer.data(),
                    this->eventDriven->readBuffer->data(),
                    maxRecv);
                /* erase cache count: maxRecv */
                this->eventDriven->readBuffer->erase(
                    this->eventDriven->readBuffer->begin(),
                    this->eventDriven->readBuffer->begin() + maxRecv);
                recvbytes = maxRecv;
                goto restore;
            } 
            else 
            {  
                buffer = *(this->eventDriven->readBuffer);/* copy out */
                this->eventDriven->readBuffer->clear();/* clear cache */
                offset = buffer.size();
            }
        } 
        else 
        {
            offset = 0;
        }
    }
    buffer.resize(maxRecv);/* ensure size after read more */
    recvbytes = offset;
    // nownsec = NowNanosec();
    // lastnsec = (timeoutMillisec < 0) ? -1.0 :
    //     (nownsec + (1e6 * timeoutMillisec));
    // while ((recvbytes < static_cast<int64_t>(maxRecv))
    //     && ((timeoutMillisec < 0) || (nownsec <= lastnsec))) {
    //     // Read more
    //     {
    //         boost::upgrade_lock<boost::shared_mutex> deviceUplock(
    //             this->deviceRwlock);
    //         boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
    //             deviceUplock);
    //         r = ::read(
    //             this->device.fd,
    //             buffer.data() + recvbytes,
    //             maxRecv - recvbytes);
    //         if (r < 0) {
    //             int const e = errno;
    //             std::cerr << "SerialPort::recv: ERROR: " << __FILE__ << "+"
    //                 << __LINE__
    //                 << ": read fail " << ::strerror(e) << "\n";
    //             if (e) {
    //                 recvbytes = -e;
    //             } else {
    //                 recvbytes = -1;
    //             }
    //             goto restore;
    //         } else if (r > 0) {
    //             recvbytes += r;
    //             continue;/* fast read again */
    //         }
    //     }
    //     std::this_thread::sleep_for(std::chrono::milliseconds(10));
    //     if (timeoutMillisec >= 0) {
    //         nownsec = NowNanosec();
    //     }
    // }
    if (recvbytes < static_cast<int64_t>(maxRecv)) 
    {
        buffer.resize(recvbytes);/* incase timeout: size should be recvbytes */
        /**caller manage capacity so no shrink_to_fit
        buffer.shrink_to_fit();
        @note if recvbytes != maxRecv => ETIME so not return -ETIME
        recvbytes = -ETIME*/
    }
restore:
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) 
        {
            // Release
            this->eventDriven->recvMode = RecvMode::Free;
        }
    }
    //std::cout<<"recvbytes "<<recvbytes<<std::endl;
    return recvbytes;
}



/////////////////////////////////
std::vector<uint8_t> SerialPort::recv(
    uint32_t const maxRecv, int32_t const timeoutMillisec) noexcept
{
    if (maxRecv <= 0) {
        return std::vector<uint8_t>();
    }
    {
        boost::shared_lock<boost::shared_mutex> deviceReadLock(
            this->deviceRwlock);
        if (this->device.fd < 0) {
            return std::vector<uint8_t>();
        }
    }
    // Check busy
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            if (this->eventDriven->recvMode != RecvMode::Free) {
                return std::vector<uint8_t>();
            }
            this->eventDriven->recvMode = RecvMode::User;/* get */
        }
    }
    // Recv to maxRecv or till timeout
    uint32_t offset;
    double nownsec, lastnsec;
    int64_t recvbytes, r;
    std::vector<uint8_t> buffer;
    buffer.reserve(maxRecv);
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven && this->eventDriven->readBuffer) {
            if (this->eventDriven->readBuffer->size() > maxRecv) {
                /*
                 * 1 ensure size after copy is maxRecv
                 * 2 be large enough for cpy(caller should ensure first)
                 */
                buffer.resize(maxRecv);
                memcpy(
                    buffer.data(),
                    this->eventDriven->readBuffer->data(),
                    maxRecv);
                // Erase cache count: maxRecv
                this->eventDriven->readBuffer->erase(
                    this->eventDriven->readBuffer->begin(),
                    this->eventDriven->readBuffer->begin() + maxRecv);
                recvbytes = maxRecv;
                goto restore;
            } else {
                buffer = *(this->eventDriven->readBuffer);/* copy out */
                this->eventDriven->readBuffer->clear();/* clear cache */
                offset = buffer.size();
            }
        } else {
            offset = 0;
        }
    }
    buffer.resize(maxRecv);/* ensure size after read more */
    recvbytes = offset;
    nownsec = NowNanosec();
    lastnsec = (timeoutMillisec < 0) ? -1.0 :
        (nownsec + (1e6 * timeoutMillisec));
    while ((recvbytes < static_cast<int64_t>(maxRecv))
        && ((timeoutMillisec < 0) || (nownsec <= lastnsec))) {
        // Read more
        {
            boost::upgrade_lock<boost::shared_mutex> deviceUplock(
                this->deviceRwlock);
            boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
                deviceUplock);
            r = ::read(
                this->device.fd,
                buffer.data() + recvbytes,
                maxRecv - recvbytes);
            if (r < 0) {
                int const e = errno;
                std::cerr << "SerialPort::recv: ERROR: " << __FILE__ << "+"
                    << __LINE__
                    << ": read fail " << ::strerror(e) << "\n";
                if (e) {
                    recvbytes = -e;
                } else {
                    recvbytes = -1;
                }
                goto restore;
            } else if (r > 0) {
                recvbytes += r;
                continue;/* fast read again */
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        if (timeoutMillisec >= 0) {
            nownsec = NowNanosec();
        }
    }
    if (recvbytes < static_cast<int64_t>(maxRecv)) {
        // Incase timeout: size should be recvbytes
        buffer.resize(recvbytes);
        // Caller manage capacity so no shrink_to_fit
        //buffer.shrink_to_fit();
        recvbytes = -ETIMEDOUT;
    }
restore:
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            this->eventDriven->recvMode = RecvMode::Free;/* release */
        }
    }
    buffer.shrink_to_fit();
    return buffer;
}
int64_t SerialPort::recvAll(std::vector<uint8_t>& buffer) noexcept
{
    // Check busy
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            if (this->eventDriven->recvMode != RecvMode::Free) {
                return -EBUSY;
            }
            this->eventDriven->recvMode = RecvMode::User;/* get */
        }
    }
    int64_t recvbytes;
    // Recv
    {
        boost::upgrade_lock<boost::shared_mutex> deviceUplock(
            this->deviceRwlock);
        boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
            deviceUplock);
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        int64_t const avail = this->__availableData();
        if (avail <= 0) {
            recvbytes = avail;
            goto restore;
        }
        uint32_t offset;
        if (this->eventDriven && this->eventDriven->readBuffer
            && (this->eventDriven->readBuffer->size() > 0)) {
            buffer = *(this->eventDriven->readBuffer);
            this->eventDriven->readBuffer->clear();
            offset = buffer.size();
        } else {
            offset = 0;
        }
        buffer.resize(offset + avail);// pre size max
        int64_t const ret = ::read(
            this->device.fd,
            buffer.data() + offset,
            avail);
        if (ret > 0) {
            buffer.resize(offset + ret);// incase ret < avail
        } else {
            buffer.resize(offset);
        }
        buffer.shrink_to_fit();
        recvbytes = buffer.size();
    }
restore:
    {
        std::lock_guard<std::mutex> eventDrivenLock(this->eventDrivenMutex);
        if (this->eventDriven) {
            this->eventDriven->recvMode = RecvMode::Free;/* release */
        }
    }
    return recvbytes;
}


int64_t SerialPort::send(
    std::vector<uint8_t> const& buffer, uint32_t const each) noexcept
{
    return this->send(buffer.data(), buffer.size(), each);
}
int64_t SerialPort::send(
    void const* const data,
    uint32_t const count,
    uint32_t const each) noexcept
{
    boost::upgrade_lock<boost::shared_mutex> deviceUplock(this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
        deviceUplock);
    if (this->device.fd < 0) 
    {
        return -ENOENT;
    }
    if (!data) 
    {
        return -EINVAL;
    }



    uint8_t const* const buffer = reinterpret_cast<uint8_t const* const>(data);
    uint32_t eachw;
    if (each > count) 
    {
        eachw = count;
    } 
    else 
    {
        eachw = each;
    }



    int64_t ret;
    uint64_t sum = 0;
    //std::cout<< "~~~~~~~~the fd is : "<<this->device.fd<<std::endl;
    while (sum < count) 
    {
        if (eachw <= (count - sum)) 
        {
            ret = ::write(this->device.fd, buffer + sum, eachw);
        } 
        else 
        {
            ret = ::write(this->device.fd, buffer + sum, count - sum);
        }
        if (ret != static_cast<int64_t>(eachw)) 
        {
            int const e = errno;
            std::cerr << "SerialPort::send: ERROR: " << __FILE__ << "+"
                << __LINE__
                << ": write fail " << ::strerror(e) << "fd   "<<this->device.fd <<'\n';
            if (e) 
            {
                return -e;
            }
            else 
            {
                return -1;
            }
        }
        sum += static_cast<uint64_t>(ret);
    }
    return sum;
}


int SerialPort::flush() noexcept
{
    boost::upgrade_lock<boost::shared_mutex> deviceUplock(this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
        deviceUplock);
    return this->__flush();
}
int SerialPort::__flush() noexcept
{
    if (this->device.fd < 0) {
        return -ENODEV;
    }
    //::fsync(this->device.fd);
    return ::tcdrain(this->device.fd);
}
void SerialPort::updatePortSettings()
{
    boost::upgrade_lock<boost::shared_mutex> deviceUplock(this->deviceRwlock);
    boost::upgrade_to_unique_lock<boost::shared_mutex> deviceWriteLock(
        deviceUplock);
    if ((!this->__isOpen()) || (!this->device.setDirty)) {
        return;
    } else {
        return this->__updatePortSetting();
    }
}
void SerialPort::__updatePortSetting()
{
    if (this->device.setDirty & kDirtyBaudRate) {
        switch (this->device.set.baudRate) {
        case serial::kBaudRate50:
            baudRateToTermios(B50, this->device.currentTermios);
            break;
        case serial::kBaudRate75:
            baudRateToTermios(B75, this->device.currentTermios);
            break;
        case serial::kBaudRate110:
            baudRateToTermios(B110, this->device.currentTermios);
            break;
        case serial::kBaudRate134:
            baudRateToTermios(B134, this->device.currentTermios);
            break;
        case serial::kBaudRate150:
            baudRateToTermios(B150, this->device.currentTermios);
            break;
        case serial::kBaudRate200:
            baudRateToTermios(B200, this->device.currentTermios);
            break;
        case serial::kBaudRate300:
            baudRateToTermios(B300, this->device.currentTermios);
            break;
        case serial::kBaudRate600:
            baudRateToTermios(B600, this->device.currentTermios);
            break;
        case serial::kBaudRate1200:
            baudRateToTermios(B1200, this->device.currentTermios);
            break;
        case serial::kBaudRate1800:
            baudRateToTermios(B1800, this->device.currentTermios);
            break;
        case serial::kBaudRate2400:
            baudRateToTermios(B2400, this->device.currentTermios);
            break;
        case serial::kBaudRate4800:
            baudRateToTermios(B4800, this->device.currentTermios);
            break;
        case serial::kBaudRate9600:
            baudRateToTermios(B9600, this->device.currentTermios);
            break;
        case serial::kBaudRate19200:
            baudRateToTermios(B19200, this->device.currentTermios);
            break;
        case serial::kBaudRate38400:
            baudRateToTermios(B38400, this->device.currentTermios);
            break;
        case serial::kBaudRate57600:
            baudRateToTermios(B57600, this->device.currentTermios);
            break;
#       if defined(SERIALPORT_B76800) && (SERIALPORT_B76800)
        case serial::kBaudRate76800:
            baudRateToTermios(B76800, this->device.currentTermios);
            break;
#       endif
        case serial::kBaudRate115200:
            baudRateToTermios(B115200, this->device.currentTermios);
            break;
#       if defined(SERIALPORT_B230400) \
            && defined(SERIALPORT_B4000000)
        case serial::kBaudRate230400:
            baudRateToTermios(B230400, this->device.currentTermios);
            break;
        case serial::kBaudRate460800:
            baudRateToTermios(B460800, this->device.currentTermios);
            break;
        case serial::kBaudRate500000:
            baudRateToTermios(B500000, this->device.currentTermios);
            break;
        case serial::kBaudRate576000:
            baudRateToTermios(B576000, this->device.currentTermios);
            break;
        case serial::kBaudRate921600:
            baudRateToTermios(B921600, this->device.currentTermios);
            break;
        case serial::kBaudRate1000000:
            baudRateToTermios(B1000000, this->device.currentTermios);
            break;
        case serial::kBaudRate1152000:
            baudRateToTermios(B1152000, this->device.currentTermios);
            break;
        case serial::kBaudRate1500000:
            baudRateToTermios(B1500000, this->device.currentTermios);
            break;
        case serial::kBaudRate2000000:
            baudRateToTermios(B2000000, this->device.currentTermios);
            break;
        case serial::kBaudRate2500000:
            baudRateToTermios(B2500000, this->device.currentTermios);
            break;
        case serial::kBaudRate3000000:
            baudRateToTermios(B3000000, this->device.currentTermios);
            break;
        case serial::kBaudRate3500000:
            baudRateToTermios(B3500000, this->device.currentTermios);
            break;
        case serial::kBaudRate4000000:
            baudRateToTermios(B4000000, this->device.currentTermios);
            break;
#       endif
        default:
            std::cerr << "SerialPort::__updatePortSetting: WARNING: "
                << __FILE__ << "+" << __LINE__
                << ": bad baudRate no update "
                << this->device.set.baudRate << '\n';
            break;
        }
    }
    if (this->device.setDirty & kDirtyParity) {
        switch (this->device.set.parityType) {
        case ParityType::SPACE:
            /*
             * space parity not directly supported - add an extra data bits to
             * simulate it
             */
            this->device.setDirty |= kDirtyDataBits;
            break;
        case ParityType::NONE:
            this->device.currentTermios.c_cflag &= (~PARENB);
            break;
        case ParityType::EVEN: {
            this->device.currentTermios.c_cflag &= (~PARODD);
            this->device.currentTermios.c_cflag |= PARENB;
        } break;
        case ParityType::ODD:
            this->device.currentTermios.c_cflag |= (PARENB|PARODD);
            break;
        }
    }
    // Must after parity settings
    if (this->device.setDirty & kDirtyDataBits) {
        if (this->device.set.parityType != ParityType::SPACE) {
            this->device.currentTermios.c_cflag &= (~CSIZE);
            switch(this->device.set.dataBits) {
            case serial::kDataBits5:
                this->device.currentTermios.c_cflag |= CS5;
                break;
            case serial::kDataBits6:
                this->device.currentTermios.c_cflag |= CS6;
                break;
            case serial::kDataBits7:
                this->device.currentTermios.c_cflag |= CS7;
                break;
            case serial::kDataBits8:
                this->device.currentTermios.c_cflag |= CS8;
                break;
            default: throw std::logic_error("invalid data bits"); break;
            }
        } else {
            /*
             * space parity not directly supported - add an extra data bit to
             * simulate it
             */
            this->device.currentTermios.c_cflag &= ~(PARENB|CSIZE);
            switch(this->device.set.dataBits) {
            case serial::kDataBits5:
                this->device.currentTermios.c_cflag |= CS6;
                break;
            case serial::kDataBits6:
                this->device.currentTermios.c_cflag |= CS7;
                break;
            case serial::kDataBits7:
                this->device.currentTermios.c_cflag |= CS8;
                break;
            case serial::kDataBits8:
                // This will never happen, put here to suppress an warning
                std::cerr << "SerialPort::__updatePortSetting: WARNING: "
                    << __FILE__ << "+"
                    << __LINE__ << ": kDataBits8 no update\n";
                break;
            default: throw std::logic_error("invalid data bits"); break;
            }
        }
    }
    if (this->device.setDirty & kDirtyStopBits) {
        switch (this->device.set.stopBits) {
        case serial::kStopBits1:
            this->device.currentTermios.c_cflag &= (~CSTOPB);
            break;
        case serial::kStopBits2:
            this->device.currentTermios.c_cflag |= CSTOPB;
            break;
        default: throw std::logic_error("invalid stop bits"); break;
        }
    }
    if (this->device.setDirty & kDirtyFlow) {
        switch(this->device.set.flowControl) {
        case FlowControl::OFF:
            this->device.currentTermios.c_cflag &= (~CRTSCTS);
            this->device.currentTermios.c_iflag &= (~(IXON|IXOFF|IXANY));
            break;
        case FlowControl::XONXOFF:
            /* software (XON/XOFF) flow control */
            this->device.currentTermios.c_cflag &= (~CRTSCTS);
            this->device.currentTermios.c_iflag |= (IXON|IXOFF|IXANY);
            break;
        case FlowControl::HARDWARE:
            this->device.currentTermios.c_cflag |= CRTSCTS;
            this->device.currentTermios.c_iflag &= (~(IXON|IXOFF|IXANY));
            break;
        }
    }
    // If any thing in currentTermios changed, flush
    if (this->device.setDirty & kDirtySettingMask) {
        ::tcsetattr(this->device.fd, TCSAFLUSH, &this->device.currentTermios);
    }
    if (this->device.setDirty & kDirtyTimeOut) {
        long const millisec = this->device.set.timeoutMillisec;
        if (millisec == -1) {
            ::fcntl(this->device.fd, F_SETFL, O_NDELAY);
        } else {
            /*
             * O_SYNC should enable blocking ::write()
             * however this seems not working on Linux 2.6.21
             * (works on OpenBSD 4.2)
             */
            ::fcntl(this->device.fd, F_SETFL, O_SYNC);
        }
        ::tcgetattr(this->device.fd, &this->device.currentTermios);
        this->device.currentTermios.c_cc[VTIME] = millisec / 100;
        ::tcsetattr(this->device.fd, TCSAFLUSH, &this->device.currentTermios);
    }
    this->device.setDirty = 0;
}
std::string SerialPort::getFullPortName(std::string const& portName) noexcept
{
    char const* const p = strstr(portName.c_str(), "/");
    if (p && (p == portName.c_str())) {
        return portName;
    } else {
        return "/dev/" + portName;
    }
}
struct termios& SerialPort::baudRateToTermios(
    uint32_t const& baudRate,
    struct termios& ter) noexcept
{
#   ifdef CBAUD
    ter.c_cflag &= (~CBAUD);
    ter.c_cflag |= baudRate;
#   else
    ::cfsetispeed(&ter, baudRate);
    ::cfsetospeed(&ter, baudRate);
#   endif
    return ter;
}
}
