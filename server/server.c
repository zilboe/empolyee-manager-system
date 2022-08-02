#include "../head.h"

MSG_T msg;
int main(int argc, char *argv[])
{
    int socketfd, acceptfd;
    int epfd, ret, r;
    int nRow, ncloumn;
    int operations = 0;
    char buf[1024] = {0};
    struct epoll_event event;
    struct epoll_event revents[OPEN_MAX];
    struct sockaddr_in server_addr;
    ssize_t res, nready;
    pid_t pid;
    sqlite3 *db;
    if (sqlite3_open("./server/hqyj.db", &db) != SQLITE_OK)
    {
        printf("open hqyj.db error = %s\n", sqlite3_errmsg(db));
        exit(-1);
    }
    // user_pswd
    // user  passwd  admin   name   age   salary   work  l_flag

    // if (argc < 3)
    // {
    //     printf("Usage:%s <ip> <port>\n", argv[0]);
    //     exit(-1);
    // }

    if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        PRINT_ERR("fail to socket");
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(atoi("8888"));

    if (bind(socketfd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
        PRINT_ERR("fail to bind");

    if (listen(socketfd, 5) < 0)
        PRINT_ERR("fail to listen");
    if ((epfd = epoll_create(10)) == -1)
    {
        PRINT_ERR("epoll create error");
    }
    event.events = EPOLLIN;
    event.data.fd = socketfd;
    if (epoll_ctl(epfd, EPOLL_CTL_ADD, socketfd, &event))
    {
        PRINT_ERR("epoll ctl error");
    }
    char buf1[1024] = {0};
    while (1)
    {
        ret = epoll_wait(epfd, revents, OPEN_MAX, -1);
        // printf("%d\n",ret);
        for (int i = 0; i < ret; i++)
        {
            if (revents[i].events & EPOLLIN)
            {
                if (revents[i].data.fd == socketfd)
                {
                    acceptfd = accept(revents[i].data.fd, NULL, NULL);
                    event.events = EPOLLIN;
                    event.data.fd = acceptfd;
                    epoll_ctl(epfd, EPOLL_CTL_ADD, acceptfd, &event);
                }
                else
                {
                    r = recv(revents[i].data.fd, &msg, sizeof(MSG_T), 0);
                    if (0 == r)
                    {
                        printf("客户端id:%d退出\n",revents[i].data.fd);
                        epoll_ctl(epfd, EPOLL_CTL_DEL, revents[i].data.fd, &event);
                    }
                    else{
                        switch(msg.type){
                            case 'R':
                                register_ser(revents[i].data.fd,db,&msg);
                                break;
                            case 'L':
                                login_ser(revents[i].data.fd,db,&msg);
                                break;
                            case 'C':
                                if(msg.admin == A){//如果是管理员的化
                                    check_ser(revents[i].data.fd,db,&msg);
                                }
                                else{//普通用户
                                    user_check_ser(revents[i].data.fd,db,&msg);
                                }
                                break;
                            case 'M':
                                if(msg.admin == A){
                                    modify_ser(revents[i].data.fd,db,&msg);
                                }
                                else{
                                    user_modify_ser(revents[i].data.fd,db,&msg);
                                }
                                break;
                            case 'A':
                                add_ser(revents[i].data.fd,db,&msg);
                                break;
                            case 'D':
                                do_delete_ser(revents[i].data.fd,db,&msg);
                                break;
                            case 'Q':
                                quit_ser(revents[i].data.fd,db,&msg);
                                break;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
