// ARXEIO DHLWSEWN
#ifndef ERGASIA2                     // IFNOTDEFINE
#define ERGASIA2                     // DEFINE

#define SHM_PATH "makefile"          // DEFINE MAKEFILE

enum operation {LOCK=-1, UNLOCK=1};  // LEITOYRGIA KLEIDWMATOS -1 - XEKLEIDWMATOS 1(THA MAS XREIASTEI STHN PC PM) 

struct shared {                      // DHMIOYRGIA DOMHS GIA APOTHYKEYSH DEDOMENWN - APOTELESMATWN TOY KYKLOU(MESA)
  int size;
  int result;
};


union semun {                        // DHMIOYRGIA DOMHS UNION GIA APOTHYKEYSH DEDOMENWN DIAFORWN TYPWN
   int              val;             // METAVLHTHS INT VAL (SETVAL)
   struct semid_ds *buf;             // DEIKTHS GIA THN IPC_STAT KAI IPC_SET
   unsigned short  *array;           // DEIKTHS PINAKA GIA GETALL, SETALL
   struct seminfo  *_buf;            // DEIKTHS GIA IPC_INFO (Linux)
};
#endif
