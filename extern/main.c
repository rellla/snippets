/*
 * Copyright (c) 2015 Andreas Baierl <ichgeh@imkreisrum.de>
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

#include "thread.h"
#include <pthread.h>
#include <stdio.h>

static pthread_t thread_1;
static pthread_t thread_2;
extern vars var;

int main()
{
	vars *var_ptr = &var;

	var_ptr->thread_exit = 0;
	printf("MAIN: Start\n");

	pthread_create(&thread_1, NULL, thread1, var_ptr);
	pthread_create(&thread_2, NULL, thread2, var_ptr);

	pthread_join(thread_1, NULL);
	pthread_join(thread_2, NULL);

	printf("MAIN: Ended, wait 2 seconds ...\n");

	usleep(2 * 1000 * 1000);
	printf("MAIN: Exit!\n");

	return 0;
}
