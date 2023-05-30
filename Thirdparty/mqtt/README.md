
Mqtt: 

源码链接:https://github.com/eclipse/paho.mqtt.c  
版本: Release Version 1.3.8  

编译选项修改:
```
## build options
SET(PAHO_WITH_SSL FALSE CACHE BOOL "Flag that defines whether to build ssl-enabled binaries too. ")
SET(PAHO_BUILD_SHARED TRUE CACHE BOOL "Build shared library")
SET(PAHO_BUILD_STATIC FALSE CACHE BOOL "Build static library")
SET(PAHO_BUILD_DOCUMENTATION FALSE CACHE BOOL "Create and install the HTML based API documentation (requires Doxygen)")
SET(PAHO_BUILD_SAMPLES FALSE CACHE BOOL "Build sample programs")
SET(PAHO_BUILD_DEB_PACKAGE FALSE CACHE BOOL "Build debian package")
SET(PAHO_ENABLE_TESTING FALSE CACHE BOOL "Build tests and run")
SET(PAHO_ENABLE_CPACK FALSE CACHE BOOL "Enable CPack")
```
