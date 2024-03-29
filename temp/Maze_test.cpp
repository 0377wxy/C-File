#include<iostream> //标准输入输出 
#include<ctime>//时间 
#include<windows.h>//图形设备接口函数， 
#include <conio.h>//文件输入输出 
#include <stdio.h>//c语言输入输出 
#include<time.h>
using namespace std;

#define UP 'w'			//定义上移键
#define DOWN 's'		//定义下移键
#define LEFT 'a'		//定义左移键
#define RIGHT 'd'		//定义右移键

//列出所有宏定义，使用注释方式说明用途
//列出所有全局变量，使用注释方式说明用途，也可以通过文字的方式在代码前或后说明用途。
static const unsigned __int64 epoch = 116444736000000000L;


class Node { //堆栈节点
	public:
		int x, y; //记录xy坐标
		int fx;//记录方向
		Node* next;
		Node(int x = 0, int y = 0, int fx = 0): x(x), y(y), fx(fx), next(NULL) {}
};
class Stack {
	private:
		Node* head;//栈的头指针
	public:
		Stack(): head(NULL) {} //头指针赋值为空
		~Stack() { //析构函数,防止内存泄漏
			Node* px;
			while (head != NULL) {
				px = head;
				head = px->next;
				delete (px);
			}
		}
		void push(int x, int y, int fx) {
			Node *px = new Node(x, y, fx); //构造新节点
			px->next = head;
			head = px;
		}
		void pop() { //栈顶
			if (empty()) return; //避免栈空的情况
			Node* px = head;
			head = px->next;
			delete (px);
		}
		bool empty() { //判空
			return head == NULL;
		}
		Node& top() { //返回栈顶节点
			return *head;
		}
};
void outps(Stack &stk) { //结尾输出路径
	Stack tmp;
	Node hc;
	while (!stk.empty()) {
		hc = stk.top();
		stk.pop();
		tmp.push(hc.x, hc.y, hc.fx);
	}
	hc = tmp.top();
	tmp.pop();
	cout << '(' << hc.x << ',' << hc.y << ')';
	while (!tmp.empty()) {
		hc = tmp.top();
		tmp.pop();
		cout << "->" << '(' << hc.x << ',' << hc.y << ')';
	}
}


int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; //方向数组
const int MAXNN = 72;//迷宫最大大小
int MAXN;//用户输入的迷宫大小 

//MAXN可修改
bool OK(int x, int y) { //判断点是否在界内
	return x > 0 && x < MAXN && y > 0 && y < MAXN;
}


int shuaxintime;//刷新时间
int maze_map[MAXNN][MAXNN];//迷宫地图，1为墙，0为空，2为怪兽 
bool vis[MAXNN][MAXNN]; //?
bool viss[MAXNN][MAXNN]; //记录该点是否被访问过 
int d[MAXNN][MAXNN];//记录当前位置搜索的方向，输出路径时用
char jt[5][6] = {"  ", "↑", "↓", "←", "→"};//打印移动路径 


void print_maze() { //打印迷宫
	for (int i = 1; i < MAXN; i++) {
		for (int j = 1; j < MAXN; j++) {
			if (maze_map[i][j] == 1) cout << "■";
			else if (maze_map[i][j] == 2) cout << "# ";
			else cout << "  ";
		}
		cout << endl;
	}
}
void maze_path() { //在搜索时同时打印迷宫和路径
	for (int i = 1; i < MAXN; i++) {
		for (int j = 1; j < MAXN; j++) {
			if (maze_map[i][j] == 1) cout << "■";
			else if (maze_map[i][j] == 2) cout << "# ";
			else if (d[i][j]) cout << jt[d[i][j]];
			else cout << "  ";
		}
		cout << endl;
	}
}

void maze_generate() { //Prim树结构迷宫生成
	srand((unsigned)time(NULL));//随机种子
	for (int i = 1; i < MAXN; i++) {
		for (int j = 1; j < MAXN; j++) {
			viss[i][j] = d[i][j] = vis[i][j] = 0;//重置所有点未访问
			if (i == 1 || i == MAXN - 1 || j == 1 || j == MAXN - 1) maze_map[i][j] = 1;
			else if (i % 2 == 0 && j % 2 == 0) maze_map[i][j] = 0;
			else maze_map[i][j] = 1;
		}
	}
	maze_map[2][1] = maze_map[MAXN - 2][MAXN - 1] = 0; //起点和终点
	bool flag = 1;
	vis[2][2] = 1;//访问过起点
	print_maze();
	int nowx = 2, nowy = 2; //当前访问格子
	while (flag) { //直到所有点都被访问过
		while (1) {
			//判断周围的点是否都被访问过
			bool flag2 = 1;
			int sj, dx, dy;
			for (int i = 0; i < 4; i++) {
				dx = nowx + dir[i][0] * 2;
				dy = nowy + dir[i][1] * 2;
				if (!OK(dx, dy)) continue; //越界
				if (vis[dx][dy]) continue; //访问过了
				flag2 = 0; //有没访问过的点
			}
			if (flag2) {
				break;//退出while
			}
			flag2 = 1;
			while (flag2) { //随机找到下一个没被访问过的点
				sj = rand() % 4;
				dx = nowx + dir[sj][0] * 2;
				dy = nowy + dir[sj][1] * 2;
				if (!OK(dx, dy)) {
					continue;   //越界
				}
				if (vis[dx][dy]) {
					continue;   //访问过了
				}
				vis[dx][dy] = 1;//访问该点
				maze_map[nowx + dir[sj][0]][nowy + dir[sj][1]] = 0;
				nowx = dx;
				nowy = dy;
				break;
			}
			print_maze();
		}
		cout<<"------------------"<<endl;
		//看是否所有点都被访问过
		flag = 0;//如果有则改成1
		for (int i = 1; i < MAXN; i++) {
			for (int j = 1; j < MAXN; j++) {
				if (i % 2 == 0 && j % 2 == 0) {
					if (!vis[i][j]) { //还有点没被访问的
						flag = 1;
						break;
					}
				}
			}
			if (flag) break;
		}
		//找新的nowx,nowy
		if (!flag) break; //所有点都已经被访问
		while (1) {
			nowx = rand() % (MAXN - 1) + 1;
			nowy = rand() % (MAXN - 1) + 1;
			if (nowx % 2 == 1) nowx++;
			if (nowy % 2 == 1) nowy++;
			if (!OK(nowx, nowy)) continue;
			if (!vis[nowx][nowy]) continue;
			//到这里说这对now xy是可以的
			break;
		}
	}
}

void read_map() { //读取迷宫
	FILE *fp1;
	char ch; //每个数据之间有' '
	if ((fp1 = fopen("D:\\Program_file\\C-File\\temp\\map.txt", "rb")) == NULL) {
		printf("-----------in.txt文件\n");
		exit(0);
	}
	fscanf(fp1, "%d", &MAXN);
	fscanf(fp1, "%c", &ch);
	for (int i = 0; i < MAXN; i++) {
		for (int j = 0; j < MAXN; j++) {
			fscanf(fp1, "%d", &maze_map[i][j]);
			fscanf(fp1, "%c", &ch);
		}
	}
	fclose(fp1);
	for (int i = 0; i < MAXN; i++) {
		for (int j = 0; j < MAXN; j++) {
			cout << maze_map[i][j] << " ";
		}
		cout << endl;
	}
	cout << "读取完成" << endl;
}

void dfs(int sx, int sy, int tx, int ty) { //使用堆栈实现的深度优先搜索
	Stack st;//栈
	st.push(sx, sy, 4);
	int x, y, dx, dy;
	while (!st.empty()) {
		//输出迷宫部分
		x = st.top().x;
		y = st.top().y;
		viss[x][y] = 1;
		d[x][y] = st.top().fx;
		Sleep(shuaxintime);
		system("cls");
		maze_path();
		cout << endl;
		cout << "■现在位置:(" << x << ',' << y << ")" << endl;
		if (x == tx && y == ty) {
			cout << "■找到终点" << endl;
			cout << "■一共有 1 条路径:" << endl;
			cout << "■路径 1 :" << endl;
			outps(st);
			cout << endl;
			break;
		}
		if (st.top().fx == 0) { //回溯
			viss[x][y] = 1;
			st.pop();
			continue;
		}
		st.top().fx = 0;
		for (int i = 0; i < 4; i++) {
			dx = x + dir[i][0];
			dy = y + dir[i][1];
			if (OK(dx, dy) && viss[dx][dy] == 0 && maze_map[dx][dy] == 0) {
				st.push(dx, dy, i + 1); //新点入栈
			}
		}
	}
}

gettimeofday(struct timeval* tp, void* tzp)
{
	FILETIME    file_time;
	SYSTEMTIME  system_time;
	ULARGE_INTEGER ularge;

	GetSystemTime(&system_time);
	SystemTimeToFileTime(&system_time, &file_time);
	ularge.LowPart = file_time.dwLowDateTime;
	ularge.HighPart = file_time.dwHighDateTime;

	tp->tv_sec = (long)((ularge.QuadPart - epoch) / 10000000L);
	tp->tv_usec = (long)(system_time.wMilliseconds * 1000);

	return 0;
}

void manual(int sx, int sy, int tx, int ty) {

	cout << "□输入搜索间隔(单位ms):";
	cin >> shuaxintime;

	Stack st;//栈
	st.push(sx, sy, 4);
	int x, y, dx, dy, df;//x,y是目前的位置坐标， 
	char direction = RIGHT;
	
	long double dif,target;
//	time_t now,t_begin,t_end;
	target = 60000.0; // 60 s
//	t_begin=time(NULL);

	struct timeval tvBegin, tvEnd;
	gettimeofday(&tvBegin, NULL);
	
	while (1) {
		//输出迷宫部分
		x = st.top().x;
		y = st.top().y;
		viss[x][y] = 1;
		d[x][y] = st.top().fx;
		Sleep(shuaxintime);
		system("cls");
		maze_path();
		cout << endl;
		cout << "■现在位置:(" << x << ',' << y << ")" << endl;
		
//		now = time(NULL);
//		dif = difftime(now,t_begin);
//		printf("time now: %f",dif);

		gettimeofday(&tvEnd, NULL);
		double dDuration = 1000 * (tvEnd.tv_sec - tvBegin.tv_sec) + ((tvEnd.tv_usec - tvBegin.tv_usec) / 1000.0);
		printf("\n使用的时间：%.2lfs", dDuration/1000);
		
		
		if (dDuration >= target){
			printf("\n时间到！\n\a"); // \a是让系统发出声音的一种表达方式
			return;
		}

  		


		if (x == tx && y == ty) {
			cout << "■找到终点" << endl;
			outps(st);
			cout << endl;
			break;
		}

		if (_kbhit()) {			//键盘输入返回1，非键盘输入返回0
			direction = _getch();
		} else {
			continue;
		}
		switch (direction) {
			case UP:
				dx = x + dir[0][0];
				dy = y + dir[0][1];
				df = 1;
				break;		//向上移动
			case DOWN:
				dx = x + dir[1][0];
				dy = y + dir[1][1];
				df = 2;
				break;		//向下移动
			case LEFT:
				dx = x + dir[2][0];
				dy = y + dir[2][1];
				df = 3;
				break;		//向左移动
			case RIGHT:
				dx = x + dir[3][0];
				dy = y + dir[3][1];
				df = 4;
				break;	//向右移动
		}
		if (OK(dx, dy) && maze_map[dx][dy] == 2) {
			cout << "遇到怪物，游戏结束" << endl;
			break;
		}
		if (OK(dx, dy) && maze_map[dx][dy] == 0) {
			if (viss[dx][dy] == 0) {
				st.push(dx, dy, df);

			} else {
				viss[x][y] = 0;
				st.pop();
				d[x][y] = 0;
			}
		} else {
			continue;
		}
	}
}

int main() {

	char hc;//进入游戏或退出程序 
	system("cls");
	int te;

	cout << "□读入迷宫（1） /  生成迷宫（0）:";
	cin >> te;
	if (te)
		read_map();
	else {
		cout << "■请自定迷宫大小" << endl;
		cout << "■Option:必须为奇数,推荐在10-40之间选" << endl;
		cout << "□迷宫大小:";
		cin >> MAXN;
		MAXN++;
		maze_generate();//生成树形迷宫
	}


	print_maze();//打印迷宫


	cout << "□手动（1） /  自动（0）:";
	cin >> te;
	if (te) {
		manual(2, 1, MAXN - 2, MAXN - 1);
	} else {
		//cout<<"■Option:迷宫为树形结构,存在且只有一条路径"<<endl;
		cout << "□开始搜索/退出程序[Y/N]:";
		cin >> hc;
		if (hc == 'Y' || hc == 'y') {
			system("cls");
			print_maze();//打印迷宫
			cout << "■如果是20左右,推荐50,如果迷宫比较大,因为打印迷宫也需要时间,可以设置间隔为0" << endl;
			cout << "□输入搜索间隔(单位ms):";
			cin >> shuaxintime;
			dfs(2, 1, MAXN - 2, MAXN - 1); //开始搜索
		}
	}
	cout << "□程序结束，输入任意字符结束" << endl;
	cin >> hc;
}
