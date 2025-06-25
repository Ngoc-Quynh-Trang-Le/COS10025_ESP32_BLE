idf_component_register(SRCS "main.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES bt nvs_flash
                       PRIV_REQUIRES esp_driver_gpio)
