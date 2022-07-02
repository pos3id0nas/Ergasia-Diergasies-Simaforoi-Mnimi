#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <math.h>
#include "ergasia2.h"

static int mysemop(const int semid, int id, enum operation op){
  struct sembuf sops;

  sops.sem_num = id;                                              // LEITOYRGIA ME SEMAPHORO 0
  sops.sem_op = op;                                               // AYXISH KATA 1
  sops.sem_flg = 0;

  if (semop(semid, &sops, 1) == -1) {                             // ELEGXOS SEMAPHORWN
     perror("semop");
     return -1;
  }
  return 0;
}

int main(void){
  int shmid, semid;
  struct shared * addr = NULL;

  srand(getpid());                                                // ARXIKOPOIHSH SYNARTHSHS RAND()
  shmid = shmget(ftok(SHM_PATH, 5566), sizeof(struct shared), 0); // DHMIOYRGIA - DESMEYSH DIAMOIRAZOMENHS MNHMHS KAI SEMAPHORWN(Path,Megethos Path,Adeia Prosvasis)
  if (shmid == -1){                                               // ELEGXOS
    perror("shmget");
    return EXIT_FAILURE;
  }

  semid = semget(ftok(SHM_PATH, 5566), 2, 0);                     // PAIRNOYME TO ID THS DIAMOIRAZOMENHS MNHMHS
  if (semid == -1){                                               // ELEGXOS AN PHRAME TO KLEIDI EPITYXWS ALLIWS -1 FAILURE
    perror("semget");
    return EXIT_FAILURE;
  }

  addr = shmat(shmid, NULL, 0);                                   // ENWSH DIAMOIARZOMENHS MNHMHS ME THN DIEYTHYNSH MNHMHS
  if (addr == (void *) -1){
    perror("shmat");
    return EXIT_FAILURE;
  }

  while(1){                                                       // ATERMON VROGXOS
    if(mysemop(semid, 0, LOCK) < 0){                              // ANAMONH AITHMATOS APO THN PM
      break;
    }

    const int size = addr->size;
    if(size == 0){                                                // AN AFIXTHEI SHMA - MHNYMA(0 APO MAMA) TOTE...
      break;                                                      // STOP
    }

    const float x =(float)rand()/((float) RAND_MAX / size);
    const float y =(float)rand()/((float) RAND_MAX / size);

    const float d = sqrt(x*x +y*y);                               // UPDATE APOTELESMATA
    addr->result = (d < size) ? 1 : 0;
    if(mysemop(semid, 1, UNLOCK) < 0){                            // UNLOCK THN PM KAI ANAMONH APOTELESMATOS
      break;
    }
  }
  return EXIT_SUCCESS;
}
