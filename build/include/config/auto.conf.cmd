deps_config := \
	/d/Aditya/ESP/esp-idf/components/app_trace/Kconfig \
	/d/Aditya/ESP/esp-idf/components/aws_iot/Kconfig \
	/d/Aditya/ESP/esp-idf/components/bt/Kconfig \
	/d/Aditya/ESP/esp-idf/components/driver/Kconfig \
	/d/Aditya/ESP/esp-idf/components/esp32/Kconfig \
	/d/Aditya/ESP/esp-idf/components/esp_adc_cal/Kconfig \
	/d/Aditya/ESP/esp-idf/components/esp_http_client/Kconfig \
	/d/Aditya/ESP/esp-idf/components/ethernet/Kconfig \
	/d/Aditya/ESP/esp-idf/components/fatfs/Kconfig \
	/d/Aditya/ESP/esp-idf/components/freertos/Kconfig \
	/d/Aditya/ESP/esp-idf/components/heap/Kconfig \
	/d/Aditya/ESP/esp-idf/components/libsodium/Kconfig \
	/d/Aditya/ESP/esp-idf/components/log/Kconfig \
	/d/Aditya/ESP/esp-idf/components/lwip/Kconfig \
	/d/Aditya/ESP/esp-idf/components/mbedtls/Kconfig \
	/d/Aditya/ESP/esp-idf/components/mdns/Kconfig \
	/d/Aditya/ESP/esp-idf/components/openssl/Kconfig \
	/d/Aditya/ESP/esp-idf/components/pthread/Kconfig \
	/d/Aditya/ESP/esp-idf/components/spi_flash/Kconfig \
	/d/Aditya/ESP/esp-idf/components/spiffs/Kconfig \
	/d/Aditya/ESP/esp-idf/components/tcpip_adapter/Kconfig \
	/d/Aditya/ESP/esp-idf/components/vfs/Kconfig \
	/d/Aditya/ESP/esp-idf/components/wear_levelling/Kconfig \
	/d/Aditya/ESP/esp-idf/Kconfig.compiler \
	/d/Aditya/ESP/esp-idf/components/bootloader/Kconfig.projbuild \
	/d/Aditya/ESP/esp-idf/components/esptool_py/Kconfig.projbuild \
	/d/Aditya/Codes/esp32/webserverTry/main/Kconfig.projbuild \
	/d/Aditya/ESP/esp-idf/components/partition_table/Kconfig.projbuild \
	/d/Aditya/ESP/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
