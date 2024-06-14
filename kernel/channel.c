#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"


struct channel {
    int chd;                // channel descriptor - index of the channel in the channel table
    struct spinlock lock;

    // channel->lock must be held when using these:
    int used;               // zero if the channel is unused, one if is used
    int data;               // channel->lock must be held in order to retreive/modify the data
    int data_available;     // '1' if data is availabe. Turns '0' when a process takes it.
    int owner;              // pid of the process created the channel. '0' if there if channel is unused.
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
      ch->used = 0;             // channels are unused in default
      ch->data = 0;             
      ch->data_available = 0;   // data is unavailable in default
      ch->owner = -1;           // there is now process with this pid
      ch->chd = id;             // channel's index in the channels array
      id++;
  }
}

int 
channel_create(void){
    struct channel *ch;
    int descriptor;
    struct proc *p = myproc();

    for(ch = channels; ch < &channels[NCHANNEL]; ch++){

        acquire(&ch->lock);
        if (!ch->used){             // zero if channel is unused
            descriptor = ch->chd;   // the channel is already initialized by it's index
            ch->owner = p->pid;     // current process is the creator of the channel
            ch->used = 1;           // signs that channel is now used

            release(&ch->lock);
            return descriptor;
        }

        release(&ch->lock);
    }

    return -1;      // If reached here then all channels are in use
}

int
channel_put(int cd, int data){
    
    if (cd < 0 || cd >= NCHANNEL) {   // No such channel descriptor exists
        return -1;
    }

    struct channel *ch = channels + cd;     // cd is the index of the desired channel in channels array

    acquire(&ch->lock);
    if (ch->used){              // channel descriptor is valid
        goto valid;
    } else {                    // channel descriptor is invalid
        release(&ch->lock);
        return -1; 
    }

valid:
    while (ch->data_available) {
        sleep(ch, &ch->lock);
    }

    ch->data = data;
    ch->data_available = 1;
    wakeup(ch);
    release(&ch->lock);

    return 0;
}