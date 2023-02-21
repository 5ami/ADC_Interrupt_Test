#include <xtensa/coreasm.h>
#include <xtensa/corebits.h>
#include <xtensa/config/system.h>
#include "freertos/xtensa_context.h"
#include "esp_private/panic_reason.h"
#include "sdkconfig.h"
#include "soc/soc.h"
#include "soc/gpio_reg.h"
#include "soc/dport_reg.h"

#define L5_INTR_STACK_SIZE  12
#define LX_INTR_A15_OFFSET 0
#define LX_INTR_A14_OFFSET 4
#define LX_INTR_A13_OFFSET 8
    .data
_l5_intr_stack:
    .space      L5_INTR_STACK_SIZE

    .section .iram1,"ax"
    .global     xt_highint5
    .type       xt_highint5,@function
    .align      4
xt_highint5:

	movi    a0, _l5_intr_stack // assuming a0 saved restored by EXCSAVE_5 at the end of this
    //about 18 ns for these three saves 
    s32i    a15, a0, LX_INTR_A15_OFFSET
    s32i    a14, a0, LX_INTR_A14_OFFSET
    s32i    a13, a0, LX_INTR_A13_OFFSET

    /* Clear ADC EOC interupt */
    movi a14,  0x6004005C
    movi a15,  0x0
    memw 
    s32i a15,  a14, 0
    
    // toggle output reg
    movi    a13, GPIO_OUT_W1TS_REG
    movi    a15, (1 << 15)
    s32i	a15, a13, 0
    movi    a14, GPIO_OUT_W1TC_REG
    s32i	a15, a14, 0

    /* Enable EOC interupt */
    movi a14,  0x6004005C
    movi a15,  0x80000000
    memw 
    s32i a15,  a14, 0

    memw
   	l32i    a15, a0, LX_INTR_A15_OFFSET
    l32i    a14, a0, LX_INTR_A14_OFFSET
    l32i    a13, a0, LX_INTR_A13_OFFSET

    rsr     a0, EXCSAVE_5 // restore a0
    rfi     5

    .global ld_include_xt_highint5
ld_include_xt_highint5:
