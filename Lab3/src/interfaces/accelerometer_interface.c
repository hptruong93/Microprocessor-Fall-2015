#include <math.h>
#include "lis3dsh.h"
#include "misc.h"

#include "interfaces/accelerometer_interface.h"
#include "utils/ma_filter.h"
#include "utils/utils.h"
#include "my_types.h"


static int16_t x_buffer[MA_FILTER_DEPTH + 2];
static int16_t y_buffer[MA_FILTER_DEPTH + 2];
static int16_t z_buffer[MA_FILTER_DEPTH + 2];
static circular_buffer cb_x, cb_y, cb_z;
static ma_filter filter_x, filter_y, filter_z;

static int16_t x_raw, y_raw, z_raw;

static void accelerometer_normalize(int16_t* x, int16_t* y, int16_t* z) {
	//Do nothing for now
}

void accelerometer_init(void) {
	LIS3DSH_InitTypeDef  LIS3DSH_InitStruct;

	/* Set configuration of LIS302DL*/
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate = LIS3DSH_DATARATE_100;
	LIS3DSH_InitStruct.Axes_Enable = LIS3DSH_X_ENABLE | LIS3DSH_Y_ENABLE | LIS3DSH_Z_ENABLE;
	LIS3DSH_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_4;
	LIS3DSH_InitStruct.AA_Filter_BW = LIS3DSH_AA_BW_200;
	LIS3DSH_InitStruct.Continous_Update = LIS3DSH_ContinousUpdate_Enabled;
	LIS3DSH_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;
	LIS3DSH_Init(&LIS3DSH_InitStruct);

	/* Enable interrupt when data is ready */	
	LIS3DSH_DRYInterruptConfigTypeDef DRYInterruptConfig;
	DRYInterruptConfig.Dataready_Interrupt = LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	DRYInterruptConfig.Interrupt_signal = LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	DRYInterruptConfig.Interrupt_type = LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_DataReadyInterruptConfig(&DRYInterruptConfig);
	
	/* Set pin as input */
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	/* Tell system that you will use PE0 for EXTI_Line0 */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0);
	
	/* PE0 is connected to EXTI_Line0 */
	EXTI_InitTypeDef EXTI_InitStruct;
	EXTI_InitStruct.EXTI_Line = EXTI_Line0;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	ma_filter_initialize(&filter_x, &cb_x, x_buffer);
	ma_filter_initialize(&filter_y, &cb_y, y_buffer);
	ma_filter_initialize(&filter_z, &cb_z, z_buffer);
}

static void accelerometer_read_raw(void) {
	uint8_t Buffer[6];
	LIS3DSH_Read(&Buffer[0], LIS3DSH_OUT_X_L, 1);
	LIS3DSH_Read(&Buffer[1], LIS3DSH_OUT_X_H, 1);
	LIS3DSH_Read(&Buffer[2], LIS3DSH_OUT_Y_L, 1);
	LIS3DSH_Read(&Buffer[3], LIS3DSH_OUT_Y_H, 1);
	LIS3DSH_Read(&Buffer[4], LIS3DSH_OUT_Z_L, 1);
	LIS3DSH_Read(&Buffer[5], LIS3DSH_OUT_Z_H, 1);
	
	x_raw = Buffer[0] + (int16_t)(Buffer[1] << 8);
	y_raw = Buffer[2] + (int16_t)(Buffer[3] << 8);
	z_raw = Buffer[4] + (int16_t)(Buffer[5] << 8);
}

void accelerometer_read(float* x, float *y, float *z) {
	accelerometer_normalize(&x_raw, &y_raw, &z_raw);

	*x = ma_filter_add(&filter_x, x_raw);
	*y = ma_filter_add(&filter_y, y_raw);
	*z = ma_filter_add(&filter_z, z_raw);
}

void accelerometer_calculate_rotation(float x, float y, float z) {
	if (y != 0 && z != 0) {
		accelerometer_angles.pitch = radian_to_degree(atan(x / sqrt(y*y + z*z)));
	}

	if (x != 0 && z != 0) {
		accelerometer_angles.roll = radian_to_degree(atan(y / sqrt(x*x + z*z)));
	}

	if (x != 0 && y != 0) {
		accelerometer_angles.yaw = radian_to_degree(atan(z / sqrt(x*x + y*y)));
	}
}

uint8_t c;
void EXTI0_IRQHandler(void) {
	c++;
	accelerometer_read_raw();
	EXTI_ClearITPendingBit(EXTI_Line0);
}
