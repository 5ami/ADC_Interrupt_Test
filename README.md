| Supported Targets ESP32-S3 |
| -------------------------- |
| ESP-IDF v5.0      |
| -------------------------- |
# ADC_Interrupt_Project

+ This is a simple project to test the ability of the esp32s3 of changing the sample rate of 
the ADC in the continout mode. 

+ Register 39.75. **APB_SARADC_INT_ENA_REG (0x005C)** hold Different Interrupts setting.
+ The example tests **APB_SARADC_ADC1_DONE_INT_ENA** Enable bit
+ The example uses ADC1 , ADC_channel_2

## How to use example
The example uses Highlevel Interrupts in Assemblæy so make sure to include: 
**target_link_libraries(${COMPONENT_TARGET} "-u ld_include_xt_highint5")**
in the CMakeList.md  

## Example folder contents
```
├── CMakeLists.txt 
├── main
│   ├──gpio_driver             This is Just a GPIO simple driver
│      ├──gpio_driver.c 
│      ├──gpio_driver.h
│   ├──highint5.S              ISR interrupt handler in Assembly
│   ├──continous_main.c
└── README.md                  Description about the project
```
  
 