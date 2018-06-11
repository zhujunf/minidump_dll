#ifndef CLIENT_H_
#define CLIENT_H_

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/stat.h>
#include <string>
#include <list>
#include <WinSock2.h>
#include <afxinet.h>
//#define INVALID_SOCKET				-1
#define MAX_PATH					260
//#define trace						printf

#define FTP_PARAM_BASE
#define FTP_DEFAULT_PORT			"21"							//FTP默认端口号
#define FTP_DEFAULT_BUFFER			1024*4							//FTP下载缓冲默认大小
#define FTP_DEFAULT_PATH			""						//FTP默认保存路径
	
#define FTP_COMMAND_BASE			1000
#define FTP_COMMAND_END				FTP_COMMAND_BASE + 30
#define FTP_COMMAND_USERNAME		FTP_COMMAND_BASE + 1			//用户名
#define FTP_COMMAND_PASSWORD		FTP_COMMAND_BASE + 2			//密码
#define FTP_COMMAND_QUIT			FTP_COMMAND_BASE + 3			//退出
#define FTP_COMMAND_CURRENT_PATH	FTP_COMMAND_BASE + 4			// 获取文件路径
#define FTP_COMMAND_TYPE_MODE		FTP_COMMAND_BASE + 5			// 改变传输模式
#define FTP_COMMAND_PSAV_MODE		FTP_COMMAND_BASE + 6			// 被动端口模式
#define FTP_COMMAND_DIR				FTP_COMMAND_BASE + 7			// 获取文件列表
#define FTP_COMMAND_CHANGE_DIRECTORY FTP_COMMAND_BASE + 8			// 改变路径
#define FTP_COMMAND_DELETE_FILE		FTP_COMMAND_BASE + 9			// 删除文件
#define FTP_COMMAND_DELETE_DIRECTORY FTP_COMMAND_BASE + 10			// 删除目录/文件夹
#define FTP_COMMAND_CREATE_DIRECTORY FTP_COMMAND_BASE + 11			// 创建目录/文件夹
#define FTP_COMMAND_RENAME_BEGIN    FTP_COMMAND_BASE  +12			// 开始重命名
#define FTP_COMMAND_RENAME_END      FTP_COMMAND_BASE + 13			// 重命名结束
#define FTP_COMMAND_FILE_SIZE		FTP_COMMAND_BASE + 14			// 获取文件大小
#define FTP_COMMAND_DOWNLOAD_POS	FTP_COMMAND_BASE + 15			// 下载文件从指定位置开始
#define FTP_COMMAND_DOWNLOAD_FILE	FTP_COMMAND_BASE + 16			// 下载文件
#define FTP_COMMAND_UPLOAD_FILE		FTP_COMMAND_BASE + 17			// 上传文件
#define FTP_COMMAND_APPEND_FILE		FTP_COMMAND_BASE + 18			// 追加上载文件	

/*		  登陆步骤
		login2Server
			|
		inputUserName
			|
		inputPassWord
			|
		  具体操作
			|
		  quit
*/

class CFTPManager 
{
public :
	
	enum type {
		binary = 0x31,
		ascii,
	};
	
	CFTPManager(void);

	virtual ~CFTPManager(void);
	

	// ! 登陆服务器
	INT32 login2Server(const std::string &serverIP);

	// !输入用户名
	INT32 inputUserName(const std::string &userName);

	// !输入密码
	INT32 inputPassWord(const std::string &password);

	// !退出FTP
	INT32 quitServer(void);

	// !命令： PWD
	const std::string PWD();

	// !设置传输格式 2进制  还是ascii方式传输
	INT32 setTransferMode(type mode);

	// !设置为被动模式
	const std::string Pasv();

	// ! 命令： DIR
	const std::string Dir(const std::string &path);

	// !命令 ： CD
	INT32 CD(const std::string &path);

	// ！删除文件
	INT32 DeleteFile(const std::string &strRemoteFile);

	// ! 删除文件夹/目录
	INT32 DeleteDirectory(const std::string &strRemoteDir);

	// ! 创建目录/文件夹
	INT32 CreateDirectory(const std::string &strRemoteDir);

	// !重命名
	INT32 Rename(const std::string &strRemoteFile, const std::string &strNewFile);

	// !获取文件大小
	long getFileLength(const std::string &strRemoteFile);

	// !关闭连接
	void Close(int sock);

	// 下载文件
	INT32 Get(const std::string &strRemoteFile, const std::string &strLocalFile);

	// 上载文件  支持断电传送方式
	INT32 Put(const std::string &strRemoteFile, const std::string &strLocalFile);

	// 文件大小计数器清零
	void ClearFileSize();
public://private:
	// !合成发送到服务器的命令
	const std::string parseCommand(const unsigned int command, const std::string &strParam);

	// ! 建立连接
	INT32 Connect(int socketfd, const std::string &serverIP, unsigned int nPort);

	// ! 返回服务器信息
	const std::string serverResponse(int sockfd);

	// !获取服务器数据
	INT32 getData(int fd, char *strBuf, unsigned long length);

	// !发送命令
	INT32 Send(int fd, const std::string &cmd);

	// !发送命令
	INT32 Send(int fd, const char *cmd, const size_t len);

	// !建立数据连接
	INT32 createDataLink(int data_fd);

	// !解析PASV模式返回的字符串获取FTP端口号和FTP服务器IP
	INT32 ParseString(std::list<std::string> strArray, unsigned long & nPort ,std::string & strServerIp);

	// 打开本地文件
	FILE *createLocalFile(const std::string &strLocalFile);

	// 下载文件 
	INT32 downLoad(const std::string &strRemoteFile, const std::string &strLocalFile, const int pos = 0, const unsigned int length = 0);

	// 解析返回ftp命令的值
	INT32 parseResponse(const std::string &str);
public:
	//文件大小
	long long unsigned m_nFilesize;
public://private:
	//！控制连接套接字
	int		m_cmdSocket;
	
	// !当前用户名
	std::string m_strUserName;

	// !当前用户密码
	std::string m_strPassWord;

	// !服务器的IP
	std::string m_strServerIP;

	// !服务器Port
	unsigned int m_nServerPort;

	// !服务器回应信息缓存
	std::string m_strResponse;

	// !保存命令参数
	std::string m_commandStr;

	// ！当前使用的命令参数
	unsigned int m_nCurrentCommand;

	// !是否登陆标志。
	bool	m_bLogin;

	
};


#endif
