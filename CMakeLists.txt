project(libcloudos)
cmake_minimum_required(VERSION 2.8)
set( CMAKE_VERBOSE_MAKEFILE on )
#set( CMAKE_CXX_FLAGS "-fsanitize=address -std=c++11" )
set( CMAKE_CXX_FLAGS "-std=c++11" )
set( CMAKE_BUILD_TYPE debug )
add_definitions(-DBOOST_NO_DEPRECATED)
add_definitions(-DBOOST_NO_CXX11_SCOPED_ENUMS)
#add_definitions(-Weverything)
#add_definitions(-Wno-c++98-compat)
#add_definitions(-Wno-padded)

# we need this flag, else all linking tools will fail to link...
# if not defined, some thing like this will be shown:
#   libcloudos.so: undefined reference to `boost::log::v2s_mt_posix::aux::once_block_sentry::enter_once_block() const'
add_definitions(-DBOOST_ALL_DYN_LINK)


#
# L I B  -  P A C K A G E S
#
find_package(Protobuf REQUIRED)
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories ("${PROJECT_SOURCE_DIR}")

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS proto/OS_System.proto
                                            proto/OS_Network.proto
                                            proto/OS_Installer.proto
                                            proto/OS_HW_HDDisk.proto
                                            proto/OpenStack_CinderServer.proto
                                            proto/OpenStack_GlanceServer.proto
                                            proto/OpenStack_KeystoneClient.proto
                                            proto/OpenStack_KeystoneServer.proto
                                            proto/OpenStack_NeutronServer.proto
                                            proto/OpenStack_NovaMetadataClient.proto
                                            proto/Package_MariadbClient.proto
                                            proto/Package_RabbitmqClient.proto
                                            proto/VM_ManagementServer.proto
                                            )


add_library(cloudos SHARED    cloudos/ui/Dialog.cpp
                              cloudos/ui/DialogWelcome.cpp
                              cloudos/ui/DialogLanguage.cpp
                              cloudos/ui/DialogModuleSelection.cpp
                              cloudos/ui/DialogNetwork.cpp
                              cloudos/ui/DialogNeutronServerMain.cpp
                              cloudos/ui/DialogStorage.cpp
                              cloudos/ui/DialogInstallerProcess.cpp
                              cloudos/ui/DialogInstallerFinished.cpp
                              cloudos/tools/System.cpp
                              cloudos/tools/IPAddress.cpp
                              cloudos/tools/NetworkInterface.cpp
                              cloudos/tools/NetworkRoute.cpp
			      cloudos/system/File.cpp
			      cloudos/system/FileTar.cpp
			      cloudos/system/Command.cpp
			      cloudos/system/ChRootController.cpp
			      cloudos/system/CommandCollectionExecutor.cpp
			      cloudos/system/Directory.cpp
			      cloudos/system/HDDisk.cpp
			      cloudos/system/Installer.cpp
			      cloudos/system/InstallerExtended.cpp
			      cloudos/system/InstallerCoreSystem.cpp
			      cloudos/system/InstallerHostSystem.cpp
			      cloudos/system/InstallerManagementSystem.cpp
			      cloudos/system/StepExecutor.cpp
			      cloudos/core/Object.cpp
			      cloudos/core/IndexName.cpp
			      cloudos/core/IndexedCommandQueue.cpp
			      ${PROTO_SRCS}
			      )
target_link_libraries(cloudos yui
                              boost_system
                              boost_filesystem
                              boost_log
                              boost_system
                              pthread
			      parted
			      ${PROTOBUF_LIBRARIES}
			      )
SET_TARGET_PROPERTIES(cloudos PROPERTIES LINKER_LANGUAGE CXX)

install(TARGETS cloudos LIBRARY DESTINATION lib)

# install our header files
install(FILES cloudos/ui/Dialog.hpp
              cloudos/ui/DialogLanguage.hpp 
              cloudos/ui/DialogWelcome.hpp
              cloudos/ui/DialogModuleSelection.hpp
              cloudos/ui/DialogNetwork.hpp
              cloudos/ui/DialogNeutronServerMain.hpp
              cloudos/ui/DialogStorage.hpp
              cloudos/ui/DialogInstallerProcess.hpp
              cloudos/ui/DialogInstallerFinished.hpp
              DESTINATION include/cloudos/ui/)

install(FILES cloudos/tools/System.hpp
              cloudos/tools/IPAddress.hpp
              cloudos/tools/NetworkElement.hpp
              cloudos/tools/NetworkInterface.hpp
              cloudos/tools/NetworkRoute.hpp
              DESTINATION include/cloudos/tools/)

install(FILES ${PROTO_HDRS}
              DESTINATION include/cloudos/proto/)

install(FILES cloudos/system/File.hpp
              cloudos/system/FileTar.hpp
              cloudos/system/Command.hpp
              cloudos/system/Directory.hpp
              cloudos/system/HDDisk.hpp
              cloudos/system/ChRootController.hpp
              cloudos/system/CommandCollectionExecutor.hpp
	      cloudos/system/Installer.hpp
	      cloudos/system/InstallerExtended.hpp
	      cloudos/system/InstallerCoreSystem.hpp
	      cloudos/system/InstallerHostSystem.hpp
	      cloudos/system/InstallerManagementSystem.hpp
	      cloudos/system/StepExecutor.hpp
              DESTINATION include/cloudos/system/)

install(FILES cloudos/core/Object.hpp
              cloudos/core/Config.hpp
              cloudos/core/ConfigVector.hpp
              cloudos/core/logging.hpp
              cloudos/core/IndexedCommandQueue.hpp
              cloudos/core/IndexName.hpp
              cloudos/core/ShareAble.hpp
              DESTINATION include/cloudos/core/)
