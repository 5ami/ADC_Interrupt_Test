
#include <stdio.h>
#include <string.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "rom/ets_sys.h"
#include "xt_utils.h"
#include "gpio_driver/gpio_driver.h"
#include "esp_adc/adc_continuous.h"
#include "gpio_driver/gpio_driver.h"
#include "soc/apb_saradc_reg.h"
#include "hal/adc_ll.h"


#define EXAMPLE_READ_LEN   4 //at least 4 (Muliple of SOC_ADC_DIGI_DATA_BYTES_PER_CONV)


 

static const char *TAG = "EXAMPLE";
uint32_t   reg = 0;

void ADC_config(adc_continuous_handle_t *out_handle){

    adc_continuous_handle_t handle = NULL;
    adc_continuous_handle_cfg_t adc_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = EXAMPLE_READ_LEN,
    };
    
     ESP_ERROR_CHECK(adc_continuous_new_handle(&adc_config, &handle));
     
     adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 80000,
        .conv_mode = ADC_CONV_SINGLE_UNIT_1,
        .format = ADC_DIGI_OUTPUT_FORMAT_TYPE2, 
    };
 
    adc_digi_pattern_config_t adc_pattern ;
    dig_cfg.pattern_num = 1; //Number of ADC channels
    adc_pattern.atten     = ADC_ATTEN_DB_11;
    adc_pattern.channel   = ADC_CHANNEL_2;
    adc_pattern.unit      = ADC_UNIT_1 ;
    adc_pattern.bit_width = SOC_ADC_DIGI_MAX_BITWIDTH;
    dig_cfg.adc_pattern   = &adc_pattern;

    ESP_ERROR_CHECK(adc_continuous_config(handle, &dig_cfg));
    *out_handle = handle;
}

 
void app_main(void)
{   
    esp_err_t ret;
    uint32_t ret_num = 0; 
    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0xcc, EXAMPLE_READ_LEN);

    adc_continuous_handle_t handle = NULL;
    ADC_config(&handle); 
    /*Setting up the Interrupt*/
    /*APB_SARADC_ADC1_DONE_INT: Triggered when SAR ADC1 completes one data conversion.*/
    REG_SET_BIT(APB_SARADC_INT_ENA_REG, APB_SARADC_ADC1_DONE_INT_ENA); 
    /*Set it To level 5 Interrupt*/
    xt_ints_off(1<<31);
    intr_matrix_set(0, 65, 31); //APB_ADC_INT
    xt_ints_on(1<<31);
    gpio_direction_set(15,GPIO_MODE_OUTPUT);
    adc_continuous_start(handle);
   
    //while (1) {
        //ret = adc_continuous_read(handle, result, EXAMPLE_READ_LEN, &ret_num, 0);
        // ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32, ret, ret_num);
        // for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
        // adc_digi_output_data_t *p = (void*)&result[i];
        // ESP_LOGI(TAG, "Unit: %d,_Channel: %d, Value: %d", p->type2.unit+1, p->type2.channel, p->type2.data);
        //  }
        //  vTaskDelay(pdMS_TO_TICKS(1000));
    //}
 }     