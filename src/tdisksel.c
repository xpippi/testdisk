/*

    File: tdisksel.c

    Copyright (C) 1998-2008 Christophe GRENIER <grenier@cgsecurity.org>
  
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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
 
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include "types.h"
#include "common.h"
#include "intrf.h"
#ifdef HAVE_NCURSES
#include "intrfn.h"
#else
#include <stdio.h>
#endif
#include "fnctdsk.h"
#include "testdisk.h"
#include "partauto.h"
#include "log.h"
#include "hdaccess.h"
#include "diskcapa.h"
#include "diskacc.h"
#include "tdiskop.h"
#include "tdisksel.h"

#ifdef HAVE_NCURSES
#define NBR_DISK_MAX 10
static int testdisk_disk_selection_ncurses(int verbose,int dump_ind, const list_disk_t *list_disk, const int saveheader, char **current_cmd)
{
  int command='Q';
  unsigned int menu=0;
  int offset=0;
  int pos_num=0;
  const list_disk_t *element_disk;
  const list_disk_t *current_disk;
  static struct MenuItem menuMain[]=
  {
    { 'P', "Previous",""},
    { 'N', "Next","" },
    { 'O',"Proceed",""},
    { 'Q',"Quit","Quit program"},
    { 0,NULL,NULL}
  };
  current_disk=list_disk;
  if(current_disk==NULL)
  {
    return intrf_no_disk("TestDisk");
  }
    /* ncurses interface */
  while(1)
  {
    const char *options;
    int i;
#ifdef HAVE_NCURSES
    aff_copy(stdscr);
    wmove(stdscr,4,0);
    wprintw(stdscr,"  TestDisk is free software, and");
    wmove(stdscr,5,0);
    wprintw(stdscr,"comes with ABSOLUTELY NO WARRANTY.");
    wmove(stdscr,7,0);
    wprintw(stdscr,"Select a media (use Arrow keys, then press Enter):");
#endif
    for(i=0,element_disk=list_disk;(element_disk!=NULL) && (i<offset);element_disk=element_disk->next,i++);
    for(;element_disk!=NULL && (i-offset)<NBR_DISK_MAX;i++,element_disk=element_disk->next)
    {
      wmove(stdscr,8+i-offset,0);
      if(element_disk!=current_disk)
	wprintw(stdscr,"%s\n",element_disk->disk->description_short(element_disk->disk));
      else
      {
	wattrset(stdscr, A_REVERSE);
	wprintw(stdscr,"%s\n",element_disk->disk->description_short(element_disk->disk));
	wattroff(stdscr, A_REVERSE);
      }
    }
    if(i<=NBR_DISK_MAX && element_disk==NULL)
      options="OQ";
    else
      options="PNOQ";
    {
      int line=20;
      mvwaddstr(stdscr,line++,0,"Note: ");
#if defined(__CYGWIN__) || defined(__MINGW32__) || defined(DJGPP)
#else
#ifdef HAVE_GETEUID
      if(geteuid()!=0)
      {
        if(has_colors())
          wbkgdset(stdscr,' ' | A_BOLD | COLOR_PAIR(1));
        waddstr(stdscr,"Some disks won't appear unless you are root user.");
        if(has_colors())
          wbkgdset(stdscr,' ' | COLOR_PAIR(0));
        wmove(stdscr,line++,0);
      }
#endif
#endif
      waddstr(stdscr,"Disk capacity must be correctly detected for a successful recovery.");
      wmove(stdscr,line++,0);
      wprintw(stdscr,"If a disk listed above has incorrect size, check HD jumper settings, BIOS");
      wmove(stdscr,line++,0);
      wprintw(stdscr,"detection, and install the latest OS patches and disk drivers."); 
    }
    command = wmenuSelect_ext(stdscr, LINES-1, INTER_MAIN_Y, INTER_MAIN_X, menuMain, 8,
	options, MENU_HORIZ | MENU_BUTTON | MENU_ACCEPT_OTHERS, &menu,NULL);
    switch(command)
    {
      case 'p':
      case 'P':
      case KEY_UP:
	if(current_disk->prev!=NULL)
	{
	  current_disk=current_disk->prev;
	  pos_num--;
	}
	break;
      case 'n':
      case 'N':
      case KEY_DOWN:
	if(current_disk->next!=NULL)
	{
	  current_disk=current_disk->next;
	  pos_num++;
	}
	break;
      case KEY_PPAGE:
	for(i=0;i<NBR_DISK_MAX-1 && current_disk->prev!=NULL;i++)
	{
	  current_disk=current_disk->prev;
	  pos_num--;
	}
	break;
      case KEY_NPAGE:
	for(i=0;i<NBR_DISK_MAX-1 && current_disk->next!=NULL;i++)
	{
	  current_disk=current_disk->next;
	  pos_num++;
	}
	break;
      case 'o':
      case 'O':
	{
	  disk_t *disk=current_disk->disk;
	  autodetect_arch(disk);
	  autoset_unit(disk);
	  if(interface_check_disk_capacity(disk)==0 &&
              interface_check_disk_access(disk, current_cmd)==0 &&
	      interface_partition_type(disk, verbose, current_cmd)==0)
	  {
	    if(menu_disk(disk, verbose, dump_ind, saveheader, current_cmd))
	      return 0;
	  }
	}
	break;
      case 'q':
      case 'Q':
	return 0;
    }
    if(pos_num<offset)
      offset=pos_num;
    if(pos_num>=offset+NBR_DISK_MAX)
      offset=pos_num-NBR_DISK_MAX+1;
  }
}
#endif

static int testdisk_disk_selection_cli(int verbose,int dump_ind, const list_disk_t *list_disk, const int saveheader, const char *cmd_device, char **current_cmd)
{
  const list_disk_t *element_disk;
  const list_disk_t *current_disk=NULL;
  if(cmd_device!=NULL)
  {
    for(element_disk=list_disk;element_disk!=NULL;element_disk=element_disk->next)
    {
      if(strcmp(element_disk->disk->device,cmd_device)==0)
	current_disk=element_disk;
    }
  }
  else
    current_disk=list_disk;
  if(current_disk==NULL)
  {
    return intrf_no_disk("TestDisk");
  }
  if(*current_cmd!=NULL)
  {
    while(*current_cmd[0]==',')
      (*current_cmd)++;
    {
      disk_t *disk=current_disk->disk;
      autodetect_arch(disk);
      autoset_unit(disk);
      if(interface_check_disk_capacity(disk)==0 &&
          interface_check_disk_access(disk, current_cmd)==0 &&
          interface_partition_type(disk, verbose, current_cmd)==0)
      {
	menu_disk(disk, verbose, dump_ind, saveheader, current_cmd);
      }
    }
  }
  return 0;
}

int do_curses_testdisk(int verbose,int dump_ind, const list_disk_t *list_disk, const int saveheader, const char *cmd_device, char **current_cmd)
{
  if(*current_cmd!=NULL)
    return testdisk_disk_selection_cli(verbose, dump_ind, list_disk, saveheader, cmd_device, current_cmd);
#ifdef HAVE_NCURSES
  return testdisk_disk_selection_ncurses(verbose, dump_ind, list_disk, saveheader, current_cmd);
#else
  return 0;
#endif
}
