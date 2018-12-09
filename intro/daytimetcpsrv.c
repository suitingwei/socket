#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

    //这都是包裹过的 socket 函数，处理了错误信息
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	//huamn to number long ,human to number short
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(8889);	/* daytime server */

	//绑定指定的端口地址
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	//监听套接字
	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		//accept 是在 tcp 三次握手之后才会返回,中间会阻塞，等待内核把三次握手搞定
		//那对于udp是怎么一个返回机制？每次都返回
		connfd = Accept(listenfd, (SA *) NULL, NULL);

        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        Write(connfd, buff, strlen(buff));

		Close(connfd);
	}
}
