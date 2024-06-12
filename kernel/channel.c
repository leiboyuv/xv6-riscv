#include "types.h"
#include "param.h"
#include "riscv.h"
#include "spinlock.h"
#include "defs.h"


struct channel {
    struct spinlock lock;
    int data;           // channel->lock must be held in order to retreive/modify the data
    int isavailable;   // '1' if data is availabe. Turns '0' when a process takes it.
    int owner;          // pid of the process created the channel.
};

// Global array of channels 
struct channel channels[NCHANNEL];

// initialize the channel array.
void
channelinit(void){

  struct channel *ch;

  for(ch = channels; ch < &channels[NCHANNEL]; ch++) {
      initlock(&ch->lock, "channel");
      ch->isavailable = 0;
      ch->owner = 0;
  }
}