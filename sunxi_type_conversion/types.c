#include <stdio.h>
#include <math.h>

int do_conversion1(float bright_in, float contrast_in, float saturation_in, float hue_in);
int do_conversion2(unsigned long bright_in, unsigned long contrast_in, unsigned long saturation_in, unsigned long hue_in);
int do_conversion3(signed int bright_in, signed int contrast_in, signed int saturation_in, signed int hue_in);


int do_conversion1(float bright_in, float contrast_in, float saturation_in, float hue_in)
{
        unsigned long bright_out, contrast_out, saturation_out, hue_out;

        printf("    Bright In: %f\n", bright_in);
        printf("    Contrast In: %f\n", contrast_in);
        printf("    Saturation In: %f\n", saturation_in);
        printf("    Hue In: %f\n", hue_in);

        bright_out = (unsigned long)(((bright_in + 1.0) / 2.0 * 100.0) + 0.5);
	if (contrast_in <= 1.0)
		contrast_out = (unsigned long)((contrast_in * 50.0) + 0.5);
	else
		contrast_out = (unsigned long)((50 + (contrast_in / 10) * 50.0) + 0.5);
	if (saturation_in <= 1.0)
		saturation_out = (unsigned long)((saturation_in * 50.0) + 0.5);
	else
		saturation_out = (unsigned long)((50 + (saturation_in / 10) * 50.0) + 0.5);
        hue_out = (unsigned long)((((hue_in / M_PI) + 1.0) / 2.0 * 100.0) + 0.5);

        printf("Conversion to 0-100:\n");
        printf("    Bright Out: %lu\n", bright_out);
        printf("    Contrast Out: %lu\n", contrast_out);
        printf("    Saturation Out: %lu\n", saturation_out);
        printf("    Hue Out: %lu\n", hue_out);

	return do_conversion2(bright_out, contrast_out, saturation_out, hue_out);
}

int do_conversion2(unsigned long bright_in, unsigned long contrast_in, unsigned long saturation_in, unsigned long hue_in)
{
        printf("Conversion to 0-32:\n");

	return do_conversion3(bright_in, contrast_in, saturation_in, hue_in);
}

int do_conversion3(signed int bright, signed int contrast, signed int saturation, signed int hue)
{
        bright = (bright * 63.0 / 100.0) + 0.5;
        contrast = (contrast * 63.0 / 100.0) + 0.5;
        saturation = (saturation * 63.0 / 100.0) + 0.5;
        hue = (hue * 63.0 / 100.0) + 0.5;

        printf("    Bright: %d\n", bright);
        printf("    Contrast: %d\n", contrast);
        printf("    Saturation: %d\n", saturation);
        printf("    Hue: %d\n", hue);

	return 0;
}

int main()
{
        float bright_in, contrast_in, saturation_in, hue_in;

        printf("--------Minimum--------\n");
        bright_in = -1.0;
        contrast_in = 0.0;
        saturation_in = 0.0;
        hue_in = -M_PI;
	do_conversion1(bright_in, contrast_in, saturation_in, hue_in);

        printf("-----No change---------\n");
        bright_in = 0.0;
        contrast_in = 1.0;
        saturation_in = 1.0;
        hue_in = 0.0;
	do_conversion1(bright_in, contrast_in, saturation_in, hue_in);

        printf("--------Maximum--------\n");
        bright_in = 1.0;
        contrast_in = 10.0;
        saturation_in = 10.0;
        hue_in = M_PI;

	do_conversion1(bright_in, contrast_in, saturation_in, hue_in);

        return 0;
}

/*
Conversion to 0-100:

Bright In: 0.345000, Bright Out: 67
Contrast In: 3.778000, Contrast Out: 38
Saturation In: 9.300000, Saturation Out: 93
Hue In: -1.200000, Hue Out: 31

Conversion to 0-32:

Bright In: 67, Bright Out: 42
Contrast In: 38, Contrast Out: 24
Saturation In: 93, Saturation Out: 59
Hue In: 31, Hue Out: 19
*/
