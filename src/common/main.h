/*
   Copyright 2005-2010 Jakub Kruszona-Zawadzki, Gemius SA, 2013-2014 EditShare, 2013-2015 Skytechnology sp. z o.o..

   This file was part of MooseFS and is part of LizardFS.

   LizardFS is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, version 3.

   LizardFS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with LizardFS  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "common/platform.h"

#include <inttypes.h>
#include <poll.h>
#include <string>
#include <vector>

#include "common/case_sensitivity.h"

#define TIMEMODE_SKIP_LATE 0
#define TIMEMODE_RUN_LATE 1

/*! \brief Returns additional command line arguments.
 *
 * Additional command line arguments can be passed using '-o extra_option' syntax.
 */
const std::vector<std::string>& main_get_extra_arguments();

/*! \brief Returns true if additional command line argument is present.
 *
 * Additional command line arguments can be passed using '-o extra_option' syntax.
 */
bool main_has_extra_argument(std::string name, CaseSensitivity mode = CaseSensitivity::kSensitive);

void main_destructregister (void (*fun)(void));
void main_canexitregister (int (*fun)(void));
void main_wantexitregister (void (*fun)(void));
void main_reloadregister (void (*fun)(void));
void main_pollregister (void (*desc)(std::vector<pollfd>&),void (*serve)(const std::vector<pollfd>&));
void main_eachloopregister (void (*fun)(void));
void* main_timeregister (int mode,uint32_t seconds,uint32_t offset,void (*fun)(void));

/*! \brief Make the next poll nonblocking
 */
void main_make_next_poll_nonblocking();

/*! \brief Unregister previously registered timed event handler.
 *
 * \param handle - handle to currently registered timed event.
 */
void main_timeunregister (void* handle);
int main_timechange(void *x,int mode,uint32_t seconds,uint32_t offset);
uint32_t main_time(void);
uint64_t main_utime(void);

/*! \brief Try to exit as if term signal was received.
 *
 * \return STATUS_OK if term sequence has been initialized
 *         ERROR_NOTPOSSIBLE if it was already initialized.
 */
uint8_t main_want_to_terminate(void);

/*! \brief Request reloading the configuration.
 *
 * Reload will be performed after the current loop, so this function returns
 * before thereload actually happens.
 */
void main_want_to_reload(void);
