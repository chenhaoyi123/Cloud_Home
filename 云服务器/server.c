#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BACKLOG 		5     	//完成三次握手但没有accept的队列的长度
#define CONCURRENT_MAX 	8   	//应用层可以同时处理的连接数
#define SERVER_PORT 	11332
#define BUFFER_SIZE 	1024
#define QUIT_CMD 		".quit"

int client_fds[CONCURRENT_MAX];

//守护进程函数
int daemon_Init(void)
{
	//1.屏蔽终端信号
	//2.忽略标准输入输出
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
	
	//3.在后台运行(创建进程)
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork");
		return -1;
	}
	else if(pid > 0)//父进程(爷爷)
	{
		exit(0);
	}
	//4.脱离终端控制（以下程序由子进程执行）
	setsid();
	
	//5.禁止进程重新打开控制终端
	pid = fork();
	if(pid < 0)
	{
		perror("fork2");
		exit(0);
	}
	else if(pid >0)//父进程（父亲）
	{
		exit(0);//剩下孙子
	}
	//6.关闭打开的文件描述符
	//备注:<sys/param.h> 文件描述符最大个数
	int i_1 = 0;
	for(; i_1 < NOFILE; i_1++)
	{ 
		close(i_1);
	}
	//7.改变当前工作目录
	chdir("tmp");
	//8.重设文件掩码
	umask(0);
	//处理 SIGCHLD 忽略该信号
	signal(SIGCHLD,SIG_IGN);
}

void usage(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage：%s <PORT>\n", argv[0]);
        exit(0);
    }
}

int main(int argc, char *argv[])
{
	int i;
	int index = -1;	//下标
    char input_msg[BUFFER_SIZE];
    char recv_msg[BUFFER_SIZE];	
	char rev_net[1024] = {0};//保存IP地址到数组中
	
	usage(argc, argv);	//判断参数个数

	daemon_Init();	//调用守护进程函数

    //本地地址
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[1]));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bzero(&(server_addr.sin_zero), 8);
	
    //创建socket
    int server_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock_fd == -1)
    {
    	perror("socket error");
    	return 1;
    }
    //绑定socket
    int bind_result = bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(bind_result == -1)
    {
    	perror("bind error");
    	return 1;
    }
    //listen
    if(listen(server_sock_fd, BACKLOG) == -1)
    {
    	perror("listen error");
    	return 1;
    }
	
    //使用Select函数中的fd_set
    fd_set server_fd_set;
    int max_fd = -1;
    struct timeval tv;     //超时时间设置

    while(1)
    {
    	tv.tv_sec = 20;		//秒
    	tv.tv_usec = 0;		//微妙
    	FD_ZERO(&server_fd_set);	//清空
    	FD_SET(STDIN_FILENO, &server_fd_set);
    	if(max_fd <STDIN_FILENO)
    	{
    		max_fd = STDIN_FILENO;
    	}
        //printf("STDIN_FILENO=%d\n", STDIN_FILENO);
		
		//服务器端socket
        FD_SET(server_sock_fd, &server_fd_set);
		// printf("server_sock_fd=%d\n", server_sock_fd);
        if(max_fd < server_sock_fd)
        {
        	max_fd = server_sock_fd;
        }
		
		//客户端连接
        for(i =0; i < CONCURRENT_MAX; i++)
        {
        	//printf("client_fds[%d]=%d\n", i, client_fds[i]);
        	if(client_fds[i] != 0)
        	{
        		FD_SET(client_fds[i], &server_fd_set);
        		if(max_fd < client_fds[i])
        		{
        			max_fd = client_fds[i];
        		}
        	}
        }
		
        int ret = select(max_fd + 1, &server_fd_set, NULL, NULL, &tv);
        if(ret < 0)
        {
        	perror("select 出错\n");
        	continue;
        }
        else if(ret == 0)
        {
        	printf("select 超时\n");
        	continue;
        }
        else
        {
        	//ret 为未状态发生变化的文件描述符的个数
        	if(FD_ISSET(STDIN_FILENO, &server_fd_set))
        	{
        		printf("发送消息：\n");
        		bzero(input_msg, BUFFER_SIZE);
        		fgets(input_msg, BUFFER_SIZE, stdin);
        		//输入“.quit"则退出服务器
        		if(strcmp(input_msg, QUIT_CMD) == 0)
        		{
        			exit(0);
        		}
        		for(i = 0; i < CONCURRENT_MAX; i++)
        		{
        			if(client_fds[i] != 0)
        			{
        				printf("client_fds[%d]=%d\n", i, client_fds[i]);
        				send(client_fds[i], input_msg, BUFFER_SIZE, 0);
        			}
        		}
        	}
        	if(FD_ISSET(server_sock_fd, &server_fd_set))
        	{
        		//有新的连接请求
        		struct sockaddr_in client_address;
        		socklen_t address_len;
				
				//等待连接
        		int client_sock_fd = accept(server_sock_fd, (struct sockaddr *)&client_address, &address_len);
        		if(client_sock_fd > 0)
        		{
					printf("new connection client_sock_fd = %d\n", client_sock_fd);
					
					//判断是否同个IP地址连入
					if(strstr(rev_net, inet_ntoa(client_address.sin_addr)) == NULL)
					{
						strcat(rev_net, inet_ntoa(client_address.sin_addr));
						for(i = 0; i < CONCURRENT_MAX; i++)
						{
							if(client_fds[i] == 0)
							{
								index = i;
								client_fds[i] = client_sock_fd;
								break;
							}
						}
					}
        			if(index >= 0)
        			{
        				printf("新客户端(%d)加入成功 %s:%d\n", index, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        			}
        			else
        			{
        				bzero(input_msg, BUFFER_SIZE);
        				strcpy(input_msg, "服务器加入的客户端数达到最大值,无法加入!\n");
        				send(client_sock_fd, input_msg, BUFFER_SIZE, 0);
        				printf("客户端连接数达到最大值，新客户端加入失败 %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        			}
        		}
        	}
			//接收客户端数据
        	for(i = 0; i < CONCURRENT_MAX; i++)
        	{
        		if(client_fds[i] != 0)
        		{
        			if(FD_ISSET(client_fds[i], &server_fd_set))
        			{
        				//处理某个客户端过来的消息
        				bzero(recv_msg, BUFFER_SIZE);
        				long byte_num = recv(client_fds[i], recv_msg, BUFFER_SIZE, 0);
        				if (byte_num > 0)
        				{
        					if(byte_num > BUFFER_SIZE)
        					{
        						byte_num = BUFFER_SIZE;
        					}
        					recv_msg[byte_num] = '\0';
							if(strstr(recv_msg,"test"))
							{
								client_fds[0] = client_fds[i];
								client_fds[i] = 0;
							}
							send(client_fds[0],recv_msg,strlen(recv_msg),0);
        					printf("客户端(%d):%s\n", i, recv_msg);
							
        				}
        				else if(byte_num < 0)
        				{
        					printf("从客户端(%d)接受消息出错.\n", i);
        				}
        				else
        				{
        					FD_CLR(client_fds[i], &server_fd_set);
        					client_fds[i] = 0;
        					printf("客户端(%d)退出了\n", i);
        				}
        			}
        		}
        	}
        }
    }
	return 0;
}
