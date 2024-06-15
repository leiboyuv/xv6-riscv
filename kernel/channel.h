struct channel {
    int chd;                // channel descriptor - index of the channel in the channel table
    struct spinlock lock;

    // channel->lock must be held when using these:
    int used;               // zero if the channel is unused, one if is used
    int data;               // channel->lock must be held in order to retreive/modify the data
    int data_available;     // '1' if data is availabe. Turns '0' when a process takes it.
    int owner;              // pid of the process created the channel. '0' if there if channel is unused.
};