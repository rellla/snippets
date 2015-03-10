#include <stdio.h>
#include <math.h>
#include "csc.h"

static int set_csc_matrix(mixer_t *mix, csc_m *matrix)
{
	mix->csc_change = 1;

	// default contrast for full-range has 1.0 as luma coefficients
	mix->contrast = ((*matrix)[0][0] + (*matrix)[1][0] + (*matrix)[2][0]) / 3;

	// the way brightness and contrast work with this driver, brightness
	// is the brightness of a "black" pixel
	mix->brightness = ((*matrix)[0][1] + (*matrix)[1][1] + (*matrix)[2][1]) / 2 +
	                  ((*matrix)[0][2] + (*matrix)[1][2] + (*matrix)[2][2]) / 2 +
	                  (*matrix)[0][3] + (*matrix)[1][3] + (*matrix)[2][3];
	mix->brightness /= 3;

	float sin = (*matrix)[0][1] + (*matrix)[2][2];
	float cos = (*matrix)[0][2] + (*matrix)[2][1];
	float e = 0.001;
	if (-e < cos && cos < e) mix->hue = M_PI;
	else mix->hue = atanf(sin/cos);
	mix->saturation = sqrtf(sin * sin + cos * cos) / (1.403 + 1.773);

	return 0;
}

static int generate_csc_matrix(procamp_t *procamp, color_standard_t standard, csc_m *csc_matrix)
{
	if (!csc_matrix)
		return -1;

	static const csc_m *cstd;

	switch (standard) {
		case BT_709:
			cstd = &cs_bt709;
			break;
		case SMPTE_240M:
			cstd = &cs_smpte_240m;
			break;
		case BT_601:
		default:
			cstd = &cs_bt601;
			break;
	}

	float b = procamp ? procamp->brightness : 0.0f;
	float c = procamp ? procamp->contrast : 1.0f;
	float s = procamp ? procamp->saturation : 1.0f;
	float h = procamp ? procamp->hue : 0.0f;

	float ybias = -16.0f / 255.0f;
	float cbbias = -128.0f / 255.0f;
	float crbias = -128.0f / 255.0f;

	int i;
	for (i = 0; i < 3; i++) {
		(*csc_matrix)[i][0] = c * (*cstd)[i][0];
		(*csc_matrix)[i][1] = c * (*cstd)[i][1] * s * cosf(h) - c * (*cstd)[i][2] * s * sinf(h);
		(*csc_matrix)[i][2] = c * (*cstd)[i][2] * s * cosf(h) + c * (*cstd)[i][1] * s * sinf(h);
		(*csc_matrix)[i][3] = (*cstd)[i][3] + (*cstd)[i][0] * (b + c * ybias) +
		                      (*cstd)[i][1] * (c * cbbias * s * cosf(h) + c * crbias * s * sinf(h)) +
		                      (*cstd)[i][2] * (c * crbias * s * cosf(h) - c * cbbias * s * sinf(h));
	}

	return 0;
}

void print_matrix(csc_m *matrix)
{
	int i, j;

	for (i = 0; i < (sizeof(*matrix) / sizeof(*matrix[0])); i++)
	{
		for (j = 0; j < (sizeof(*matrix[0]) / sizeof(*matrix[0][0])); j++)
			printf("%10f ", (*matrix)[i][j]);
		printf("\n");
	}
}

int main ()
{

	static csc_m csc;
	static csc_m *csc_ptr;
	static const csc_m *cstd;
	static procamp_t procamp;
	static procamp_t *p_ptr;
	static mixer_t *mix_ptr;
	static mixer_t mix;
	color_standard_t standard;

	// Use Procamp to generate CSC Matrix
	procamp.brightness = 0.0f;
	procamp.saturation = 1.0f;
	procamp.contrast = 1.0f;
	procamp.hue = 0.0f;

	p_ptr = &procamp;


	printf("Procamp->Brightness: %f\n", p_ptr->brightness);
	printf("Procamp->Saturation: %f\n", p_ptr->saturation);
	printf("Procamp->Contrast  : %f\n", p_ptr->contrast);
	printf("Procamp->Hue       : %f\n", p_ptr->hue);

	p_ptr = &procamp;
	csc_ptr = &csc;
	mix_ptr = &mix;

	cstd = &cs_bt601;
	printf("BT_601 Matrix:\n");
	print_matrix((csc_m *)cstd);

	standard = BT_601;
	printf("BT_601 Matrix (Procamp):\n");
	if(!generate_csc_matrix(p_ptr, standard, csc_ptr))
		print_matrix(csc_ptr);

	cstd = &cs_bt709;
	printf("BT_709 Matrix:\n");
	print_matrix((csc_m *)cstd);

	standard = BT_709;
	printf("BT_709 Matrix (Procamp):\n");
	if(!generate_csc_matrix(p_ptr, standard, csc_ptr))
		print_matrix(csc_ptr);

	if(!set_csc_matrix(mix_ptr, csc_ptr))
	{
		printf("Procamp->Brightness: %f\n", p_ptr->brightness);
		printf("Procamp->Saturation: %f\n", p_ptr->saturation);
		printf("Procamp->Contrast  : %f\n", p_ptr->contrast);
		printf("Procamp->Hue       : %f\n", p_ptr->hue);
		printf("Mixer->Brightness: %f\n", mix_ptr->brightness);
		printf("Mixer->Saturation: %f\n", mix_ptr->saturation);
		printf("Mixer->Contrast  : %f\n", mix_ptr->contrast);
		printf("Mixer->Hue       : %f\n", mix_ptr->hue);
	}

	return 0;
}
