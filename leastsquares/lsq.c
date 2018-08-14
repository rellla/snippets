/*
 * Copyright (c) 2018 Andreas Baierl <ichgeh@imkreisrum.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include <stdio.h>

#define MAXPOINTS 5
#define DEBUG 1

static const float points1[MAXPOINTS][2] = {
    {    000,    200},
    {    100,    100},
    {    200,    000},
    {    300,   -100},
    {    400,    100}
};

static const float points2[MAXPOINTS][2] = {
    {    000,    100},
    {    100,    000},
    {    200,    110},
    {    300,    400},
    {   -100,    390}
};

static const float points3[MAXPOINTS][2] = {
    {    476,     404},
    {    600,    3335},
    {    761,    5408},
    {    944,    3895},
    {    541,    2167}
};

int main()
{
    float A[MAXPOINTS][3];
    float points[MAXPOINTS][2];
    float AT[3][MAXPOINTS];
    float ATA[3][3];
    float ATB[3];
    float A1[3][3];
    float A2[3][3];
    float A3[3][3];
    float sum, deta1, deta2, deta3, detata = 0.0;
    float a, b, c = 0.0;
    int i, j, k, m = 0;

    /* which array should we use? */
    for (i = 0; i < MAXPOINTS; i++) {
	points[i][0] = points2[i][0]/100.0;
	points[i][1] = points2[i][1]/100.0;
	points[i][2] = points2[i][2]/100.0;
    }

#ifdef DEBUG
    printf("These are our original points:\n");
    for (i = 0; i < MAXPOINTS; i++) {
	printf("%.2f", points[i][0]);
	printf("\t%.2f\n", points[i][1]);
    }
    printf("\n");
#endif

    /* fill A matrix */
    for (i = 0; i < MAXPOINTS; i++) {
	A[i][0] = 1;
	A[i][1] = points[i][0];
	A[i][2] = points[i][0] * points[i][0];
    }

#ifdef DEBUG
    printf("These is the A matrix:\n");
    for (i = 0; i < MAXPOINTS; i++) {
	printf("%.2f", A[i][0]);
	printf("\t%.2f", A[i][1]);
	printf("\t%.2f\n", A[i][2]);
    }
    printf("\n");
#endif

    /* fill AT matrix */
    for (i = 0; i < MAXPOINTS; i++) {
	AT[0][i] = A[i][0];
	AT[1][i] = A[i][1];
	AT[2][i] = A[i][2];
    }

#ifdef DEBUG
    printf("These is the AT matrix:\n");
    for (i = 0; i < 3; i++) {
	printf("%.2f", AT[i][0]);
	printf("\t%.2f", AT[i][1]);
	printf("\t%.2f", AT[i][2]);
	printf("\t%.2f", AT[i][3]);
	printf("\t%.2f\n", AT[i][4]);
    }
    printf("\n");
#endif

    /* multiplicate A * AT matrix */
    sum = 0.0;
    for (j = 0; j < 3; j++) {
	for (k = 0; k < 3; k++) {
	    for (m = 0; m < MAXPOINTS; m++) {
		sum += AT[j][m] * A[m][k];
	    }
	    ATA[j][k] = sum;
	    sum = 0.0;
	}
    }

#ifdef DEBUG
    printf("These is the ATA matrix:\n");
    for (i = 0; i < 3; i++) {
	printf("%.2f", ATA[i][0]);
	printf("\t%.2f", ATA[i][1]);
	printf("\t%.2f\n", ATA[i][2]);
    }
    printf("\n");
#endif

    /* multiplicate B * AT matrix */
    sum = 0.0;
    for (k = 0; k < 3; k++) {
	for (m = 0; m < MAXPOINTS; m++) {
	    sum += AT[k][m] * points[m][1];
	}
	ATB[k] = sum;
	sum = 0.0;
    }

#ifdef DEBUG
    printf("These is the ATB matrix:\n");
    printf("%.2f", ATB[0]);
    printf("\t%.2f", ATB[1]);
    printf("\t%.2f\n", ATB[2]);
    printf("\n");
#endif

    /* fill A1 - A3 matrix */
    for (k = 0; k < 3; k++) {
	A1[k][0] = ATB[k];
	A1[k][1] = ATA[k][1];
	A1[k][2] = ATA[k][2];
	A2[k][0] = ATA[k][0];
	A2[k][1] = ATB[k];
	A2[k][2] = ATA[k][2];
	A3[k][0] = ATA[k][0];
	A3[k][1] = ATA[k][1];
	A3[k][2] = ATB[k];
    }

#ifdef DEBUG
    printf("These is the A1 matrix:\n");
    for (i = 0; i < 3; i++) {
	printf("%.2f", A1[i][0]);
	printf("\t%.2f", A1[i][1]);
	printf("\t%.2f\n", A1[i][2]);
    }
    printf("\n");
    printf("These is the A2 matrix:\n");
    for (i = 0; i < 3; i++) {
	printf("%.2f", A2[i][0]);
	printf("\t%.2f", A2[i][1]);
	printf("\t%.2f\n", A2[i][2]);
    }
    printf("\n");
    printf("These is the A3 matrix:\n");
    for (i = 0; i < 3; i++) {
	printf("%.2f", A3[i][0]);
	printf("\t%.2f", A3[i][1]);
	printf("\t%.2f\n", A3[i][2]);
    }
    printf("\n");
#endif

    /* calculate determninants */
    deta1 = (A1[0][0] * A1[1][1] * A1[2][2]) +
	    (A1[0][1] * A1[1][2] * A1[2][0]) +
	    (A1[0][2] * A1[1][0] * A1[2][1]) -
	    (A1[0][2] * A1[1][1] * A1[2][0]) -
	    (A1[0][0] * A1[1][2] * A1[2][1]) -
	    (A1[0][1] * A1[1][0] * A1[2][2]);

    deta2 = (A2[0][0] * A2[1][1] * A2[2][2]) +
	    (A2[0][1] * A2[1][2] * A2[2][0]) +
	    (A2[0][2] * A2[1][0] * A2[2][1]) -
	    (A2[0][2] * A2[1][1] * A2[2][0]) -
	    (A2[0][0] * A2[1][2] * A2[2][1]) -
	    (A2[0][1] * A2[1][0] * A2[2][2]);

    deta3 = (A3[0][0] * A3[1][1] * A3[2][2]) +
	    (A3[0][1] * A3[1][2] * A3[2][0]) +
	    (A3[0][2] * A3[1][0] * A3[2][1]) -
	    (A3[0][2] * A3[1][1] * A3[2][0]) -
	    (A3[0][0] * A3[1][2] * A3[2][1]) -
	    (A3[0][1] * A3[1][0] * A3[2][2]);

    detata = (ATA[0][0] * ATA[1][1] * ATA[2][2]) +
	     (ATA[0][1] * ATA[1][2] * ATA[2][0]) +
	     (ATA[0][2] * ATA[1][0] * ATA[2][1]) -
	     (ATA[0][2] * ATA[1][1] * ATA[2][0]) -
	     (ATA[0][0] * ATA[1][2] * ATA[2][1]) -
	     (ATA[0][1] * ATA[1][0] * ATA[2][2]);

#ifdef DEBUG
    printf("These are the determinantes:\n");
    printf("A1: %.2f\n", deta1);
    printf("A2: %.2f\n", deta2);
    printf("A3: %.2f\n", deta3);
    printf("ATA: %.2f\n", detata);
    printf("\n");
#endif

    a = (float)deta1 / (float)detata;
    b = (float)deta2 / (float)detata;
    c = (float)deta3 / (float)detata;

    printf("Use this formula::\n");
    printf("y = (%.2f)x^2 + (%.2f)x + (%.2f)\n", c, b, a);
    printf("\n");

    return 0;
}
