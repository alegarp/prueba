#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{

  int syscall_code = *(int*)f->esp;
  if (syscall_code == SYS_HALT){
    shutdown_power_off();
  }
  else if(syscall_code ==  SYS_WAIT){
    tid_t pid = (tid_t)(*((int*)f->esp +1));
    f->eax = process_wait(pid);

  }



  printf ("system call!\n");
  thread_exit ();
}

static void exit(int status){
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}

