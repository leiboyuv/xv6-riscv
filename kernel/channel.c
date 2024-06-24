#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"
#include "proc.h"
#include "channel.h"

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
        if (!ch->used){
            release(&ch->lock);
            return -1;              // woken up since channel_destroy was called on this cd
        }
    }

    ch->data = data;
    ch->data_available = 1;
    wakeup(ch);
    release(&ch->lock);

    return 0;
}

int
channel_take(int cd, uint64 data){

    if (cd < 0 || cd >= NCHANNEL) {   // No such channel descriptor exists
        return -1;
    }

    struct channel* ch = channels + cd;

    acquire(&ch->lock);
    if (!ch->used){                 // channel descriptor is invalid
        release(&ch->lock);
        return -1;
    }

    while (!ch->data_available) {
        sleep(ch, &ch->lock);
        if (!ch->used){
            release(&ch->lock);
            return -1;              // woken up since channel_destroy was called on this cd
        }
    }

    // data is available
    int ret = copyout(myproc()->pagetable, data, (char *)&ch->data, sizeof(int));
    ch->data_available = 0;
    wakeup(ch);
    release(&ch->lock);

    return ret;     // ret is the result of copyout, zero if copied successfully and -1 otherwise

}

int channel_destroy(int cd){

    if (cd < 0 || cd >= NCHANNEL) {   // No such channel descriptor exists
        return -1;
    }

    struct channel* ch = channels + cd;

    acquire(&ch->lock);
    if (!ch->used){                 // channel descriptor is invalid
        release(&ch->lock);
        return -1;
    }

    ch->used = 0;
    ch->owner = -1;
    ch->data_available = 0;
    wakeup(ch);
    release(&ch->lock);

    return 0;
}