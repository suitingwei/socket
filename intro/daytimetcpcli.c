#include	"unp.h"

int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 3)
		err_quit("usage: a.out <IPaddress> <Port>");

    /**
     * create the ipv4 tcp connection to the given server ip.
     */
	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		err_sys("socket error");

    //清空结构体内的数据,这就是 c 语言蛋疼的地方了，内存啥的都需要手动来
    //从操作系统分配得到的不一定是什么数据
	bzero(&servaddr, sizeof(servaddr));

	//设置协议
	servaddr.sin_family = AF_INET;

    //设置服务器的端口,这里是写死了一个端口13
	servaddr.sin_port   = htons(atoi(argv[2]));	/* daytime server */

	//端口转换,从点分十进制变成
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}
