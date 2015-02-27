#include <stdio.h>
#include <math.h>

int do_conversion1(float bright_in, float contrast_in, float saturation_in, float hue_in);
int do_conversion2(unsigned long bright_in, unsigned long contrast_in, unsigned long saturation_in, unsigned long hue_in);


int do_conversion1(float bright_in, float contrast_in, float saturation_in, float hue_in)
{
        unsigned long bright_out, contrast_out, saturation_out, hue_out;

        bright_out = (unsigned long)(((bright_in + 1.0) / 2.0 * 100.0) + 0.5);
        contrast_out = (unsigned long)((contrast_in * 10.0) + 0.5);
        saturation_out = (unsigned long)((saturation_in * 10.0) + 0.5);
        hue_out = (unsigned long)((((hue_in / M_PI) + 1.0) / 2.0 * 100.0) + 0.5);

        printf("\nConversion to 0-100:\n\n");
        printf("Bright In: %f, Bright Out: %lu\n", bright_in, bright_out);
        printf("Contrast In: %f, Contrast Out: %lu\n", contrast_in, contrast_out);
        printf("Saturation In: %f, Saturation Out: %lu\n", saturation_in, saturation_out);
        printf("Hue In: %f, Hue Out: %lu\n", hue_in, hue_out);

	do_conversion2(bright_out, contrast_out, saturation_out, hue_out);
}

int do_conversion2(unsigned long bright_in, unsigned long contrast_in, unsigned long saturation_in, unsigned long hue_in)
{
        signed int bright_out, contrast_out, saturation_out, hue_out;

        bright_out = (signed int)((bright_in * 63.0 / 100.0) + 0.5);
        contrast_out = (signed int)((contrast_in * 63.0 / 100.0) + 0.5);
        saturation_out = (signed int)((saturation_in * 63.0 / 100.0) + 0.5);
        hue_out = (signed int)((hue_in * 63.0 / 100.0) + 0.5);

        printf("\nConversion to 0-32:\n\n");
        printf("Bright In: %lu, Bright Out: %d\n", bright_in, bright_out);
        printf("Contrast In: %lu, Contrast Out: %d\n", contrast_in, contrast_out);
        printf("Saturation In: %lu, Saturation Out: %d\n", saturation_in, saturation_out);
        printf("Hue In: %lu, Hue Out: %d\n", hue_in, hue_out);
        printf("\n-----------------------");
}

int main()
{
        float bright_in, contrast_in, saturation_in, hue_in;

        bright_in = -0.02;
        contrast_in = 5.1;
        saturation_in = 5.0;
        hue_in = 0.0;

	do_conversion1(bright_in, contrast_in, saturation_in, hue_in);

        bright_in = -1.0;
        contrast_in = 0.0;
        saturation_in = 0.0;
        hue_in = -M_PI;

	do_conversion1(bright_in, contrast_in, saturation_in, hue_in);

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
