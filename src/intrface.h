/*

    File: intrface.h

    Copyright (C) 1998-2006 Christophe GRENIER <grenier@cgsecurity.org>
  
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

#define ANALYSE_X	0
#define ANALYSE_Y	5
#define INTER_BAD_PART	10

list_part_t *ask_structure(disk_t *disk_car,list_part_t *list_part, const int verbose, char **current_cmd);
void interface_list(disk_t *disk_car, const int verbose, const int saveheader, const int backup, char **current_cmd);
