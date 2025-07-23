idf_component_register(SRCS "WiFi/wifi_manager.cpp"
 "main.cpp"
 "audio/audio_manager.cpp"
 "audio/audio_capture.cpp"
 "audio/audio_utils.cpp"
 "audio/audio_manager.cpp"
 "audio/audio_utils.cpp"
                       INCLUDE_DIRS "." "WiFi" "audio"
                       REQUIRES esp_wifi nvs_flash esp_event audio_utils

                       PRIV_REQUIRES esp_system esp_event driver)
target_link_libraries(${COMPONENT_LIB} PRIVATE "-lstdc++")
include_directories(${IDF_PATH}/components/driver/include)

target_link_libraries(xiaozhi-esp32 PRIVATE opus lame libsndfile)