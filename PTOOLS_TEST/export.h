#pragma once
#include <Windows.h>

#ifdef WIN_EXPORT 
#define PTOOLS_WINAPI __stdcall
#else
#define PTOOLS_WINAPI __cdecl
#endif

#if defined(_WIN64)
typedef unsigned __int64 _PTR;
typedef __int64 _SPTR;
#else
typedef unsigned __int32 _PTR;
typedef __int32 _SPTR;
#endif

#ifdef __cplusplus
extern "C"
{
#endif
	typedef struct EnStart {
		char ip[64];
		UINT port;
		UINT version;
		UINT appid;
		char id[64];
		char pw[64];
		UINT status;
	}EnStart, * pEnStart;

	// 系统是否初始化完毕
	BOOL PTOOLS_WINAPI IsReady();
	// 系统运行状态
	/*
	@return
		0		未运行
		1		更新中
		2		更新失败
		3		分析中
		4		初始化
		5		失败
		6		成功
	*/
	ULONG PTOOLS_WINAPI GetProcessStatus();
	// 注册状态通知回调
	/*
	@param callback_ptr	接收状态值的回调函数地址
	@return	是否成功
	*/
	BOOL PTOOLS_WINAPI OnProcessChange(void* callback_ptr);
	// 初始化
	/*
	@param ip		服务器IP
	@param port		服务器端口
	@param ver		应用版本
	@param appid	应用ID
	*/
	int PTOOLS_WINAPI InitIt(const char* ip, int port, int ver, int appid);
	int PTOOLS_WINAPI InitItSync(const char* ip, int port, int ver, int appid);
	// 反初始化
	void PTOOLS_WINAPI UnloadIt();
	// 关闭提示
	void PTOOLS_WINAPI CloseMessage();

	// 初始化进程操作对象
	/*
	@param pid		进程ID
	*/
	void* PTOOLS_WINAPI LoadProcess(DWORD pid);
	// 反初始化进程操作对象
	/*
	@param handle	进程句柄
	*/
	void PTOOLS_WINAPI UnloadProcess(void* handle);
	// 申请内存
	/*
	@param handle	进程句柄
	@param len		申请内存大小
	@param execute	是否可执行
	@param outPtr	接收申请地址的buffer地址入口(因为模块地址可能是4字节或者8字节,所以这里使用Buffer接收)
	*/
	void PTOOLS_WINAPI AllocProcessMemory(void* handle, DWORD len, bool execute, _PTR* outPtr);

#ifdef _WIN64
	int PTOOLS_WINAPI LoginIt(const char* id, const char* pw, long long chk);
	int PTOOLS_WINAPI LoginItSync(const char* id, const char* pw, long long chk);
	void PTOOLS_WINAPI FreeProcessMemory(void* handle, void* address);
	void PTOOLS_WINAPI GetProcessMemory(void* handle, void* address, char* buffer, DWORD len);
	void PTOOLS_WINAPI PutProcessMemory(void* handle, void* address, char* buffer, DWORD len);
#else
	// 登录
	/*
	@param id	用户名
	@param pw	密码
	@param chk	额外参数,写0
	*/
	int PTOOLS_WINAPI LoginIt(const char* id, const char* pw, int chk);
	int PTOOLS_WINAPI LoginItSync(const char* id, const char* pw, int chk);
	// 释放内存
	/*
	@param handle	进程句柄
	@param LAddress	地址低32位
	@param HAddress	地址高32位
	*/
	void PTOOLS_WINAPI FreeProcessMemory(void* handle, void* LAddress, void* HAddress);
	// 读取内存
	/*
	@param handle	进程句柄
	@param LAddress	地址低32位
	@param HAddress	地址高32位
	@param buffer	接收读取到的数据的指定内存地址
	@param len		读取长度
	*/
	void PTOOLS_WINAPI GetProcessMemory(void* handle, void* LAddress, void* HAddress, char* buffer, DWORD len);
	// 写入内存
	/*
	@param handle	进程句柄
	@param LAddress	地址低32位
	@param HAddress	地址高32位
	@param buffer	需要写入到指定内存地址的数据的地址
	@param len		读取长度
	*/
	void PTOOLS_WINAPI PutProcessMemory(void* handle, void* LAddress, void* HAddress, char* buffer, DWORD len);

#endif
	// 获得目标进程模块载入地址
	/*
	@param handle	进程句柄
	@param module_name	模块名称
	@param o_buffer	接收模块地址的buffer地址入口(因为模块地址可能是4字节或者8字节,所以这里使用Buffer接收)
	*/
	void PTOOLS_WINAPI GetModuleBase(void* handle, const char* module_name, _PTR* o_buffer);
	// 添加删除文件任务到清理任务队列
	//	当驱动执行环境清理任务时强制删除指定的文件
	/*
	@param file_name	文件名名称
	*/
	void PTOOLS_WINAPI AddWantDeleteFile(char* file_name);

	// 从字节集载入DLL
	/*
	@param buffer	字节集指针
	@return			模块句柄
	*/
	void* PTOOLS_WINAPI LoadLibraryFormByte(char* buffer);
	// 从文件载入DLL
	/*
	@param path		文件路径
	@return			模块句柄
	*/
	void* PTOOLS_WINAPI LoadLibraryFormFile(const char* path);
	// 获得导出函数地址
	/*
	@param handle	模块句柄
	@param funcName	导出函数名称
	@return			函数地址
	*/
	void* PTOOLS_WINAPI GetMemProcAddress(void* handle, const char* funcName);
	// 释放句柄
	/*
	@param handle	模块句柄
	*/
	void PTOOLS_WINAPI CloseMemHandle(void* handle);

	// 内存映射系统
	/*
	*/

	// 获得win10版本号
	typedef struct _Version {
		ULONG OSMajorVersion;                                                   //0xa4
		ULONG OSMinorVersion;                                                   //0xa8
		USHORT OSBuildNumber;                                                   //0xac
		USHORT OSCSDVersion;                                                    //0xae
		ULONG OSPlatformId;                                                     //0xb0
		ULONG ImageSubsystem;                                                   //0xb4
		ULONG ImageSubsystemMajorVersion;                                       //0xb8
		ULONG ImageSubsystemMinorVersion;                                       //0xbc
	}_Version, * pVersion;
	_Version* PTOOLS_WINAPI rGetWin10BuildNumber();

	// 分析系统文件
	typedef struct SymbalResult {
		DWORD num;
		DWORD result;
	}SymbalResult, * pSymbalResult;

	// 获得 file 文件内部未公开结构或者函数的偏移地址
	/*
	@param file				系统文件路径
	@param name				符号名称
	@param result			接受返回的 SymbalResult 结构所在 数据块的地址
	@return					成功返回true,失败返回false
	*/
	BOOL PTOOLS_WINAPI QuerySystemStruct(const char* file, const char* name, SymbalResult* result);
	// 释放通过 QuerySystemStruct 获得的 SymbalResult 数据块
	/*
	@param result_ptr		储存 SymbalResult 结构所在 内存块的地址
	@return					成功返回true,失败返回false
	*/
	BOOL PTOOLS_WINAPI CloseSystemStruct(SymbalResult* result);
	// 为Pure函数生成变形拷贝
	/*
	@param functionEntry	储存 要变形的函数的地址 的地址
	@param generatorEntry	接收 变形后的函数的地址 的地址
	@return					成功返回true,失败返回false
	 */
	BOOL PTOOLS_WINAPI DynamicPureGenerator(char** functionEntry, char** generatorEntry);
	// 创建本地内存读写专用句柄
	/*
	@param handle			进程句柄
	@return					成功返回新句柄,失败返回null
	 */
	HANDLE PTOOLS_WINAPI CreateLocalHandle(void* handle);
	// 创建本地内存读写专用句柄
	/*
	@param handle			本地内存读写专用句柄
	@return					成功返回true,失败返回false
	 */
	BOOL PTOOLS_WINAPI CloseLocalHandle(void* handle);
	// 移动鼠标
	/*
	@param x			目的 x 坐标
	@param y			目的 y 坐标
	@return				成功返回true,失败返回false
	 */
	BOOL PTOOLS_WINAPI MouseMove(LONG x, LONG y);
#ifdef __cplusplus
}
#endif
