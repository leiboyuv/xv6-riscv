//added for task2
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int isPrime(int num){
  int prime = 1;
  if(num < 2)
    prime = 0;
  for(int i = 2; i * i <= num && prime; i++){
    if(num % i == 0){
      prime = 0;
    }
  }
  return prime;
}

void generateNumbers(int channel1){
  int counter = 1;
  while(1){
    if(channel_put(channel1, counter) == -1){
      break;
    }
    counter++;
  }
}
void checkNumbers(int channel1, int channel2){
  int data;
  while(1){
    if(channel_take(channel1, &data) == -1){
      break;
    }
    if(isPrime(data)){
      if(channel_put(channel2, data) == -1){
        channel_destroy(channel1);
        break;
      }
    }
  }
}

void printNumbers(int channel2){
  int counter = 0;
  int data;
  while(counter != 100){
    if(channel_take(channel2, &data) == -1){
      break;
    }
    printf("%d\n",  data);
    counter++;
  }  
  channel_destroy(channel2);
}

int main(int argc, char *argv[])
{
  int checkersCount = 0;
  if(argc < 2){
    checkersCount = 3;
  }
  else {
    checkersCount = atoi(argv[1]);
  }
  int channel1 = channel_create();
  int channel2 = channel_create();
  if(channel1 == -1 || channel2 == -1){
    exit(1);
  }
  if(fork() == 0){
    printNumbers(channel2);
    exit(0);
  }
  for(int i = 0; i < checkersCount; i++){
    if(fork() == 0){
      checkNumbers(channel1, channel2);
      exit(0);
    }
  }
  generateNumbers(channel1);
  exit(0);
}
