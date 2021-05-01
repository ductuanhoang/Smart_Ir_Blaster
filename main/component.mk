#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#
COMPONENT_SRCDIRS := .  Interface  Task  \
						Led_driver \
						Interface/shell \
						Interface/Logger_File \
						Interface/Spiffs \
						Ir_driver \
						Ir_driver/ir_protocol_cpp \
						json_parser \
						json_parser/cJson_lib \
						app_test \
						data_base \
						mqtt_driver\
						mqtt_driver/MqttLib

COMPONENT_ADD_INCLUDEDIRS := Interface  \
							app_test \
							data_base \
							Task  \
							Led_driver \
							Interface/shell \
							Interface/Logger_File \
							Interface/Spiffs \
							Ir_driver \
							Ir_driver/ir_protocol_cpp \
							json_parser \
							json_parser/cJson_lib

COMPONENT_ADD_INCLUDEDIRS := components/bt/bluedroid/api/include/api