#include <math.h>

typedef enum {
	BT_709,
	SMPTE_240M,
	BT_601
} color_standard_t;

typedef struct {
	float brightness;
	float saturation;
	float contrast;
	float hue;
} procamp_t;

typedef struct {
	int   csc_change;
	float brightness;
	float saturation;
	float contrast;
	float hue;
} mixer_t;

typedef float csc_m[3][4];

static const csc_m cs_bt601 = {
	{ 1.164f,  0.0f,     1.793f,   0.0f },
	{ 1.164f,  -0.213f,  -0.534f,  0.0f },
	{ 1.164f,  2.115f,   0.0f,     0.0f }
};

static const csc_m test123 = {
	{ 1.1f,  1.2f,     1.3f,     1.4f },
	{ 2.1f,  2.2f,     2.3f,     2.4f },
	{ 3.1f,  3.2f,     3.3f,     3.4f }
};

static const csc_m cs_bt709 = {
	{ 1.164f,  0.0f,     1.596f,   0.0f, },
	{ 1.164f,  -0.336f,  -0.813f,  0.0f, },
	{ 1.164f,  2.018f,   0.0f,     0.0f, }
};

static const csc_m cs_smpte_240m = {
	{ 1.164f,  0.0f,     1.794f,   0.0f, },
	{ 1.164f,  -0.258f,  -0.543f,  0.0f, },
	{ 1.164f,  2.079f,   0.0f,     0.0f, }
};

static int set_csc_matrix(mixer_t *mix, csc_m *matrix, const csc_m *cstd);
static int generate_csc_matrix(procamp_t *procamp, color_standard_t standard, csc_m *csc_matrix);
