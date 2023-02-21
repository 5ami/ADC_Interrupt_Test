#include "gpio_driver/gpio_driver.h"

gpio_dev_t *dev = GPIO_LL_GET_HW(GPIO_PORT_0);  //Neccesarry for all GPIO
void gpio_direction_set(gpio_num_t gpio_num, gpio_mode_t mode){
    if (mode & GPIO_MODE_DEF_INPUT) {        
        //gpio_ll_input_enable(gpio_context.gpio_hal, gpio_num);
        //PIN_INPUT_ENABLE(GPIO_PIN_MUX_REG[gpio_num]);
        //SET_PERI_REG_MASK(GPIO_PIN_MUX_REG[gpio_num],FUN_IE); 
        //Write value to register: read reg and mask it with a mask (FUN_IE)
        (*((volatile uint32_t *)((GPIO_PIN_MUX_REG[gpio_num])))) = (uint32_t)((*((volatile uint32_t *)((GPIO_PIN_MUX_REG[gpio_num]))))|(FUN_IE));       
    }else {
        //gpio_ll_input_disable((hal)->dev, gpio_num);
        //PIN_INPUT_DISABLE(GPIO_PIN_MUX_REG[gpio_num]); 
        CLEAR_PERI_REG_MASK(GPIO_PIN_MUX_REG[gpio_num],FUN_IE);
        (*((volatile uint32_t *)((GPIO_PIN_MUX_REG[gpio_num])))) = (uint32_t)((*((volatile uint32_t *)((GPIO_PIN_MUX_REG[gpio_num]))))&~(FUN_IE));     
    }
    if (mode & GPIO_MODE_DEF_OUTPUT) {   
        //gpio_ll_output_enable((gpio_context.gpio_hal)->dev, gpio_num);
        if (gpio_num < 32) {
            dev->enable_w1ts = (0x1 << gpio_num);
        }else {
            dev->enable1_w1ts.data = (0x1 << (gpio_num - 32));
        }   
       // esp_rom_gpio_connect_out_signal(gpio_num, SIG_GPIO_OUT_IDX, false, false);  //Not needed 
    }else {
        //gpio_ll_output_disable((hal)->dev, gpio_num)
        if (gpio_num < 32) {
            dev->enable_w1tc = (0x1 << gpio_num);
        } else {
            dev->enable1_w1tc.data = (0x1 << (gpio_num - 32));
        }
        //Ensure no other output signal is routed via GPIO matrix to this pin
        //REG_WRITE(GPIO_FUNC0_OUT_SEL_CFG_REG + (gpio_num * 4), SIG_GPIO_OUT_IDX);
        (*(volatile uint32_t *)(GPIO_FUNC0_OUT_SEL_CFG_REG + (gpio_num * 4))) = (SIG_GPIO_OUT_IDX);
    }
    if (mode & GPIO_MODE_DEF_OD) {
        //gpio_hal_od_enable(hal, gpio_num) gpio_ll_od_enable((hal)->dev, gpio_num)
        dev->pin[gpio_num].pad_driver = 1;
    } else {
        dev->pin[gpio_num].pad_driver = 0;
    }
}

void gpio_level_set(gpio_num_t gpio_num, uint32_t level){
      //gpio_ll_set_level(dev, gpio_num, level);
        if (level) {
        if (gpio_num < 32) {
             dev->out_w1ts = (1 << gpio_num);
        }else {
            dev->out1_w1ts.data = (1 << (gpio_num - 32));
        }
        } else {
          if (gpio_num < 32) {
             dev->out_w1tc = (1 << gpio_num);
        } else {
            dev->out1_w1tc.data = (1 << (gpio_num - 32));
        }
    }
}
