#include "/home/linux/xiangmu/hqyj-xiangmu1/head.h"

void register_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char buf[512] = {0};
    char *errors = NULL;
    char **result = NULL;
    int nRow, ncloumn;
    sprintf(buf, "SELECT * FROM user_pswd WHERE user='%s';", msg->user);
    if (SQLITE_OK != sqlite3_get_table(db, buf, &result, &nRow, &ncloumn, &errors))
        printf("errors = %s\n", errors);
    if (nRow == 1)
    {
        strcpy(msg->meg, "failed");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
    else
    {
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "INSERT INTO user_pswd(user,passwd,admin,l_flag,name,age,salary,work) VALUES('%s','%s',%d,0,'%s',0,0,0);", msg->user, msg->passwd, msg->admin,msg->user);
        if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
        {
            printf("errors = %s\n", errors);
            return;
        }
    }
}

void login_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    memset(msg->meg, 0, sizeof(msg->meg));
    char buf[512] = {0};
    char **result, *errors;
    int nRow, ncloumn;
    sprintf(buf, "SELECT * FROM user_pswd WHERE l_flag=0 AND user='%s' AND passwd='%s' AND admin=%d;", msg->user, msg->passwd, msg->admin);
    if (SQLITE_OK != sqlite3_get_table(db, buf, &result, &nRow, &ncloumn, &errors))
    {
        printf("error = %s\n", errors);
        return;
    }
    if (nRow == 1)
    {
        strcpy(msg->meg, "success");
        //必须在发送包之前把l_flagfangda
        msg->l_flag = socketfd;//唯一登陆标志存在l_flag中
        send(socketfd, msg, sizeof(MSG_T), 0);
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "UPDATE user_pswd SET l_flag=%d WHERE user='%s' AND passwd='%s';", socketfd, msg->user, msg->passwd);
        if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
        {
            printf("error = %s\n", errors);
            return;
        }
    }
    else
    {
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}
void check_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char **result, *errors;
    int nRow, ncloumn;
    char buf[256] = {0};
    sprintf(buf, "SELECT * FROM user_pswd WHERE name='%s';", msg->name);
    if (SQLITE_OK != sqlite3_get_table(db, buf, &result, &nRow, &ncloumn, &errors))
    {
        printf("error = %s\n", errors);
        return;
    }
    if (nRow == 1)
    {
        strcpy(msg->user, result[ncloumn1]);
        strcpy(msg->passwd, result[ncloumn2]);
        strcpy(msg->name, result[ncloumn4]);
        msg->age = atoi(result[ncloumn5]);
        msg->salary = atoi(result[ncloumn6]);
        strcpy(msg->work, result[ncloumn7]);
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
    else
    {
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}

void modify_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char **result, *errors;
    int nRow, ncloumn;
    char buf[512] = {0};
    sprintf(buf, "SELECT * FROM user_pswd WHERE name='%s';", msg->name);
    if (SQLITE_OK != sqlite3_get_table(db, buf, &result, &nRow, &ncloumn, &errors))
    {
        printf("error = %s\n", errors);
        return;
    }
    if (nRow == 1)
    {
        strcpy(msg->user, result[ncloumn1]);
        strcpy(msg->passwd, result[ncloumn2]);
        strcpy(msg->name, result[ncloumn4]);
        msg->age = atoi(result[ncloumn5]);
        msg->salary = atoi(result[ncloumn6]);
        strcpy(msg->work, result[ncloumn7]);
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0); //发送反馈
        recv(socketfd, msg, sizeof(MSG_T), 0);
        switch (msg->Modify_options)
        {
        case 1:
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "UPDATE user_pswd SET name='%s' WHERE user='%s';", msg->name, msg->user);
            if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
            {
                printf("error = %s\n", errors);
                return;
            }
            memset(msg->meg, 0, sizeof(msg->meg));
            strcpy(msg->meg, "success");
            send(socketfd, msg, sizeof(MSG_T), 0);
            break;
        case 2:
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "UPDATE user_pswd SET age=%d WHERE user='%s';", msg->age, msg->user);
            if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
            {
                printf("error = %s\n", errors);
                return;
            }
            memset(msg->meg, 0, sizeof(msg->meg));
            strcpy(msg->meg, "success");
            send(socketfd, msg, sizeof(MSG_T), 0);
            break;
        case 3:
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "UPDATE user_pswd SET salary=%d WHERE user='%s';", msg->salary, msg->user);
            if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
            {
                printf("error = %s\n", errors);
                return;
            }
            memset(msg->meg, 0, sizeof(msg->meg));
            strcpy(msg->meg, "success");
            send(socketfd, msg, sizeof(MSG_T), 0);
            break;
        case 4:
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "UPDATE user_pswd SET work='%s' WHERE user='%s';", msg->work, msg->user);
            if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
            {
                printf("error = %s\n", errors);
                return;
            }
            memset(msg->meg, 0, sizeof(msg->meg));
            strcpy(msg->meg, "success");
            send(socketfd, msg, sizeof(MSG_T), 0);
            break;
        case 5:
            memset(buf,0,sizeof(buf));
            sprintf(buf,"UPDATE user_pswd SET passwd='%s' WHERE name='%s';",msg->passwd,msg->name);
            if(SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
            {
                printf("error = %s\n",errors);
                return;
            }
            memset(msg->meg,0,sizeof(msg->meg));
            strcpy(msg->meg,"success");
            send(socketfd,msg,sizeof(MSG_T),0);
            break;
        }
    }
    else
    {
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}

void add_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char buf[512] = {0};
    char *errors;
    sprintf(buf, "INSERT INTO user_pswd(user,passwd,admin,name,age,salary,work,l_flag) VALUES('%s','%s',2,'%s',%d,%d,'%s',0)",
            msg->name, msg->name, msg->name, msg->age, msg->salary, msg->work);
    if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
    {
        printf("error = %s\n", errors);
        memset(msg->meg, 0, sizeof(msg->meg));
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
    else
    {
        memset(msg->meg, 0, sizeof(msg->meg));
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}
void quit_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char buf[512] = {0};
    char *errors;
    printf("l_flag = %d\n",msg->l_flag);
    sprintf(buf, "UPDATE user_pswd SET l_flag=0 WHERE l_flag=%d;",msg->l_flag);
    if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
    {
        printf("error = %s\n", errors);
        memset(msg->meg, 0, sizeof(msg->meg));
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
    else
    {
        memset(msg->meg, 0, sizeof(msg->meg));
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}
void do_delete_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char buf[256] = {0};
    char *errors, **result;
    int nRow, ncloumn;
    sprintf(buf, "SELECT * FROM user_pswd WHERE name='%s';", msg->name);
    if (SQLITE_OK != sqlite3_get_table(db, buf, &result, &nRow, &ncloumn, &errors))
    {
        printf("error = %s\n", errors);
        return;
    }
    if (nRow == 0)
    {
        memset(msg->meg, 0, sizeof(msg->meg));
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
    else
    {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "DELETE FROM user_pswd WHERE name='%s';", msg->name);
        if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &errors))
        {
            printf("error = %s\n", errors);
            memset(msg->meg, 0, sizeof(msg->meg));
            strcpy(msg->meg, "fail");
            send(socketfd, msg, sizeof(MSG_T), 0);
        }
        memset(msg->meg, 0, sizeof(msg->meg));
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}

//普通用户查询
void user_check_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char buf[256] = {0};
    char *errors, **result;
    int nRow, ncloumn;
    sprintf(buf, "SELECT * FROM user_pswd WHERE user='%s';", msg->user);
    if (SQLITE_OK != sqlite3_get_table(db, buf, &result, &nRow, &ncloumn, &errors))
    {
        printf("error = %s\n", errors);
        return;
    }
    if (nRow == 1)
    {
        strcpy(msg->user, result[ncloumn1]);
        strcpy(msg->passwd, result[ncloumn2]);
        strcpy(msg->name, result[ncloumn4]);
        msg->age = atoi(result[ncloumn5]);
        msg->salary = atoi(result[ncloumn6]);
        strcpy(msg->work, result[ncloumn7]);
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
    else
    {
        strcpy(msg->meg, "fail");
        send(socketfd, msg, sizeof(MSG_T), 0);
    }
}

void user_modify_ser(int socketfd, sqlite3 *db, MSG_T *msg)
{
    char buf[256] = {0};
    char *error = NULL;
    switch (msg->Modify_options)
    {
    case 1: //客户端要修改姓名
        sprintf(buf, "UPDATE user_pswd SET name='%s' WHERE user='%s';", msg->name, msg->user);
        if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &error))
        {
            printf("error = %s\n",error);
            return;
        }
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
        break;
    case 2: //客户端要修改年龄
        sprintf(buf, "UPDATE user_pswd SET age=%d WHERE user='%s';", msg->age, msg->user);
        if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &error))
        {
            printf("error = %s\n", error);
            return;
        }
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
        break;
    case 3: //客户端要修改密码
        sprintf(buf, "UPDATE user_pswd SET passwd='%s' WHERE user='%s';", msg->passwd, msg->user);
        if (SQLITE_OK != sqlite3_exec(db, buf, NULL, NULL, &error))
        {
            printf("error = %s\n", error);
            return;
        }
        strcpy(msg->meg, "success");
        send(socketfd, msg, sizeof(MSG_T), 0);
        break;
    }
}