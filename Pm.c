#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "ergasia2.h"

static int mysemop(const int semid, int id, enum operation op){                 // SYNARTHSH LEITOYRGIAS SEMAPHORWN
  struct sembuf sops;

  sops.sem_num = id;                                                             // LEITOYRGIA ME SEMAPHORO 0
  sops.sem_op = op;                                                              // AFXANW KATA 1
  sops.sem_flg = 0;

  if (semop(semid, &sops, 1) == -1) {                                            // ELEGXOS (SYNARTHSH) LEITOYRGIAS SEMAFORWN
     perror("semop");
     return -1;
  }
  return 0;
}

int main(const int argc, char * argv[]){
  int i, shmid, semid;                                                           // DHLWSEIS - ARXIKOPOIHSEIS
  unsigned short semval[2] = {0,0};                                              // KLEIDWMENOI SEMAPHOROI(2)
  struct shared * addr = NULL;
  unsigned int *K, *T, *sizes;
  union semun un;

  if(argc < 3){                                                                  // ELEGXOS ARGS
    fprintf(stderr, "LATHOS XRHSH: ./ergasia2 <size> [ARGS...] <num_tests>\n");
    return EXIT_FAILURE;
  }

  const int num_procs = argc - 2;                                                // PAIDIA
  const int num_tests = atoi(argv[argc-1]);                                      // PLITHOS EPANALYPSEWN

                                                                                 // DESMEYSEIS
  sizes     = (unsigned int*) calloc(num_procs, sizeof(int));                    // DHMIOYRGIA PINAKWN GIA TA MEGETHOI TWN KYKLWN KAI COUNTERS
  K = (unsigned int*) calloc(num_procs, sizeof(int));                            // COUNTER SHMEIWN TOY KYKLOU
  T = (unsigned int*) calloc(num_procs, sizeof(int));                            // COUNTER SYNOLIKWN SHMEIWN POY EXOYN TESTARISTEI GIA AYTO TO MEGETHOS

  for(i=0; i < num_procs; i++){
    sizes[i] = atoi(argv[1 + i]);                                                // STRING TO INT
  }

  srand(getpid());                                                               // ARXIKOPOIHSH SYNARTHSHS RAND() PARAGWGH ARITHMWN
  shmid = shmget(ftok(SHM_PATH, 5566), sizeof(struct shared), IPC_CREAT | 0600); // DHMIOYRGIA - DESMEYSH DIAMOIRAZOMENHS MNHMHS KAI SEMAPHORWN (FILE TO KEY)
  if (shmid == -1){
    perror("shmget");
    return EXIT_FAILURE;
  }

  semid = semget(ftok(SHM_PATH, 5566), 2, IPC_CREAT | 0600);                     // 0600 PERMISIONS DHMIOYRGIA SEMAPHORWN
  if (semid == -1){
    perror("semget");
    return EXIT_FAILURE;
  }

  addr = shmat(shmid, NULL, 0);                                                  // ENWSH DIAMOIARZOMENHS MNHMHS ME THN DIEYTHYNSH MNHMHS
  if (addr == (void *) -1){
    perror("shmat");
    return EXIT_FAILURE;
  }

  un.array = semval;                                                             // ARXIKOPOIHSH SEMAPHOROY 0 ME TIMH 1
  if (semctl(semid, 0, SETALL, un) == -1){
    perror("semctl");
    return EXIT_FAILURE;
  }

  for(i=0; i < num_procs; i++){
    pid_t pid = fork();                                                          // DIMIOYRGIA ANTIGRAFWN
    if(pid == -1){
      perror("fork");
      return EXIT_FAILURE;
    }else if(pid == 0){                                                          // DIERGASIA PAIDI PC(i)
      execl("./Pc", "./Pc", NULL);                                               // EXEC!!!!!!!!!!!!!!!!! -- EKTELESE THN DIERGASIA WS DIERGASIA PAIDI(i)
      perror("execl");
      exit(EXIT_FAILURE);
    }
  }

  for(i=0; i < num_tests; i++){
    const int idx = rand() % num_procs;                                          // KATHORIZW RANDOM SHMEIA
    addr->size   = sizes[idx];                                                   // SETARW THN DIAMOIRAZOMENH MNHMH ME TA APOTELESMATA(SHMEIA)
    addr->result = -1;                                                           // XWRIS APOTELESMA

    if(mysemop(semid, 0, UNLOCK) < 0){                                           // XEKLEIDWMA PC SEMAPHOROY 0
      break;
    }

    if(mysemop(semid, 1, LOCK) < 0){                                             // ANAMONH APOTELESMATWN SEMAPHOROY 1 APO PAIDI PC
      break;
    }

    if(addr->result == 1){                                                       // AN EXOYME APOTELESMATA XEKLEIDWNEI H PM
      K[idx]++;                                                                  // UPDATE TIS METRHSEIS BASH APOTELESMATWN
    }
    T[idx]++;
  }                                                                              // PRWTH EKTYPWSH
  printf("Tests: %d\n", i);

  addr->size = 0;                                                                // APOSTOLH SHMATOS GIA EXODOS APO MAMA SE PAIDI
  for(i=0; i < num_tests; i++){
    mysemop(semid, 0, UNLOCK);                                                   // XEKLEIDWNOYME PC SEMAPHORO 0
  }

  for(i=0; i < num_procs; i++){                                                  // ANAMONH OLOKLHRWSHS APO DIERGASIES PAIDIA
    wait(NULL);                                                                  // ANAMONH MAMAS
  }

  for(i=0; i < num_procs; i++){
    printf("T(%d)=%d ", sizes[i], T[i]);
  }
  printf("\n");

  for(i=0; i < num_procs; i++){
    printf("K(%d)=%d ", sizes[i], K[i]);
  }
  printf("\n");

  for(i=0; i < num_procs; i++){
    const float pi = 4* (float) K[i] / T[i];
    printf("P(%i) = %.2f\n", sizes[i], pi);
  }

  free(sizes);                                                                   // APO DESMEYSEIS
  free(K);
  free(T);

  if (shmctl(shmid, IPC_RMID, NULL) == -1){                                      // APELEYTHERWSH DESMEYOMENHS MNHMHS
    perror("shmctl");
  }

  if (semctl(semid, 0, IPC_RMID, NULL) == -1){                                   // APELEYTHERWSH SEMAPHORWN
    perror("semctl");
  }

  return EXIT_SUCCESS;
}
