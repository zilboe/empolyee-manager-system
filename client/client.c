#include "../head.h"

int main(int argc,char* argv[]){
    int socketfd;
    struct sockaddr_in server_addr;
    MSG_T msg;
    int A_operation;
    int modify_int2;
    int modify_age;
    int modify_salary;
    char modify_work;
    char *modfiy2;
    // if(argc < 3){
    //     printf("Usage:%s <serv_ip> <serv_port>\n",argv[0]);
    //     exit(-1);
    // }
    if(-1 == (socketfd = socket(AF_INET,SOCK_STREAM,0))){
        perror("fail to socket");
        exit(-1);
    }
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(atoi("8888"));

    if(-1 == connect(socketfd,(struct sockaddr *)&server_addr,sizeof(server_addr))){
        perror("fail to connect");
        exit(-1);
    }
    int moder_chose;//登陆注册选项
    char check_scanf[32];
    chose_moder();
    scanf("%d",&msg.admin);
    while(getchar()!='\n');
    switch(msg.admin){
        case A://管理员模式
    num1:
            chose_register_login();//显示界面
            scanf("%d",&moder_chose);//注册还是登陆
            while(getchar()!='\n');
            switch(moder_chose){
                case 1://注册
                    do_register(socketfd,&msg);
                    goto num1;
                    break;
                case 2://登陆
                    if(do_login(socketfd,&msg)){
    num2:
                        admin_menu();//显示登陆之后界面
                        //登陆之后要执行的操作
                        if(5 == do_operations(socketfd,&msg))
                        {
                            goto num1;//账号退出界面返回到登陆之前
                        }
                        goto num2;//一次操作执行完继续执行等待操作
                    }
                    goto num1;//账号密码输入错误继续返回到初始界面
                    break;
            }
            break;
        case U:
    num3:
            chose_register_login();//显示界面
            scanf("%d",&moder_chose);//注册还是登陆
            while(getchar()!='\n');
            switch(moder_chose){
                case 1://注册
                    do_register(socketfd,&msg);
                    goto num3;
                    break;
                case 2://登陆
                    if(do_login(socketfd,&msg)){
    num4:
                        user_menu();//显示登陆之后界面
                        //登陆之后要执行的操作
                        if(3 == user_do_operations(socketfd,&msg))
                        {
                            goto num3;//账号退出界面返回到登陆之前
                        }
                        goto num4;//一次操作执行完继续执行等待操作
                    }
                    goto num3;//账号密码输入错误继续返回到初始界面
                    break;
                }
            break;
    }
    return 0;
}
