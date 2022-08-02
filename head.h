#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sqlite3.h>
#include <signal.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#define PRINT_ERR(msg) do{perror(msg);exit(-1);}while(0)
#define N 16
#define R 1
#define L 2
typedef struct MSG_T{
    int admin;
    int operation;
    int l_flag;
    int salary;
    int age;
    int Modify_options;
    char type;
	char user[N];
	char passwd[16];   // password or word
    char name[20];
    char work[32];
    char meg[128];
}MSG_T;
MSG_T msg;
enum MODER{
    A = 1,
    U = 2,
};
enum {
    ncloumn1 = 8,
    ncloumn2,
    ncloumn3,
    ncloumn4,
    ncloumn5,
    ncloumn6,
    ncloumn7,
    ncloumn8,

};
//服务端
extern void register_ser(int socketfd,sqlite3* db,MSG_T *msg);
extern void login_ser(int socketfd,sqlite3* db,MSG_T *msg);
extern void check_ser(int socketfd, sqlite3 *db, MSG_T *msg);
extern void modify_ser(int socketfd,sqlite3* db,MSG_T *msg);
extern void add_ser(int socketfd,sqlite3* db,MSG_T *msg);
extern void quit_ser(int socketfd,sqlite3* db,MSG_T *msg);
extern void do_delete_ser(int socketfd, sqlite3 *db, MSG_T *msg);
extern void user_check_ser(int socketfd,sqlite3* db,MSG_T *msg);
extern void user_modify_ser(int socketfd,sqlite3 *db,MSG_T *msg);
#define OPEN_MAX 20

//客户端
extern void chose_moder(void);
extern void chose_register_login(void);
extern void admin_menu(void);
extern void user_menu(void);
extern void do_register(int socketfd,MSG_T *msg);
extern int do_login(int socketfd,MSG_T *msg);
extern int do_operations(int socketfd, MSG_T *msg);
extern void do_check(int socketfd,MSG_T *msg);
extern void do_modify(int socketfd,MSG_T *msg);
extern void do_add(int socketfd,MSG_T *msg);
extern int user_do_operations(int socketfd,MSG_T *msg);
extern void do_delete(int socketfd,MSG_T *msg);