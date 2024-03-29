#include <math.h>
#include "lis3dsh.h"
#include "misc.h"

#include "interfaces/accelerometer_interface.h"
#include "utils/ma_filter.h"
#include "utils/utils.h"
#include "my_types.h"

/*	accelerometer_interface.c contains methods which will initialize the accelerometer sensor and take measurements to show tilts of the processor in terms of x, y, z axes
*	@param		x_buffer, y_buffer, z_buffer	arrays of size filter-depth	(14) + 2 to calculate moving average filter of the final result of x axis
*	@param		cb_x, cb_y, cb_z				circular buffers for each axis to implement moving average filter
*	@param		x_raw, y_raw, z_raw				raw values obtained from the accelerometer sensor before converting it to degrees, normalization and filtering
*/

static float x_buffer[MA_FILTER_DEPTH + 2];
static float y_buffer[MA_FILTER_DEPTH + 2];
static float z_buffer[MA_FILTER_DEPTH + 2];
static circular_buffer cb_x, cb_y, cb_z;
static ma_filter filter_x, filter_y, filter_z;

static int16_t x_raw, y_raw, z_raw;

/*	accelerometer_normalize method is used to calibrate the axis readings by using the lease square approximation
*/

static void accelerometer_normalize(float* x, float* y, float* z) {

#ifdef LSM9DS1
	static const float normalizing_matrix[3][3] = {
		{0.99221273,  0.00273199, -0.01238626},
		{-0.03079504,  0.97989342,  -0.04617855},
		{0.09919607,  0.10090316,  1.05888633}
	};
	
	static const float cx = 101.19645906;
	static const float cy = 235.91834136;
	static const float cz = -1850.67185213;
#else
	static const float normalizing_matrix[3][3] = {
		{1.05980676,  0.02639947, -0.00279462},
		{-0.03077862,  1.1679603,  -0.01424515},
		{-0.01497205,  0.05370348,  1.22005476}
	};
	
	static const float cx = -163.44321229;
	static const float cy = -142.16213624;
	static const float cz = -112.6033728;
#endif

	float normalized_values[3];
	for (uint8_t i = 0; i < 3; i++) {
		normalized_values[i] = normalizing_matrix[i][0] * (*x) + normalizing_matrix[i][1] * (*y) + normalizing_matrix[i][2] * (*z);
	}

	*x = (normalized_values[0]) + cx;
	*y = (normalized_values[1]) + cy;
	*z = (normalized_values[2]) + cz;
}

/*	accelerometer_init method will do the following:
*	1. configure LIS3DSH (since we are using C type processor) for the accelerometer sensor for all 3 axes
*	2. enable interrupt of the sensor so that it would activate once the data is ready to be taken for measurement
*	3. GPIO initialization to GPIOE with pin 0 on the processor that is set as input for the external interrupt (EXTI) generated by the sensor
*	4. Generate an interrupt request through NVIC (processor based) for the external interrupt that is generated by the sensor
*	5. Initialize the circular buffer to calculate the moving average filter for x, y, and z axes values
*/

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

/*	accelerometer_read_raw method will read 6 types of measurements: MSB (H) and LSB (L) of the acceleration signals acting on X, Y and Z axes respectively
*	@param 		x_raw		will store the LSB measurement of X and add MSB of X (which is shifted by 8 bits)
*	@param 		y_raw		will store the LSB measurement of Y and add MSB of Y (which is shifted by 8 bits)
*	@param 		z_raw		will store the LSB measurement of Z and add MSB of Z (which is shifted by 8 bits)
*/

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

/*	accelerometer_read method will initialize the normalization/calibration of the processor's accelerometer sensor for the raw data 
*	@param 		x, y, z 	will store the filtered average after adding the raw data for each axis into the circular buffer for each axis	
*/

void accelerometer_read(float* x, float *y, float *z) {
	float x_norm = (float) x_raw;
	float y_norm = (float) y_raw;
	float z_norm = (float) z_raw;
	
	accelerometer_normalize(&x_norm, &y_norm, &z_norm);

	*x = ma_filter_add(&filter_x, x_norm);
	*y = ma_filter_add(&filter_y, y_norm);
	*z = ma_filter_add(&filter_z, z_norm);
}

/*	accelerometer_calculate_rotation method will do the following calculations:
*	radian_to_degree			will convert input value from radian format to degree format
*	@param		accelerometer_angles.pitch 		will produce angle (between x axis and horizontal plane) in the form of degrees 
*	@param		accelerometer_angles.roll 		will produce angle (between y axis and horizontal plane) in the form of degrees 
*	@param		accelerometer_angles.yaw 		will produce angle (between z axis and horizontal plane) in the form of degrees 
*	equation with arctan function is used to produce constant sensitivity over 360 degrees rotation of the processor in X, Y or Z axis
*/

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

/*	EXTI0_IRQHandler method will check the status of the external interrupt generated if it's not reset to clear EXTI Line0
*	accelerometer_read_raw method will be called to read the raw value generated from the accelerometer sensor
*/

//void EXTI0_IRQHandler(void) {
//	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {	
//		EXTI_ClearITPendingBit(EXTI_Line0);
//		accelerometer_read_raw();
//	}
//}
