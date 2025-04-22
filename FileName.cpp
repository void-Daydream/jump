#include<bits/stdc++.h>
#include<graphics.h>
#include<Windows.h>
#include<conio.h>
#include<chrono>
#include<cmath>
#include<random>
#include<io.h>

std::mt19937_64 rnd(std::chrono::steady_clock().now().time_since_epoch().count());//时间戳随机

bool beginning = false;
bool body = false;
bool end = false;//游戏三阶段

char get[10];
int bestscore;
int score;
int lastscore;
int adcheck = 0;//游戏内得分以及获取记录

TCHAR s[11];
TCHAR buffer[114];
TCHAR buffer2[114];//各种数字转文字

int cheertime;//鼓励时间

struct Camera {
	int x = 80;
	int y = 40;
	int z = 210;
	int x_rot = 19;
	int y_rot = -49;
	int z_rot = 0;
	int fl = 300;//焦距
};//相机

struct Block {
	int x;
	int y;
	int z;
};//方块

int presschange;

struct Role {
	int x;
	int y;
	int z;
};//主角

int waited_z;//暂时储存

double nx;
double ny;
double nz;//用于转换变量

char key = ' ';//按键

double first;
double second;//返回值


float p_x[8];
float p_y[8];//各点屏幕坐标

float F_up;//牵引力
float f;//惯性

float jumptip;//跳跃进度条长度


void auto_3d_to_2d(int x, int y, int z, struct Camera c);//3d转2d

void auto_wor_to_cam(int x, int y, int z, struct Camera c);//世界转相机

void auto_cam_to_screen(int x, int y, int z, struct Camera c);//相机转屏幕

void sBlock(int x, int y, int z, int r, struct Camera c);//在(x,y,z)的位置生成半径为r的正方体

void role(int x,int y,int z,int r,struct Camera c);//主角移到某位置

bool box(Role role, Block block);//碰撞检测

void better_graphic();//更好的图像

void cheer();//鼓励语

void jumpshow();//蓄力进度条显示和变化

int main() {

	std::string dir = "C:/record"; // 文件夹路径
	if (_access(dir.c_str(), 0) == -1) {
		system("md C:\\record");
	}//保证文件夹存在

	FILE *fp = NULL;
	fp = fopen("C:/record/record.txt", "a+");
	fgets(get, 10, (FILE*)fp);
	fclose(fp);
	sscanf_s(get, "%d", &bestscore);//读取最高分

	fp = fopen("C:/record/last.txt", "a+");
	fgets(get, 10, (FILE*)fp);
	fclose(fp);
	sscanf_s(get, "%d", &lastscore);//读取上次得分

	int round = 1;//成功跳跃次数，用于加分判断
	int check = 0;//跳跃状态
	static std::uniform_int_distribution<int> gen(200, 300);//随机方块2位置
	Camera cam;
	Block block{ -100,-100,350 };
	Block block2{ -100,-100,350+gen(rnd)};
	Role day{ -100,0,350 };//定义结构体
	ExMessage m{};//定义消息结构体

	initgraph(700, 700);//产生图像
	setorigin(350, 350);//设置原点
	BeginBatchDraw();//辅助图像刷新
	beginning = true;//第一阶段
	setbkcolor(RGB(202, 229, 255));//设置背景颜色

	while (beginning) {
		FlushBatchDraw();//辅助刷新屏幕
		cleardevice();//刷新屏幕
		settextstyle(32, 16, _T("Consolas"));//设置字体
		settextcolor(RGB(0, 106, 199));//设置背景颜色
		outtextxy(-270, -150, _T("Welcome to GAME JUMP ONE JUMP !"));
		_stprintf(buffer, _T("Your last score is :%d"), lastscore);
		outtextxy(-260, 0, buffer);
		settextcolor(RGB(105, 165, 217));
		outtextxy(-270, 100, _T("go on last game,press SPACE"));
		outtextxy(-270, 170, _T("press other key to begin a new game"));//文字显示
		if (_kbhit()) {
			key = _getch();
			if (key == ' ')score = lastscore;
			else score = 0;
			body = true;
			beginning = false;
		}//开始判断
	}

	settextcolor(RGB(0, 106, 199));
	setaspectratio(1, -1);//设置y轴正方向
	while (body == true) {//主循环
		FlushBatchDraw();//辅助刷新屏幕
		cleardevice();//刷新屏幕

		if (check == 2) {
			while (box(day, block) == false && box(day,block2) == false && day.y > -250) {
				day.z = day.z + f;
				day.y = day.y + F_up - 2;
				if (box(day, block) || box(day, block2)) {
					if (day.y < -38) {
						day.z = day.z - f;
					}
					check = 0;
					F_up = 0;
				}
				F_up--;
				FlushBatchDraw();//辅助刷新屏幕
				cleardevice();//刷新屏幕
				sBlock(block.x, block.y, block.z, 50, cam);//生成方块1
				sBlock(block2.x, block2.y, block2.z, 50, cam);//生成方块2
				role(day.x, day.y, day.z, 10, cam);
				Sleep(2);
			}
			F_up = 0;
			jumptip = 0;
		}
		if (check == 0) {//按键点击检测
			if (GetAsyncKeyState('X'))cam.y = cam.y + 10,Sleep(15);//点击X镜头上升
			if (GetAsyncKeyState('Z'))cam.y = cam.y - 10, Sleep(15);//点击Z键镜头下降
			if (GetAsyncKeyState('D'))cam.x = cam.x + 10, Sleep(15);//点击D键镜头右移
			if (GetAsyncKeyState('A'))cam.x = cam.x - 10, Sleep(15);//点击A键镜头左移
			if (GetAsyncKeyState('W'))cam.z = cam.z + 10, Sleep(15);//点击W键镜头前进
			if (GetAsyncKeyState('S'))cam.z = cam.z - 10, Sleep(15);//点击S键镜头后退
			if (GetAsyncKeyState('L'))cam.y_rot++, Sleep(15);//点击L键镜头右转
			if (GetAsyncKeyState('J'))cam.y_rot--, Sleep(15);//点击J键镜头左转
			if (GetAsyncKeyState('K'))cam.x_rot++, Sleep(15);//点击K键镜头往下
			if (GetAsyncKeyState('I'))cam.x_rot--, Sleep(15);//点击I键镜头往上
			if (GetAsyncKeyState('Q')) {
				body = false,end = true;//点击Q键游戏退出
				break;
				_stprintf(buffer, _T("Your score is :%d"), score);
				_stprintf(buffer2, _T("Your best score is :%d"), bestscore);
			}

			if (GetAsyncKeyState(VK_SPACE)){
				check = 1;
				Sleep(20);
			}//点击空格蓄力
		}
		if (check == 1) {
			if (GetAsyncKeyState(VK_SPACE)) {
				F_up = F_up + 0.5;
				f = F_up * 1.3;	
				jumptip++;
			}
			else check = 2;
			Sleep(27);
		}
		
		else {
			day.y--;
			while (box(day, block))day.y++;
			while (box(day, block2)) {
				day.y++;
				Sleep(2);
				if (adcheck != round) {
					check = 0;
					score++, adcheck = round;
					block.x = block2.x;
					block.y = block2.y;
					block.z = block2.z;
					block2.z = block.z + gen(rnd);
					cheertime = 0;
					round++;
				}
			}
		}

		if (day.y < -300)day.y = 0, day.z = block.z, day.x = block.x, f = 0;//重新开始

		if (score == 2 && cheertime <= 6) cheer();
		if (score == 4 && cheertime <= 6) cheer();
		if (score == 6 && cheertime <= 6) cheer();

		sBlock(block.x, block.y, block.z, 50, cam);//生成方块1
		sBlock(block2.x, block2.y, block2.z, 50, cam);//生成方块2

		jumpshow();
		role(day.x, day.y, day.z, 10, cam);
		setaspectratio(1, 1);
		_stprintf_s(s, _T("score:%d"), score);
		outtextxy(-330, -330, s);
		setaspectratio(1, -1);
		setaspectratio(1, -1);

		if (block.z - cam.z > 65)cam.z = cam.z + 1;
	}
	
	while (end == true) {
		FlushBatchDraw();
		cleardevice();
		setaspectratio(1, 1);
		outtextxy(-270, -150, _T("GAME IS OVER"));
		_stprintf(buffer, _T("Your score is :%d"), score);
		_stprintf(buffer2, _T("Your best score is :%d"), bestscore);
		outtextxy(-270, -100, buffer);
		outtextxy(-270, -50, buffer2);
		outtextxy(-270, 100, _T("(press space to escape)"));
		Sleep(200);
		key = _getch();
		if(key == ' ')end = false;
	}
	if (score > bestscore) {
		char newnum[114];
		fp = fopen("C:/record/record.txt", "w+");
		sprintf(newnum, "%d", score);
		fprintf(fp, "%s", newnum);
		fclose(fp);
	}

	fp = fopen("C:/record/last.txt","w+");
	fprintf(fp, "%d", score);
	fclose(fp);
	TCHAR buffer3[114];
	_stprintf(buffer3, _T("当前分数为:%d\n历史最高为%d\n分数已保存"), score, bestscore);
	MessageBox(GetHWnd(), buffer3, _T("提示"), MB_OK);
	EndBatchDraw();
	closegraph();//游戏停止
	return 0;
}

void better_graphic() {
	setbkcolor(RGB(202, 229, 255));
	setlinecolor(RGB(0, 132, 255));
	POINT pts[4] = { {p_x[0],p_y[0]},{p_x[1],p_y[1]},{p_x[3],p_y[3]},{p_x[2],p_y[2]} };
	POINT pts2[4] = { {p_x[4],p_y[4]},{p_x[5],p_y[5]},{p_x[7],p_y[7]},{p_x[6],p_y[6]} };
	POINT pts3[4] = { {p_x[0],p_y[0]},{p_x[4],p_y[4]},{p_x[6],p_y[6]},{p_x[2],p_y[2]} };//AEGC
	POINT pts4[4] = { {p_x[1],p_y[1]},{p_x[5],p_y[5]},{p_x[7],p_y[7]},{p_x[3],p_y[3]} };//BFHD
	POINT pts5[4] = { {p_x[0],p_y[0]},{p_x[4],p_y[4]},{p_x[5],p_y[5]},{p_x[1],p_y[1]} };//AEFB
	POINT pts6[4] = { {p_x[2],p_y[2]},{p_x[6],p_y[6]},{p_x[7],p_y[7]},{p_x[3],p_y[3]} };//CGHD
	//获得点坐标

	polygon(pts6, 4);
	polygon(pts5, 4);
	polygon(pts4, 4);
	polygon(pts3, 4);
	polygon(pts2, 4);
	polygon(pts, 4);//连接每个点
}//更好的图像

void auto_3d_to_2d(int x, int y, int z, struct Camera c) {
	auto_wor_to_cam(x, y, z, c);
	auto_cam_to_screen(nx, ny, nz, c);
}//3D转2D

void auto_wor_to_cam(int x, int y, int z, struct Camera c) {
	float y_z;
	float y_x;
	float x_y;
	float x_z;
	float z_x;
	float z_y;

	nx = x - c.x;
	ny = y - c.y;
	nz = z - c.z;

	double y_rot = c.y_rot * 3.14159 / 180.0;
	y_z = nz * cos(y_rot) + nx * sin(y_rot);
	y_x = nx * cos(y_rot) - nz * sin(y_rot);
	double x_rot = c.x_rot * 3.14159 / 180.0;
	x_y = ny * cos(x_rot) + y_z * sin(x_rot);
	x_z = y_z * cos(x_rot) - ny * sin(x_rot);
	double z_rot = c.z_rot * 3.14159 / 180.0;
	z_x = y_x * cos(z_rot) + x_y * sin(z_rot);
	z_y = x_y * cos(z_rot) - y_x * sin(z_rot);

	nx = z_x;
	ny = z_y;
	nz = x_z;
}//世界转镜头

void auto_cam_to_screen(int x, int y, int z, struct Camera c) {
	first = x * c.fl / static_cast<float>(z);
	second = y * c.fl / static_cast<float>(z);
}//镜头转屏幕

void sBlock(int x, int y, int z, int r, struct Camera c) {
	auto_3d_to_2d(x + r, y + r, z + r, c);
	solidcircle(first, second, 1);
	p_x[0] = first;
	p_y[0] = second;
	auto_3d_to_2d(x - r, y + r, z + r, c);
	solidcircle(first, second, 1);
	p_x[1] = first;
	p_y[1] = second;
	auto_3d_to_2d(x + r, y - r, z + r, c);
	solidcircle(first, second, 1);
	p_x[2] = first;
	p_y[2] = second;
	auto_3d_to_2d(x - r, y - r, z + r, c);
	solidcircle(first, second, 1);
	p_x[3] = first;
	p_y[3] = second;
	auto_3d_to_2d(x + r, y + r, z - r, c);
	solidcircle(first, second, 1);
	p_x[4] = first;
	p_y[4] = second;
	auto_3d_to_2d(x - r, y + r, z - r, c);
	solidcircle(first, second, 1);
	p_x[5] = first;
	p_y[5] = second;
	auto_3d_to_2d(x + r, y - r, z - r, c);
	solidcircle(first, second, 1);
	p_x[6] = first;
	p_y[6] = second;
	auto_3d_to_2d(x - r, y - r, z - r, c);
	solidcircle(first, second, 1);
	p_x[7] = first;
	p_y[7] = second;

	better_graphic();
}//生成方块

void role(int x,int y,int z,int r,struct Camera c) {
	sBlock(x, y, z,r,c);
	sBlock(x, y + 2*r, z, r, c);
}

bool box(Role role, Block block) {
	bool ret = false;
	if (abs(role.x - block.x) <= 40) {
		if (abs(role.z - block.z) <= 40) {
			if (role.y - block.y <= 60) {
				ret = true;
			}
		}
	}
	return ret;
}

void cheer() {
	setaspectratio(1, 1);
	static std::uniform_int_distribution<int> gen(1, 3);
	switch (gen(rnd))
	{
	case 1:cheertime++,outtextxy(-270, 150, _T("VERY GOOD!"));
	case 2:cheertime++,outtextxy(-270, 150, _T("EXCELLENT!"));
	case 3:cheertime++,outtextxy(-270, 150, _T("GREAT JUMP!"));
	default:
		break;
	}
	setaspectratio(1, -1);
	Sleep(200);
}

void jumpshow() {
	if (jumptip != 0) {
		setfillcolor(RGB(0, 106, 199));
		solidrectangle(-300, -200 + jumptip, -280, -200);
	}
}
