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
#pragma once
#include <stdint.h> /* uint8_t .. */
#include <termios.h> /* termios */
#include <string> /* string */
#include <vector>
#include <mutex>
#include <thread>
#include "boost/thread/shared_mutex.hpp"
namespace ctirobot
{
#if defined(B76800) && (!defined(SERIALPORT_B76800))
#define SERIALPORT_B76800 1
#endif
#ifndef SERIALPORT_B76800
#define SERIALPORT_B76800 0
#endif

#if defined(B230400) && (!defined(SERIALPORT_B230400))
#define SERIALPORT_B230400 1
#endif
#if defined(B4000000) && (!defined(SERIALPORT_B4000000))
#define SERIALPORT_B4000000 1
#endif

#if (!defined(SERIALPORT_B230400)) && (!defined(SERIALPORT_B4000000))
#define SERIALPORT_B230400 1
#define SERIALPORT_B4000000 1
#endif

#if defined(_POSIX_VDISABLE) && (!defined(SERIALPORT_POSIXVDISABLE))
#define SERIALPORT_POSIXVDISABLE _POSIX_VDISABLE
#endif
#ifndef SERIALPORT_POSIXVDISABLE
#   define SERIALPORT_POSIXVDISABLE '\0'
#endif

namespace serial
{
constexpr uint32_t kDefaultReadBufferSize = sizeof(long) * 1024;
constexpr int kBaudRate50      = 50;/* posix only */
constexpr int kBaudRate75      = 75;/* posix only */
constexpr int kBaudRate134     = 134;/* posix only */
constexpr int kBaudRate150     = 150;/* posix only */
constexpr int kBaudRate200     = 200;/* posix only */
constexpr int kBaudRate1800    = 1800;/* posix only */
#if defined(SERIALPORT_B76800) && (SERIALPORT_B76800)
constexpr int kBaudRate76800   = 76800;/* posix only */
#endif
#if defined(SERIALPORT_B230400) && defined(SERIALPORT_B4000000)
constexpr int kBaudRate230400  = 230400;/* posix only */
constexpr int kBaudRate460800  = 460800;/* posix only */
constexpr int kBaudRate500000  = 500000;/* posix only */
constexpr int kBaudRate576000  = 576000;/* posix only */
constexpr int kBaudRate921600  = 921600;/* posix only */
constexpr int kBaudRate1000000 = 1000000;/* posix only */
constexpr int kBaudRate1152000 = 1152000;/* posix only */
constexpr int kBaudRate1500000 = 1500000;/* posix only */
constexpr int kBaudRate2000000 = 2000000;/* posix only */
constexpr int kBaudRate2500000 = 2500000;/* posix only */
constexpr int kBaudRate3000000 = 3000000;/* posix only */
constexpr int kBaudRate3500000 = 3500000;/* posix only */
constexpr int kBaudRate4000000 = 4000000;/* posix only */
#endif
constexpr int kBaudRate110     = 110;
constexpr int kBaudRate300     = 300;
constexpr int kBaudRate600     = 600;
constexpr int kBaudRate1200    = 1200;
constexpr int kBaudRate2400    = 2400;
constexpr int kBaudRate4800    = 4800;
constexpr int kBaudRate9600    = 9600;
constexpr int kBaudRate19200   = 19200;
constexpr int kBaudRate38400   = 38400;
constexpr int kBaudRate57600   = 57600;
constexpr int kBaudRate115200  = 115200;
constexpr int kDataBits5 = 5;
constexpr int kDataBits6 = 6;
constexpr int kDataBits7 = 7;
constexpr int kDataBits8 = 8;
constexpr int kStopBits1 = 1;
constexpr int kStopBits2 = 2;
constexpr uint32_t kOpenFlagsNotOpen   = 0x0000;
constexpr uint32_t kOpenFlagsRead      = 0x0004;
constexpr uint32_t kOpenFlagsWrite     = 0x0002;
constexpr uint32_t kOpenFlagsReadWrite = kOpenFlagsRead | kOpenFlagsWrite;
constexpr uint32_t kOpenFlagsAppend    = 0x0008;
constexpr uint32_t kOpenFlagsTruncate  = 0x0010;
constexpr uint32_t kOpenFlagsText      = 0x0020;
}
enum class ParityType: int {
    NONE,
    ODD,
    EVEN,
    SPACE
};
enum class FlowControl: int {
    OFF,
    HARDWARE,
    XONXOFF
};
/// Structure contains port settings
struct PortSetting {
    std::string deviceName{ "" };
    //
    int baudRate;
    int dataBits;
    ParityType parityType;
    int stopBits;
    FlowControl flowControl;
    long timeoutMillisec;
};
struct SerialPort {
    enum class QueryMode {
        Polling,
        EventDriven
    };
    /**
     * Ctor / other params use default
     * @param queryMode QueryMode const& default EventDriven
     */
    SerialPort(QueryMode const& queryMode = QueryMode::EventDriven) noexcept;
    /**
     * Ctor / other params use default
     * @param portName std::string const&: serial port name
     * @param queryMode QueryMode const& default EventDriven
     */
    SerialPort(std::string const& portName,QueryMode const& queryMode = QueryMode::EventDriven) noexcept;
    SerialPort(PortSetting const& portSetting,QueryMode const& mode = QueryMode::EventDriven) noexcept;
    /// Close device and release resources
    ~SerialPort() noexcept;
    inline QueryMode const& getQueryMode() const { return this->queryMode; }
    std::string getPortName() const noexcept;
    int getBaudRate() const noexcept;
    bool isOpen() const noexcept;
    void setPortName(std::string const& portName) noexcept;
    /// @throw std::logic_error when @a baudRate invalid
    void setBaudRate(int const baudRate, bool const update = true);
    /// Set callback rate for read when EventDriven
    bool setRate(double const rate) noexcept;
    /**
     * Opens a serial port and sets its OpenFlag to @a openFlag
     * (if not NotOpen)
     * @param openFlags default serial::kOpenFlagsReadWrite
     * @param asyncSend defualt false
     * @param readBufferSz when EventDriven and @a readBufferSz > 0 read
     * buffer will be alloced
     * @note that this function does not specify which device to open
     * @return
     * - return 0 if no any error
     * - return <0 (-errno) when fail
     * - return >0 (errno) when alreay done
     * @details
     * - this function has no effect if the port associated with the class is
     *   already open and return is EBUSY
     * - the port is also configured to the current settings,
     *   as stored in the settings structure
     * - if do open start will be called
     * @throw std::logic_error when port settings invalid
     */
    int open(
        uint32_t const openFlags = serial::kOpenFlagsReadWrite,
        bool const asyncSend = false,
        uint32_t const readBufferSz = serial::kDefaultReadBufferSize);
    /**
     * Start event loop when EventDriven also open if not
     * @throw std::logic_error when settings invalid
     */
    int start(
        bool const asyncSend = false,
        uint32_t const readBufferSz = serial::kDefaultReadBufferSize);
    /// Stop (if started) and close
    int close() noexcept;
    int stop() noexcept;
    /**
     * Override to read data when EventDriven
     * e.x. use recv or recvAll
     * @note
     * - if rate enabled shouldRecv be callbacked when cache full or
     * no cache or time to rate.
     * - if rate disabled shouldRecv be callbacked once data available
     * @return >= 0 when success else fail and will not callback shouldSend
     */
    virtual int64_t shouldRecv(uint32_t const maxAvailable);
    /**
     * Override to send to fd when just fd writeable
     * when EventDriven e.x. use send to send
     */
    virtual int64_t shouldSend();
    /// @return available data bytes when success(>= 0), -errno when fail
    int64_t hasData() const noexcept;
    /**
     * Recv max bytes data or till @a timeoutMillisec or error
     * @param buffer std::vector<uint8_t>&: should has min @a maxRecv capacity
     * (e.x. reserve) but not required .. here will resize
     * @param[out] buffer receive to this buffer
     * @param maxRecv max recv in bytes
     * @param timeoutMillisec long const: if is 0: read once till ok or fail
     * @return
     * - full success return received bytes and return equals maxRecv
     * - else if return > 0 but < maxRecv means time out
     * - return -errno when fail
     */
    int64_t recv(
        std::vector<uint8_t>& buffer,
        uint32_t const maxRecv = serial::kDefaultReadBufferSize * 2,
        int32_t const timeoutMillisec = -1) noexcept;
    /// @overload recv but not need buffer parameter
    std::vector<uint8_t> recv(
        uint32_t const maxRecv = serial::kDefaultReadBufferSize * 2,
        int32_t const timeoutMillisec = -1) noexcept;
    int64_t recvDate(std::vector<uint8_t>& buffer,uint32_t const maxRecv) noexcept;
    /**
     * Recv all data in cache and device driver buffer to @a buffer
     * @param[out] buffer read buffer, not need resize / .. NOTE NOT append
     * @return received bytes (>=0) when success else -errno
     */
    int64_t recvAll(std::vector<uint8_t>& buffer) noexcept;
    int64_t send(
        std::vector<uint8_t> const& buffer,
        uint32_t const each = UINT32_MAX) noexcept;
    int64_t send(
        void const* const data,
        uint32_t const count,
        uint32_t const each = UINT32_MAX) noexcept;
    int flush() noexcept;
    /// @throw std::logic_error when settings invalid
    void updatePortSettings();
    static std::string getFullPortName(std::string const& portName) noexcept;
    static struct termios& baudRateToTermios(
        uint32_t const& baudRate,
        struct termios& ter) noexcept;
private:
    static constexpr uint32_t kDirtyBaudRate    = 0x0001;
    static constexpr uint32_t kDirtyParity      = 0x0002;
    static constexpr uint32_t kDirtyStopBits    = 0x0004;
    static constexpr uint32_t kDirtyDataBits    = 0x0008;
    static constexpr uint32_t kDirtyFlow        = 0x0010;
    static constexpr uint32_t kDirtyTimeOut     = 0x0100;
    static constexpr uint32_t kDirtyALL         = 0x0fff;
    static constexpr uint32_t kDirtySettingMask = 0x00ff;///< without TimeOut
    enum class RecvMode {
        Free,
        User,
    };
    struct EventDriven {
        bool terminate{ false };
        double rate{ 0.0 };///< If 0.0 as fast as possible
        double lastCallbackRecv{ -1.0 };///< nsec if < 0 not need update
        RecvMode recvMode{ RecvMode::Free };
        boost::shared_ptr<std::thread> thread{ nullptr };
        boost::shared_ptr<std::vector<uint8_t> > readBuffer;
        EventDriven(uint32_t const readBufferSz):
            readBuffer((readBufferSz <= 0) ? nullptr :
                new std::vector<uint8_t>(readBufferSz, 0)) {
            if (this->readBuffer) {
                this->readBuffer->reserve(readBufferSz);
                this->readBuffer->resize(0);
            }
        }
    };
    void handleIoEvs(short const ioevs) noexcept;
    void ioevsloop() noexcept;
    /// Required locks: device read lock
    bool __isOpen() const noexcept;
    /**
     * Do real open no MT lock
     * @note required locks:
     * - device write lock
     * - eventDriven mutex
     * - portSetting read lock
     * @throw std::logic_error when settings invalid
     */
    int __open(
        uint32_t const openFlags = serial::kOpenFlagsReadWrite,
        bool const asyncSend = false,
        uint32_t const readBufferSz = serial::kDefaultReadBufferSize);
    /// Pair { __doopen + __doclose }
    bool __doopen(std::string const& d, uint32_t const f) noexcept;
    /// __doopen -> __doSetupDevice
    bool __doSetupDevice() noexcept;
    /// Pair { __doopen + __doclose }
    void __doclose() noexcept;
    /**
     * Real start
     * @note required locks: device write lock + eventDriven mutex
     */
    int __start(
        bool const asyncSend = false,
        uint32_t const readBufferSz = serial::kDefaultReadBufferSize) noexcept;
    /**
     * @note required locks: device read lock + eventDriven lock
     * @return available data bytes when success(>= 0), -errno when fail
     */
    int64_t __availableData() const noexcept;
    /**
     * Required locks: device lock
     * @return
     * - 0  on success.
     * - -1 on failure and set errno to indicate the error.
     */
    int __flush() noexcept;
    /**
     * @note required locks: device write lock and portSetting write lock
     * @throw std::logic_error when settings invalid
     */
    void __updatePortSetting();
    /// deviceRwlock{ eventDrivenMutex }
    mutable boost::shared_mutex deviceRwlock;
    struct Device {
        int fd{ -1 };///< serial device fd
        mutable bool reset{ false };
        uint32_t openFlags{ 0 };
        short ioevs;
        struct termios oldTermios;
        struct termios currentTermios;
        std::vector<uint8_t> buffer;
        PortSetting set;
        uint32_t setDirty{ 0 };///< for set
        Device() {}
        ~Device() {}
    } device;
    mutable std::mutex eventDrivenMutex;
    boost::shared_ptr<EventDriven> eventDriven{ nullptr };
    QueryMode const queryMode;///< ev drive or poll
};
}
