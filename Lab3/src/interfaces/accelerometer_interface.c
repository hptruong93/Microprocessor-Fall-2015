#include <math.h>
#include "lis3dsh.h"

#include "interfaces/accelerometer_interface.h"
#include "utils/ma_filter.h"
#include "utils/utils.h"
#include "my_types.h"


static int16_t x_buffer[MA_FILTER_DEPTH + 2];
static int16_t y_buffer[MA_FILTER_DEPTH + 2];
static int16_t z_buffer[MA_FILTER_DEPTH + 2];
static circular_buffer cb_x, cb_y, cb_z;
static ma_filter filter_x, filter_y, filter_z;

accelerometer_info accelerometer_angles;

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

	ma_filter_initialize(&filter_x, &cb_x, x_buffer);
	ma_filter_initialize(&filter_y, &cb_y, y_buffer);
	ma_filter_initialize(&filter_z, &cb_z, z_buffer);
}

void accelerometer_read(float* x, float *y, float *z) {
	uint8_t Buffer[6];
	LIS3DSH_Read(&Buffer[0], LIS3DSH_OUT_X_L, 1);
	LIS3DSH_Read(&Buffer[1], LIS3DSH_OUT_X_H, 1);
	LIS3DSH_Read(&Buffer[2], LIS3DSH_OUT_Y_L, 1);
	LIS3DSH_Read(&Buffer[3], LIS3DSH_OUT_Y_H, 1);
	LIS3DSH_Read(&Buffer[4], LIS3DSH_OUT_Z_L, 1);
	LIS3DSH_Read(&Buffer[5], LIS3DSH_OUT_Z_H, 1);
	
	int16_t	x_int = Buffer[0] + (int16_t)(Buffer[1] << 8);
	int16_t y_int = Buffer[2] + (int16_t)(Buffer[3] << 8);
	int16_t z_int = Buffer[4] + (int16_t)(Buffer[5] << 8);
	accelerometer_normalize(&x_int, &y_int, &z_int);

	*x = ma_filter_add(&filter_x, x_int);
	*y = ma_filter_add(&filter_y, y_int);
	*z = ma_filter_add(&filter_z, z_int);
	
	printf("(%.2f, %.2f, %.2f)\n", *x, *y, *z);
}

void accelerometer_calculate_rotation(float x, float y, float z) {
	accelerometer_angles.pitch = radian_to_degree(atan(x / sqrt(y*y + z*z)));
	accelerometer_angles.roll = radian_to_degree(atan(y / sqrt(x*x + z*z)));
	accelerometer_angles.yaw = radian_to_degree(atan(z / sqrt(x*x + y*y)));
}
