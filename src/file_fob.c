/*

    File: file_fob.c

    Copyright (C) 2008 Christophe GRENIER <grenier@cgsecurity.org>
  
    This software is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License along
    with this program; if not, write the Free Software Foundation, Inc., 51
    Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */

#if !defined(SINGLE_FORMAT) || defined(SINGLE_FORMAT_fob)
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <stdio.h>
#include "types.h"
#include "filegen.h"
#include "memmem.h"

/*@ requires valid_register_header_check(file_stat); */
static void register_header_check_fob(file_stat_t *file_stat);

const file_hint_t file_hint_fob= {
  .extension="fob",
  .description="Microsoft Dynamics NAV (MS Navision)",
  .max_filesize=PHOTOREC_MAX_FILE_SIZE,
  .recover=1,
  .enable_by_default=1,
  .register_header_check=&register_header_check_fob
};

/*@
  @ requires separation: \separated(&file_hint_fob, buffer+(..), file_recovery, file_recovery_new);
  @ requires valid_header_check_param(buffer, buffer_size, safe_header_only, file_recovery, file_recovery_new);
  @ ensures  valid_header_check_result(\result, file_recovery_new);
  @ assigns  *file_recovery_new;
  @*/
static int header_check_fob(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int safe_header_only, const file_recovery_t *file_recovery, file_recovery_t *file_recovery_new)
{
  static const unsigned char sign_navnl[5]	= {'N','A','V','N','L'};
  static const unsigned char sign_navw[4]	= {'N','A','V','W'};
  const char *sbuffer=(const char *)buffer;
  unsigned int tmp=0;
  const char *pos1=(const char *)td_memmem(buffer, buffer_size, sign_navnl, sizeof(sign_navnl));
  const char *pos2=(const char *)td_memmem(buffer, buffer_size, sign_navw, sizeof(sign_navw));
  if(pos1==NULL && pos2==NULL)
    return 0;
  if(pos1!=NULL)
    tmp=pos1-sbuffer;
  if(pos2!=NULL && pos2-sbuffer > tmp)
    tmp=pos2-sbuffer;
  reset_file_recovery(file_recovery_new);
  file_recovery_new->extension=file_hint_fob.extension;
  file_recovery_new->min_filesize=tmp;
  return 1;
}

static void register_header_check_fob(file_stat_t *file_stat)
{
  register_header_check(0, "Codeunit ",  	 9, &header_check_fob, file_stat);
#ifndef DISABLED_FOR_FRAMAC
  register_header_check(0, "Dataport ",  	 9, &header_check_fob, file_stat);
  register_header_check(0, "Form ",		 5, &header_check_fob, file_stat);
  register_header_check(0, "MenuSuite ",	10, &header_check_fob, file_stat);
  register_header_check(0, "Report ",		 7, &header_check_fob, file_stat);
  register_header_check(0, "Table ",		 6, &header_check_fob, file_stat);
  register_header_check(0, "XMLport ",		 8, &header_check_fob, file_stat);
#endif
}
#endif
