/*
 *   portspoof       Service signature obfucastor 
 *   Copyright (C) 12012 Piotr Duszyński <piotr[at]duszynski.eu>
 *
 *   This program is free software; you can redistribute it and/or modify it
 *   under the terms of the GNU General Public License as published by the
 *   Free Software Foundation; either version 2 of the License, or (at your
 *   option) any later version.
 * 
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *   See the GNU General Public License for more details.
 * 
 *   You should have received a copy of the GNU General Public License along
 *   with this program; if not, see <http://www.gnu.org/licenses>.
 * 
 *   Linking portspoof statically or dynamically with other modules is making
 *   a combined work based on portspoof. Thus, the terms and conditions of
 *   the GNU General Public License cover the whole combination.
 * 
 *   In addition, as a special exception, the copyright holder of portspoof
 *   gives you permission to combine portspoof with free software programs or
 *   libraries that are released under the GNU LGPL. You may copy
 *   and distribute such a system following the terms of the GNU GPL for
 *   portspoof and the licenses of the other code concerned.
 * 
 *   Note that people who make modified versions of portspoof are not obligated
 *   to grant this special exception for their modified versions; it is their
 *   choice whether to do so. The GNU General Public License gives permission
 *   to release a modified version without this exception; this exception
 *   also makes it possible to release a modified version which carries
 *   forward this exception.
 */

#include <pthread.h>


extern pthread_cond_t new_connection_cond;
extern pthread_mutex_t new_connection_mutex;



#ifndef THREAD_VARS
#define THREAD_VARS

#define MAX_THREADS    1
#define MAX_CLIENT_PER_THREAD 30

typedef struct {
	pthread_t tid;
	int client_count;
	int clients[MAX_CLIENT_PER_THREAD];
} Thread;

#endif
