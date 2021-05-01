#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

PROJECT_NAME := ESP32_IR_BLASTER

#COMPONENT_SRCDIRS = . ./Interface ./Task 
#COMPONENT_ADD_INCLUDEDIRS += $(PROJECT_PATH)/Source $(PROJECT_PATH)/Source/Task	\
							$(PROJECT_PATH)/Source/MqttLib							\
						

#EXTRA_COMPONENT_DIRS += $(PROJECT_PATH)/Source $(PROJECT_PATH)/Source/Task $(PROJECT_PATH)/Source/MqttLib	\

#include project.mk

#ESPPORT = /dev/ttyUSB0
#ESPBAUD = 460800

COMPONENT_ADD_INCLUDEDIRS := components/include

COMPONENT_ADD_INCLUDEDIRS := components/bt/bluedroid/api/include/api
include $(IDF_PATH)/make/project.mk
