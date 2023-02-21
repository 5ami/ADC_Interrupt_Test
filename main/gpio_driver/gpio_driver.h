#ifndef _GPIOD_H_
#define _GPIOD_H_
#include "soc/gpio_periph.h"
#include "soc/soc_caps.h"
#include "hal/gpio_ll.h"
#include "hal/gpio_types.h"
#include "soc/soc.h"    

void gpio_direction_set(gpio_num_t gpio_num, gpio_mode_t mode);
void gpio_level_set(gpio_num_t gpio_num, uint32_t level);
 
#endif /*_GPIOD_H_*/




   