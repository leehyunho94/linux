#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>

#define IPC_KEY_FILENAME    "/proc" // 커널에서 관리하는 가상 파일 시스템으로 언제든지 프로세스 정보를 읽을 수 있다
#define IPC_KEY_PROJ_ID      'a'    

struct msgbuf {
    long mtype;
#define MSGSTR_SIZE 256
    char string[MSGSTR_SIZE];
};

void print_usage(const char *progname)
{
    printf("%s (send|recv) mtype\n", progname);
}

static int init_msgq(void) {
    int msgq;
    key_t key;

    key = ftok(IPC_KEY_FILENAME, IPC_KEY_PROJ_ID);
    if(key == -1) {
        perror("ftok()\n");
        return -1;
    }

    // 키 값을 ftok로 생성하는 경우에는 flag로 IPC_CREAT를 넣고, 커널에게 키 값을 요청할 땐 IPC_PRIVATE
    // 0644는 파일 퍼미션
    msgq = msgget(key, IPC_CREAT | 0644);
    if(msgq == -1) {
        perror("msgget()\n");
        return -1;
    }
    
    return 0;
}

static int do_send(long mtype)
{
    int msgq;
    struct msgbuf mbuf;

    msgq = init_msgq();
    if(msgq == -1) {
        perror("init_msgq\n");
        return -1;
    }
   
    memset(&mbuf, 0, sizeof(mbuf));
    mbuf.mtype = mtype; // 메세지를 보낼 때 mtype은 반드시 양수로 설정해야 한다 
    snprintf(mbuf.string, sizeof(mbuf.string),
           "hello world mtype %ld", mtype); 
    if(msgsnd(msgq, &mbuf, sizeof(mbuf.string), 0) == -1) {
        // 보낼 때와 받을 때의 사이즈는 string에만 맞춰주면 된다. mtype은 제외
        // 마지막 flag는IPC_NOWAIT로 메세지 큐가 생기지 않았다면 바로 에러를 출력하게 된다
        perror("msgsnd()\n");
        return -1;  
    } 
    
    return 0;
}

static int do_recv(long mtype)
{
    int ret, msgq;
    struct msgbuf mbuf;

    msgq = init_msgq();
    if(msgq == -1) {
        perror("init_msgq\n");
        return -1;
    }
   
    memset(&mbuf, 0, sizeof(mbuf));
    // 현재는 원하는 mtype을 넣어준다
    ret = msgrcv(msgq, &mbuf, sizeof(mbuf.string), mtype, 0);
    if(ret == -1) {
        perror("msgrcv()\n");
        return -1;
    }
    printf("recv msg : mtype %ld, msg[%s]\n",
            mbuf.mtype, mbuf.string);

    return 0;
}

int main(int argc, char **argv) 
{
    long mtype;

    if(argc < 3) {
        print_usage(argv[0]);
        return -1;
    }
    
    // 10진수로 치환
    mtype = strtol(argv[2], NULL, 10);

    if(!strcmp(argv[1], "send")) {
        if(mtype <= 0) {
            print_usage(argv[0]);
            return -1;
        }
        if(do_send(mtype) == -1) {
            perror("send()\n");
            return -1;
        }
    } else if(!strcmp(argv[1], "recv")) {
        if(do_recv(mtype) == -1) {
            perror("recv()\n");
            return -1;
        }
    } else {   
        print_usage(argv[0]);
        return -1;
    }

    return 0;
}
