#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"


struct channel {
    struct spinlock lock;
    int chd;           // channel descriptor - index of the channel in the channel table
    int data;          // channel->lock must be held in order to retreive/modify the data
    int isavailable;   // '1' if data is availabe. Turns '0' when a process takes it.
    int owner;         // pid of the process created the channel. '0' if there if channel is unused.
};

// Global array of channels 
struct channel channels[NCHANNEL];

// initialize the channel array.
void
channelinit(void){

  struct channel *ch;
  int id = 0;

  for(ch = channels; ch < &channels[NCHANNEL]; ch++) {
      initlock(&ch->lock, "channel");
      ch->isavailable = 0;
      ch->owner = 0;
      ch->chd = id;
      id++;
  }
}

int 
channel_create(void){
    struct channel *ch;
    int descriptor;
    struct proc *p = myproc();

    for(ch = channels; ch < &channels[NCHANNEL]; ch++){
        if (!ch->owner){            // zero if channel is unused
            descriptor = ch->chd;   // the channel is already initialized by it's index
            ch->owner = p->pid;     // current process is the creator of the channel

            return descriptor;
        }
    }

    return -1;      // If reached here then all channels are in use
}