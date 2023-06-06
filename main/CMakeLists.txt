idf_component_register(SRCS "mcp2515CanController.cpp" "test.cpp" "twaiCanController.cpp"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES esp32_twai esp32-mcp2515 driver freertos )