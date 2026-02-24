#include<iostream>
#include<fstream>
#include<windows.h>
#include<string>
#include<vector>
#include<algorithm>
#include<clocale>
using namespace std;
void help() {
    cout << "���̼�¼�鿴�� - ʹ��˵��" << endl;
    cout << "==========================" << endl;
    cout << "1. �Զ�ɨ�赱ǰĿ¼�µļ��̼�¼�ļ�" << endl;
    cout << "2. ��ʱ��˳����ʾ�ļ��б�" << endl;
    cout << "3. ѡ���ļ���Ų鿴����" << endl;
    cout << "4. ֧�ֹؼ�������" << endl;
    cout << "5. ���� 'q' �˳�����" << endl;
    cout << "==========================" << endl;
}
vector<string> get(){
	vector<string> f;
	WIN32_FIND_DATA fd;
	HANDLE h=FindFirstFile("keylog_*.txt",&fd);
	if(h!=INVALID_HANDLE_VALUE){
		do f.push_back(fd.cFileName);
		while(FindNextFile(h,&fd));
		FindClose(h);
	}
	sort(f.begin(),f.end());
	return f;
}
void show(const string& fn){
	ifstream in(fn);
	if(!in.is_open()){cout<<"无法打开文件: "<<fn<<endl; return;}
	cout<<"\n=== 文件内容: "<<fn<<" ===\n";
	string s;
	while(getline(in,s)) cout<<s<<endl;
	cout<<"=== 文件结束 ===\n\n";
	in.close();
}
void search(const vector<string>& f,const string& kw){
	cout<<"\n=== 搜索关键词: '"<<kw<<"' ===\n";
	for(const auto& fn:f){
		ifstream in(fn);
		if(!in.is_open()) continue;
		string s;
		int ln=0;
		bool found=0;
		while(getline(in,s)){
			ln++;
			if(s.find(kw)!=string::npos){
				if(!found){cout<<"在文件 "<<fn<<" 中找到:\n"; found=1;}
				cout<<"  第 "<<ln<<" 行: "<<s<<endl;
			}
		}
		in.close();
	}
	cout<<"=== 搜索结束 ===\n\n";
}
int main(){
	setlocale(LC_ALL, "zh_CN.UTF-8");
	system("chcp 65001");
	cout<<"键盘记录查看器启动...\n";
	help();
	while(1){
		vector<string> f=get();
		if(f.empty()){cout<<"未找到任何键盘记录文件！\n按回车键退出...\n"; cin.get(); return 0;}
		cout<<"\n找到 "<<f.size()<<" 个记录文件:\n";
		for(int i=0;i<f.size();i++) cout<<"["<<i+1<<"] "<<f[i]<<endl;
		cout<<"\n请选择操作:\n1. 输入文件编号查看内容\n2. 输入 's' 进行搜索\n3. 输入 'q' 退出程序\n> ";
		string op;
		getline(cin,op);
		if(op=="q"||op=="Q") break;
		else if(op=="s"||op=="S"){
			cout<<"请输入要搜索的关键词: ";
			string kw;
			getline(cin,kw);
			if(!kw.empty()) search(f,kw);
		}else{
			try{
				int x=stoi(op);
				if(1<=x&&x<=f.size()) show(f[x-1]);
				else cout<<"无效的选择！\n";
			}catch(...){cout<<"无效的输入！\n";}
		}
	}
	cout<<"程序已退出\n";
	return 0;
}
