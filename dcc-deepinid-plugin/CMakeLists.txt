set(PLUGIN_NAME "dcc-deepinid-plugin")

# 启用 qt moc 的支持
set(CMAKE_AUTOMOC ON)
# 启用 qrc 资源文件的支持
set(CMAKE_AUTORCC ON)
# 启用当前源目录和构建目录添加到包含路径
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Install settings
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

include(GNUInstallDirs)

# 指定所有源码文件
file(GLOB_RECURSE SRCS
    "*.h"
    "*.cpp"
    "*.qrc"
    "*.svg"
    "*.html"
)

# 指定所有qrc文件
set(QRCS icons/deepinid.qrc)

# 指定用到库
#find_package(QT NAMES Qt5 REQUIRED COMPONENTS Core)
find_package(Qt5 REQUIRED COMPONENTS Core Concurrent Network Widgets DBus Svg LinguistTools WebChannel WebEngine WebEngineCore WebEngineWidgets)
find_package(DdeControlCenter REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Dtk REQUIRED COMPONENTS Widget)
find_package(PolkitQt5-1)
find_package(OpenSSL REQUIRED)

# 新增一个编译目标
file(GLOB TS_FILES "../translations/*.ts")

message(STATUS "status:${QT_NO_CREATE_VERSIONLESS_FUNCTIONS}")
QT5_ADD_TRANSLATION(QM_FILES ${TS_FILES})

add_custom_target(translations ALL DEPENDS ${QM_FILES})

# 这里使用命令 add_library 来表示本项目要生成一个库文件目标，
add_library(${PLUGIN_NAME} SHARED ${SRCS} ${QRCS})

# 设置目标要使用的 include 目录，即头文件目录
# 变量 ${DtkWidget_INCLUDE_DIRS} 是在前面执行 find_package 命令时引入的
# 当出现编译失败提示找不到某些库的头文件时应该检查此处是否将所有需要的头文件都包含了
target_include_directories(${PLUGIN_NAME} PUBLIC
    ${DdeControlCenter_INCLUDE_DIR}
)

# 设置目标要使用的链接库
# 变量 ${DtkWidget_LIBRARIES} 和 ${Qt5Widgets_LIBRARIES} 是在前面执行 find_package 命令时引入的
# 当出现运行时错误提示某些符号没有定义时应该检查此处是否将所有用的库都写在了这里
target_link_libraries(${PLUGIN_NAME} PRIVATE
    Qt5::Widgets
    Qt5::DBus
    Qt5::Svg
    Qt5::Network
    Qt5::Concurrent
    Qt5::WebChannel
    Qt5::WebEngineWidgets
    Dtk::Widget
    ${DdeControlCenter_LIBRARIES}
    PolkitQt5-1::Agent
    libcrypto.so
)

# 设置安装路径的前缀(默认为"/usr/local")

set(UOSID_LOGO ${PROJECT_SOURCE_DIR}/icons/icons/uos-id.svg)
# 指定安装位置
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}/dde-control-center/modules)
# 安装图标文件
#if(EXISTS "/usr/share/icons/hicolor/scalable/apps/uos-id.svg")
#    message(STATUS "app logo exists")
#else()
#    message(STATUS "app logo not exists")
#    # 安装图标文件
#    install(FILES ${UOSID_LOGO} DESTINATION share/icons/hicolor/scalable/apps)
#endif()

# 安装 .qm 文件
install(FILES ${QM_FILES} DESTINATION ${CMAKE_INSTALL_DATADIR}/dde-control-center/translations)
