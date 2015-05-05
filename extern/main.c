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

static pthread_t thread_id;
extern vars var;

int main()
{
	vars *var_ptr = &var;
	var_ptr->thread_exit = 0;
	printf("Start\n");

	pthread_create(&thread_id, NULL, thread, var_ptr);

	usleep(5 * 1000 * 1000);

	var_ptr->thread_exit = 1;
	pthread_join(thread_id, NULL);
	printf("Ended, wait 2 seconds ...\n");

	usleep(2 * 1000 * 1000);
	printf("Exit!\n");

	return 0;
}
