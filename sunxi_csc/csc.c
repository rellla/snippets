#include <stdio.h>
#include <math.h>
#include "csc.h"

static int set_csc_matrix(mixer_t *mix, csc_m *matrix, const csc_m *cstd)
{
	float ybias = -16.0f / 255.0f;
	float cbbias = -128.0f / 255.0f;
	float crbias = -128.0f / 255.0f;
	float asin;

	mix->csc_change = 1;

	// At least contrast was 0.0f. Set Hue and saturation to default. They cannot be guessed...
	if ((*matrix)[1][0] == 0 && (*matrix)[1][1] == 0 && (*matrix)[1][2] == 0)
	{
		mix->contrast = 0.0f;
		mix->hue = 0.0f;
		mix->saturation = 1.0f;
	}
	// Saturation was 0.0f. Set Hue to default. This cannot be guessed...
	else if ((*matrix)[1][1] == 0 && (*matrix)[1][2] == 0)
	{
		mix->saturation = 0.0f;
		// Contrast
		mix->contrast = (*matrix)[0][0] / (*cstd)[0][0];
		mix->hue = 0.0f;
	}
	else
	{
		// Contrast
		mix->contrast = (*matrix)[0][0] / (*cstd)[0][0];

		// Hue
		asin = asinf(sqrtf(pow(((*matrix)[1][1] * (*cstd)[1][2] - (*matrix)[1][2] * (*cstd)[1][1]), 2.0) /
		       (pow((-(*matrix)[1][1] * (*cstd)[1][1] - (*matrix)[1][2] * (*cstd)[1][2]), 2.0) +
		        pow(((*matrix)[1][1] * (*cstd)[1][2] - (*matrix)[1][2] * (*cstd)[1][1]), 2.0))));

		if (((*matrix)[2][1] < 0 && (*cstd)[2][1] < 0) || ((*matrix)[2][1] > 0 && (*cstd)[2][1] > 0))
			if (((*matrix)[0][1] < 0 && (*matrix)[0][2] > 0) || ((*matrix)[0][1] > 0 && (*matrix)[0][2] < 0))
				mix->hue = asin;
			else
				mix->hue = - asin;
		else
			if (((*matrix)[0][1] < 0 && (*matrix)[0][2] > 0) || ((*matrix)[0][1] > 0 && (*matrix)[0][2] < 0))
				mix->hue = - M_PI + asin;
			else
				mix->hue = M_PI - asin;

		// Check, if Hue was M_PI or -M_PI
		if ((fabs(fabs(mix->hue) - M_PI)) < 0.00001f)
			mix->hue = - mix->hue;

		// Saturation
		mix->saturation = (*matrix)[1][1] / (mix->contrast * ((*cstd)[1][1] * cosf(mix->hue) - (*cstd)[1][2] * sinf(mix->hue)));
	}

	// Brightness
	mix->brightness = ((*matrix)[1][3] -
	                  (*cstd)[1][1] * mix->contrast * mix->saturation * (cbbias * cosf(mix->hue) + crbias * sinf(mix->hue)) -
	                  (*cstd)[1][2] * mix->contrast * mix->saturation * (crbias * cosf(mix->hue) - cbbias * sinf(mix->hue)) -
	                  (*cstd)[1][3] - (*cstd)[1][0] * mix->contrast * ybias) / (*cstd)[1][0];

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
		(*csc_matrix)[i][1] = c * s * ((*cstd)[i][1] * cosf(h) - (*cstd)[i][2] * sinf(h));
		(*csc_matrix)[i][2] = c * s * ((*cstd)[i][2] * cosf(h) + (*cstd)[i][1] * sinf(h));
		(*csc_matrix)[i][3] = (*cstd)[i][3] + (*cstd)[i][0] * (b + c * ybias) +
		                      (*cstd)[i][1] * c * s * (cbbias * cosf(h) + crbias * sinf(h)) +
		                      (*cstd)[i][2] * c * s * (crbias * cosf(h) - cbbias * sinf(h));
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

	// Use procamp values to generate CSC matrix
	procamp.brightness = 0.0f;	// -1.0f ~  1.0f
	procamp.saturation = 1.0f;	//  0.0f ~ 10.0f
	procamp.contrast = 1.0f;	//  0.0f ~ 10.0f
	procamp.hue = 0.0f;		// -M_PI ~  M_PI

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

	if(!set_csc_matrix(mix_ptr, csc_ptr, cstd))
	{
		printf("-------------------------\n");
		printf("\t\tProcamp\t\t\tMixer\n");
		printf("Brightness:\t%f\t\t%f", p_ptr->brightness, mix_ptr->brightness);
		if (pow((p_ptr->brightness - mix_ptr->brightness), 2.0) < 0.00001f)
			printf("\tBrightness MATCH\n");
		else
			printf("\tERROR Brightness MATCH\n");
		printf("Saturation:\t%f\t\t%f", p_ptr->saturation, mix_ptr->saturation);
		if (pow((p_ptr->saturation - mix_ptr->saturation), 2.0) < 0.00001f)
			printf("\tSaturation MATCH\n");
		else
			printf("\tERROR Saturation MATCH\n");
		printf("Contrast:\t%f\t\t%f", p_ptr->contrast, mix_ptr->contrast);
		if (pow((p_ptr->contrast - mix_ptr->contrast), 2.0) < 0.00001f)
			printf("\tContrast MATCH\n");
		else
			printf("\tERROR Contrast MATCH\n");
		printf("Hue:\t\t%f\t\t%f", p_ptr->hue, mix_ptr->hue);
		if (pow((p_ptr->hue - mix_ptr->hue), 2.0) < 0.00001f)
			printf("\tHue MATCH\n");
		else
			printf("\tERROR Hue MATCH\n");
	}

	return 0;
}
