#include "/home/linux/xiangmu/hqyj-xiangmu1/head.h"
void do_modify_name(MSG_T *msg);
void chose_moder(void)
{
    printf("************员工管理系统**********\n");
    printf("    1.管理员模式       2.普通员工  \n");
    printf("*********************************\n");
}
void chose_register_login(void)
{
    printf("*********************************\n");
    printf("   1.注册      2.登陆      3.退出 \n");
    printf("*********************************\n");
}

void admin_menu(void)
{
    printf("****************管理员模式***********************\n");
    printf("      1.查询  2.修改  3.增加  4.删除  5.退出      \n");
    printf("************************************************\n");
    printf("  请输入要执行的操作(非正常退出会导致账号无法登陆)  \n");
    printf("*************************************************\n");
}

void user_menu(void)
{
    printf("*****************普通模式*******************\n");
    printf("        1.查询      2.修改       3退出      \n");
    printf("*******************************************\n");
    printf("请输入要执行的操作(非正常退出会导致账号无法登陆)\n");
    printf("*******************************************\n");
}
void do_register(int socketfd, MSG_T *msg)
{
    msg->type = 'R';
    printf("请输入账号：");
    scanf("%s", msg->user);
    while (getchar() != '\n')
        ;
    printf("请输入密码：");
    scanf("%s", msg->passwd);
    while (getchar() != '\n')
        ;
    send(socketfd, msg, sizeof(MSG_T), 0); //发送账号密码到服务器
    recv(socketfd, msg, sizeof(MSG_T), 0);
    if (strcmp(msg->meg, "success") == 0)
    {
        printf("注册成功\n");
    }
    else
    {
        printf("注册失败,用户名已存在\n");
    }
}

int do_login(int socketfd, MSG_T *msg)
{
    msg->type = 'L';
    printf("请输入账号：");
    scanf("%s", msg->user);
    while (getchar() != '\n')
        ;
    printf("请输入密码：");
    scanf("%s", msg->passwd);
    while (getchar() != '\n')
        ;
    send(socketfd, msg, sizeof(MSG_T), 0); //发送账号密码到服务器
    recv(socketfd, msg, sizeof(MSG_T), 0);
    if (strcmp(msg->meg, "success") == 0)
    {
        printf("登陆成功\n");
        return 1;
    }
    else
    {
        printf("登陆失败,用户名或密码错误或已经登陆\n");
        return 0;
    }
}

void do_check(int socketfd, MSG_T *msg)
{
    printf("请输入你要查询的姓名:\n");
    scanf("%s", msg->name);
    while (getchar() != '\n')
        ;
    send(socketfd, msg, sizeof(MSG_T), 0);
    recv(socketfd, msg, sizeof(MSG_T), 0);
    if (strcmp(msg->meg, "success") == 0)
    {
        printf("查询成功\n");
        printf("账号:%s,密码:%s,姓名:%s,年龄:%d岁,工资:%d元,工作:%s\n", msg->user,
               msg->passwd, msg->name, msg->age, msg->salary, msg->work);
    }
    else
    {
        printf("查询失败,请检查姓名是否正确\n");
        return;
    }
}

void do_modify(int socketfd, MSG_T *msg)
{
    printf("请输入你要修改信息的姓名：\n");
    scanf("%s", msg->name);
    send(socketfd, msg, sizeof(MSG_T), 0);
    recv(socketfd, msg, sizeof(MSG_T), 0);//已经接收来自
    if (strcmp(msg->meg, "success") == 0)
    {
        printf("姓名:%s,年龄:%d岁,工资:%d元,工作:%s\n",
               msg->name, msg->age, msg->salary, msg->work);
        printf("*****************************************\n");
        printf("            请选择你要修改的选项:          \n");
        printf(" 1.姓名 2.年龄 3.工资 4.工作 5.密码 6.退出  \n");
        scanf("%d", &msg->Modify_options);
        switch (msg->Modify_options)
        {
        case 1: //姓名
            printf("请输入你要修改的名字:");
            memset(msg->name, 0, sizeof(msg->name));
            scanf("%s", msg->name);
            while (getchar() != '\n');
            break;
        case 2: //年龄
            printf("请输入你要修改的年龄:");
            scanf("%d",&msg->age);
            while(getchar()!='\n');
            break;
        case 3: //工资
            printf("请输入你要修改的工资:");
            scanf("%d",&msg->salary);
            while(getchar()!='\n');
            break;
        case 4: //工作
            printf("请输入你要修改的工作:");
            memset(msg->work,0,sizeof(msg->work));
            scanf("%s",msg->work);
            while(getchar()!='\n');
            break;
        case 5:
            printf("请输入你要修改的密码:");
            memset(msg->passwd,0,sizeof(msg->passwd));
            scanf("%s",msg->passwd);
            while(getchar()!='\n');
            break;
        case 6:
            return;
            break;
        }
        send(socketfd, msg, sizeof(MSG_T), 0);
        recv(socketfd, msg, sizeof(MSG_T), 0);
        if (strcmp(msg->meg, "success") == 0)
        {
            printf("修改成功\n");
        }
        else
        {
            printf("修改失败\n");
        }
    }
    else
    {
        printf("姓名不存在\n");
        return;
    }
}

void do_add(int socketfd,MSG_T *msg)
{
    printf("请输入你要添加的姓名:");
    scanf("%s",msg->name);
    while(getchar()!='\n');
    printf("请输入添加的年龄:");
    scanf("%d",&msg->age);
    while(getchar()!='\n');
    printf("请输入要添加的工资:");
    scanf("%d",&msg->salary);
    while(getchar()!='\n');
    printf("请输入要添加的工作:");
    scanf("%s",msg->work);
    while(getchar()!='\n');
    send(socketfd,msg,sizeof(MSG_T),0);
    recv(socketfd,msg,sizeof(MSG_T),0);
    if(strcmp(msg->meg,"success") == 0){
        printf("添加成功\n");
        printf("新用户账号密码为姓名\n");
    }else{
        printf("添加失败\n");
    }
}

void do_delete(int socketfd,MSG_T *msg)
{
    printf("请输入你要删除信息的姓名:");
    scanf("%s",msg->name);
    while(getchar()!='\n');
    send(socketfd,msg,sizeof(MSG_T),0);
    recv(socketfd,msg,sizeof(MSG_T),0);
    if(strcmp(msg->meg,"success") == 0){
        printf("删除成功\n");
    }
    else{
        printf("删除失败\n");
    }
}

void do_quit(int socketfd,MSG_T *msg)
{
    send(socketfd,msg,sizeof(MSG_T),0);
    recv(socketfd,msg,sizeof(MSG_T),0);
    if(strcmp(msg->meg,"success") == 0){
        printf("成功退出\n");
    }else{
        printf("退出失败\n");
    }
}
int do_operations(int socketfd, MSG_T *msg)
{
    int operation;
    scanf("%d", &operation);
    if (operation == 1)
        msg->type = 'C';
    if (operation == 2)
        msg->type = 'M';
    if (operation == 3)
        msg->type = 'A';
    if (operation == 4)
        msg->type = 'D';
    if (operation == 5)
        msg->type = 'Q';
    switch (msg->type)
    {
    case 'C':
        do_check(socketfd, msg);
        break;
    case 'M':
        do_modify(socketfd, msg);
        break;
    case 'A':
        do_add(socketfd,msg);
        break;
    case 'D':
        do_delete(socketfd,msg);
        break;
    case 'Q':
        do_quit(socketfd,msg);
        return 5;
        break;
    }
    return 0;
}

//普通用户查询功能
void user_check(int socketfd,MSG_T *msg)
{
    send(socketfd,msg,sizeof(MSG_T),0);
    recv(socketfd,msg,sizeof(MSG_T),0);
    if(strcmp(msg->meg,"success") == 0){
        printf("查询成功\n");
        printf("账号:%s,密码:%s,姓名:%s,年龄:%d岁,工资:%d元,工作:%s\n",
        msg->user,msg->passwd,msg->name,msg->age,msg->salary,msg->work);
    }
    else{
        printf("查询失败\n");
    }
}

int  user_modify(int socketfd,MSG_T* msg)
{
    int operations;
    printf("************普通用户*************\n");
    printf("  1.姓名  2.年龄  3.密码  4.返回  \n");
    printf("        请选择要执行的选项：      \n");
    scanf("%d",&operations);
    msg->Modify_options = operations;
    switch(operations){
        case 1://姓名
            printf("请输入要修改的名字:");
            scanf("%s",msg->name);
            while(getchar()!='\n');
            break;
        case 2://年龄
            printf("请输入要修改的年龄:");
            scanf("%d",&msg->age);
            while(getchar()!='\n');
            break;
        case 3://密码
            printf("请输入要修改的密码:");
            scanf("%s",msg->passwd);
            break;
        case 4:
            return 4;
            break;
    }
    send(socketfd,msg,sizeof(MSG_T),0);
    recv(socketfd,msg,sizeof(MSG_T),0);
    if(strcmp(msg->meg,"success") == 0){
        printf("修改成功\n");
    }else{
        printf("修改失败\n");
    }

}

//普通用户退出账户
void user_quit(int socketfd,MSG_T *msg)
{
    send(socketfd,msg,sizeof(MSG_T),0);
    recv(socketfd,msg,sizeof(MSG_T),0);
    if(strcmp(msg->meg,"success") == 0){
        printf("成功退出\n");
    }else{
        printf("退出失败\n");
    }
}
//普通用户的操作
int user_do_operations(int socketfd,MSG_T *msg)
{
    int operations;
    scanf("%d",&operations);
    if(operations == 1)
        msg->type = 'C';
    if(operations == 2)
        msg->type = 'M';
    if(operations == 3)
        msg->type = 'Q';
    switch(msg->type){
        case 'C':
            user_check(socketfd,msg);
            break;
        case 'M':
            user_modify(socketfd,msg);
            break;
        case 'Q':
            user_quit(socketfd,msg);
            return 3;
            break;
    }
}
