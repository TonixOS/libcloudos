project(libcloudos)
cmake_minimum_required(VERSION 2.8)
set( CMAKE_VERBOSE_MAKEFILE on )
set( CMAKE_CXX_FLAGS -std=c++0x )
#  set( CMAKE_CXX_FLAGS -fsanitize=address )
set( CMAKE_BUILD_TYPE debug )

include_directories ("${PROJECT_SOURCE_DIR}")

add_library(cloudos SHARED    cloudos/ui/Dialog.cpp
                              cloudos/ui/DialogWelcome.cpp
                              cloudos/ui/DialogLanguage.cpp
                              cloudos/ui/DialogModuleSelection.cpp
                              cloudos/ui/DialogNetwork.cpp
                              cloudos/ui/DialogNeutronServerMain.cpp
                              cloudos/ui/DialogStorage.cpp
                              cloudos/tools/System.cpp
                              cloudos/tools/IPAddress.cpp
                              cloudos/tools/NetworkInterface.cpp
                              cloudos/tools/NetworkRoute.cpp
                              cloudos/tools/StorageLocal.cpp
                              cloudos/proto/OS.System.pb.cc
                              cloudos/proto/OS.Network.pb.cc
                              cloudos/proto/OS.Installer.pb.cc
			      cloudos/proto/OpenStack.CinderServer.pb.cc
			      cloudos/proto/OpenStack.GlanceServer.pb.cc
			      cloudos/proto/OpenStack.KeystoneClient.pb.cc
			      cloudos/proto/OpenStack.KeystoneServer.pb.cc
			      cloudos/proto/OpenStack.NeutronServer.pb.cc
			      cloudos/proto/OpenStack.NovaMetadataClient.pb.cc
			      cloudos/proto/Package.MariadbClient.pb.cc
			      cloudos/proto/Package.RabbitmqClient.pb.cc
			      cloudos/proto/VM.ManagementServer.pb.cc
			      )
target_link_libraries(cloudos yui
                              boost_system
                              boost_filesystem
                              protobuf
                              pthread
			      parted
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
              DESTINATION include/cloudos/ui/)

install(FILES cloudos/tools/System.hpp
              cloudos/tools/IPAddress.hpp
              cloudos/tools/NetworkElement.hpp
              cloudos/tools/NetworkInterface.hpp
              cloudos/tools/NetworkRoute.hpp
              cloudos/tools/StorageLocal.hpp
              DESTINATION include/cloudos/tools/)

install(FILES cloudos/proto/OS.System.pb.h
              cloudos/proto/OS.Network.pb.h
              cloudos/proto/OS.Installer.pb.h
	      cloudos/proto/OpenStack.CinderServer.pb.h
	      cloudos/proto/OpenStack.GlanceServer.pb.h
	      cloudos/proto/OpenStack.KeystoneClient.pb.h
	      cloudos/proto/OpenStack.KeystoneServer.pb.h
	      cloudos/proto/OpenStack.NeutronServer.pb.h
	      cloudos/proto/OpenStack.NovaMetadataClient.pb.h
	      cloudos/proto/Package.MariadbClient.pb.h
	      cloudos/proto/Package.RabbitmqClient.pb.h
	      cloudos/proto/VM.ManagementServer.pb.h
              DESTINATION include/cloudos/proto/)
