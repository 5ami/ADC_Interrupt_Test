idf_component_register(SRCS "continous_main.c"
                            "highint5.S"
                            "gpio_driver/gpio_driver.c"
                     INCLUDE_DIRS ".")

target_link_libraries(${COMPONENT_TARGET} "-u ld_include_xt_highint5")
