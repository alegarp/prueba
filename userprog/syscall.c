#include "userprog/syscall.h"
#include "userprog/process.h"
#include "userprog/pagedir.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "threads/init.h"
#include "devices/input.h"
#include <list.h>
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "filesys/inode.h"

static void syscall_handler (struct intr_frame *);
static void exit(int status);
static bool is_valid(void *addr);
static struct file * search_file(int fd);
struct lock lock_filesys;

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
  lock_init(&lock_filesys);
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
  if(!is_valid(f->esp))
  	exit(-1);

  int syscall_code = *(int*)f->esp;
  if(syscall_code == SYS_HALT)
  {
  	shutdown_power_off();
  }
}


static void exit(int status)
{
	printf("%s: exit(%d)\n", thread_current()->name, status);
  	thread_current()->mis_datos->return_state = status;
  	thread_exit();
}

	static bool is_valid(void *addr)
{
	bool ret = true;
	for(int i = 0; i < 4; i++)
	{
		if (!is_user_vaddr(addr))
		{
			ret = false;
			break;
		}
		else
			if(pagedir_get_page(thread_current()->pagedir, addr) == NULL)
			{
				ret = false;
				break;
			}
		addr++;
	}
	return ret;
}


