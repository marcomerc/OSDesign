#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"
struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {

//you write thisi
uint addre = 0;
int found = 0;
int i,fr = 0;

uint va = PGROUNDUP(myproc()->sz);
acquire(&(shm_table.lock));

for (i = 0; i< 64; i++) {
   if(shm_table.shm_pages[i].id == id){
    addre = (uint) shm_table.shm_pages[i].frame;
    shm_table.shm_pages[i].refcnt = shm_table.shm_pages[i].refcnt+1 ;
    found = 1;
    }if (shm_table.shm_pages[i].refcnt == 0){
      fr = i;
    }
}
release(&(shm_table.lock));
if(found){
   if( mappages(myproc()->pgdir,(void*) va ,PGSIZE,V2P( addre ), PTE_W | PTE_U ) != 0)
      panic("mmpages error");
    
    *pointer = (char*) myproc()->sz;
    myproc()->sz = myproc()->sz + PGSIZE;
}else{
    shm_table.shm_pages[fr].id = id;
    shm_table.shm_pages[fr].frame = kalloc();
memset(shm_table.shm_pages[fr].frame, 0, PGSIZE);   
 if( mappages(myproc()->pgdir,(void*) va,PGSIZE,V2P( shm_table.shm_pages[fr].frame ), PTE_W | PTE_U ) != 0)
      panic("mmpages error");
    shm_table.shm_pages[fr].refcnt =1;
    *pointer = (char*) myproc()->sz;
    myproc()->sz = myproc()->sz + PGSIZE;
}
return 0; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!
int i;
acquire(&(shm_table.lock));
for (i = 0; i< 64; i++) {
   if( shm_table.shm_pages[i].id == id && shm_table.shm_pages[i].refcnt ==1 ){
       shm_table.shm_pages[i].id =0;
       shm_table.shm_pages[i].frame =0;
       shm_table.shm_pages[i].refcnt =0;
   }else{
       shm_table.shm_pages[i].refcnt=  shm_table.shm_pages[i].refcnt -1;
   }
}
release(&(shm_table.lock));
return 0; //added to remove compiler warning -- you should decide what to return
}
