#include<bits/stdc++.h>
#include<graphics.h>
#include<Windows.h>
#include<conio.h>
#include<chrono>
#include<cmath>
#include<random>
#include<io.h>

std::mt19937_64 rnd(std::chrono::steady_clock().now().time_since_epoch().count());//ʱ������

bool beginning = false;
bool body = false;
bool end = false;//��Ϸ���׶�

char get[10];
int bestscore;
int score;
int lastscore;
int adcheck = 0;//��Ϸ�ڵ÷��Լ���ȡ��¼

TCHAR s[11];
TCHAR buffer[114];
TCHAR buffer2[114];//��������ת����

int cheertime;//����ʱ��

struct Camera {
	int x = 80;
	int y = 40;
	int z = 210;
	int x_rot = 19;
	int y_rot = -49;
	int z_rot = 0;
	int fl = 300;//����
};//���

struct Block {
	int x;
	int y;
	int z;
};//����

int presschange;

struct Role {
	int x;
	int y;
	int z;
};//����

int waited_z;//��ʱ����

double nx;
double ny;
double nz;//����ת������

char key = ' ';//����

double first;
double second;//����ֵ


float p_x[8];
float p_y[8];//������Ļ����

float F_up;//ǣ����
float f;//����

float jumptip;//��Ծ����������


void auto_3d_to_2d(int x, int y, int z, struct Camera c);//3dת2d

void auto_wor_to_cam(int x, int y, int z, struct Camera c);//����ת���

void auto_cam_to_screen(int x, int y, int z, struct Camera c);//���ת��Ļ

void sBlock(int x, int y, int z, int r, struct Camera c);//��(x,y,z)��λ�����ɰ뾶Ϊr��������

void role(int x,int y,int z,int r,struct Camera c);//�����Ƶ�ĳλ��

bool box(Role role, Block block);//��ײ���

void better_graphic();//���õ�ͼ��

void cheer();//������

void jumpshow();//������������ʾ�ͱ仯

int main() {

	std::string dir = "C:/record"; // �ļ���·��
	if (_access(dir.c_str(), 0) == -1) {
		system("md C:\\record");
	}//��֤�ļ��д���

	FILE *fp = NULL;
	fp = fopen("C:/record/record.txt", "a+");
	fgets(get, 10, (FILE*)fp);
	fclose(fp);
	sscanf_s(get, "%d", &bestscore);//��ȡ��߷�

	fp = fopen("C:/record/last.txt", "a+");
	fgets(get, 10, (FILE*)fp);
	fclose(fp);
	sscanf_s(get, "%d", &lastscore);//��ȡ�ϴε÷�

	int round = 1;//�ɹ���Ծ���������ڼӷ��ж�
	int check = 0;//��Ծ״̬
	static std::uniform_int_distribution<int> gen(200, 300);//�������2λ��
	Camera cam;
	Block block{ -100,-100,350 };
	Block block2{ -100,-100,350+gen(rnd)};
	Role day{ -100,0,350 };//����ṹ��
	ExMessage m{};//������Ϣ�ṹ��

	initgraph(700, 700);//����ͼ��
	setorigin(350, 350);//����ԭ��
	BeginBatchDraw();//����ͼ��ˢ��
	beginning = true;//��һ�׶�
	setbkcolor(RGB(202, 229, 255));//���ñ�����ɫ

	while (beginning) {
		FlushBatchDraw();//����ˢ����Ļ
		cleardevice();//ˢ����Ļ
		settextstyle(32, 16, _T("Consolas"));//��������
		settextcolor(RGB(0, 106, 199));//���ñ�����ɫ
		outtextxy(-270, -150, _T("Welcome to GAME JUMP ONE JUMP !"));
		_stprintf(buffer, _T("Your last score is :%d"), lastscore);
		outtextxy(-260, 0, buffer);
		settextcolor(RGB(105, 165, 217));
		outtextxy(-270, 100, _T("go on last game,press SPACE"));
		outtextxy(-270, 170, _T("press other key to begin a new game"));//������ʾ
		if (_kbhit()) {
			key = _getch();
			if (key == ' ')score = lastscore;
			else score = 0;
			body = true;
			beginning = false;
		}//��ʼ�ж�
	}

	settextcolor(RGB(0, 106, 199));
	setaspectratio(1, -1);//����y��������
	while (body == true) {//��ѭ��
		FlushBatchDraw();//����ˢ����Ļ
		cleardevice();//ˢ����Ļ

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
				FlushBatchDraw();//����ˢ����Ļ
				cleardevice();//ˢ����Ļ
				sBlock(block.x, block.y, block.z, 50, cam);//���ɷ���1
				sBlock(block2.x, block2.y, block2.z, 50, cam);//���ɷ���2
				role(day.x, day.y, day.z, 10, cam);
				Sleep(2);
			}
			F_up = 0;
			jumptip = 0;
		}
		if (check == 0) {//����������
			if (GetAsyncKeyState('X'))cam.y = cam.y + 10,Sleep(15);//���X��ͷ����
			if (GetAsyncKeyState('Z'))cam.y = cam.y - 10, Sleep(15);//���Z����ͷ�½�
			if (GetAsyncKeyState('D'))cam.x = cam.x + 10, Sleep(15);//���D����ͷ����
			if (GetAsyncKeyState('A'))cam.x = cam.x - 10, Sleep(15);//���A����ͷ����
			if (GetAsyncKeyState('W'))cam.z = cam.z + 10, Sleep(15);//���W����ͷǰ��
			if (GetAsyncKeyState('S'))cam.z = cam.z - 10, Sleep(15);//���S����ͷ����
			if (GetAsyncKeyState('L'))cam.y_rot++, Sleep(15);//���L����ͷ��ת
			if (GetAsyncKeyState('J'))cam.y_rot--, Sleep(15);//���J����ͷ��ת
			if (GetAsyncKeyState('K'))cam.x_rot++, Sleep(15);//���K����ͷ����
			if (GetAsyncKeyState('I'))cam.x_rot--, Sleep(15);//���I����ͷ����
			if (GetAsyncKeyState('Q')) {
				body = false,end = true;//���Q����Ϸ�˳�
				break;
				_stprintf(buffer, _T("Your score is :%d"), score);
				_stprintf(buffer2, _T("Your best score is :%d"), bestscore);
			}

			if (GetAsyncKeyState(VK_SPACE)){
				check = 1;
				Sleep(20);
			}//����ո�����
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

		if (day.y < -300)day.y = 0, day.z = block.z, day.x = block.x, f = 0;//���¿�ʼ

		if (score == 2 && cheertime <= 6) cheer();
		if (score == 4 && cheertime <= 6) cheer();
		if (score == 6 && cheertime <= 6) cheer();

		sBlock(block.x, block.y, block.z, 50, cam);//���ɷ���1
		sBlock(block2.x, block2.y, block2.z, 50, cam);//���ɷ���2

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
	_stprintf(buffer3, _T("��ǰ����Ϊ:%d\n��ʷ���Ϊ%d\n�����ѱ���"), score, bestscore);
	MessageBox(GetHWnd(), buffer3, _T("��ʾ"), MB_OK);
	EndBatchDraw();
	closegraph();//��Ϸֹͣ
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
	//��õ�����

	polygon(pts6, 4);
	polygon(pts5, 4);
	polygon(pts4, 4);
	polygon(pts3, 4);
	polygon(pts2, 4);
	polygon(pts, 4);//����ÿ����
}//���õ�ͼ��

void auto_3d_to_2d(int x, int y, int z, struct Camera c) {
	auto_wor_to_cam(x, y, z, c);
	auto_cam_to_screen(nx, ny, nz, c);
}//3Dת2D

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
}//����ת��ͷ

void auto_cam_to_screen(int x, int y, int z, struct Camera c) {
	first = x * c.fl / static_cast<float>(z);
	second = y * c.fl / static_cast<float>(z);
}//��ͷת��Ļ

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
}//���ɷ���

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
