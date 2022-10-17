#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <tchar.h>
#include <opencv2/opencv.hpp>
#include<queue>
#include<map>
#include<stack>

using namespace std;
using namespace cv;

unordered_map<string, bool> vis;
unordered_map<string, string>pre;
stack<string> st;
typedef pair<int, int> PII;
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };
int ord[10] ;
int a[4][4];
vector<string> adr(10); //ͼƬ���
vector<Mat> images(9);//ͼƬ�ĵ�ַ

//���ַ���ת��������
void sToA(string s)
{
	int num = 0;
	for (int i = 1; i <= 3; i++) {

		for (int j = 1; j <= 3; j++) {
			a[i][j] = s[num] - '0';
			num++;
		}
	}
}
//��ȡ�������
PII getZero() {
	for (int i = 1; i <=3; i++) {

		for (int j = 1; j <= 3 ; j++) {
			if (a[i][j] == 0) return { i,j };
		}
	}

}
//������ת�����ַ���
string getS() {
	string s = "";
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			s += to_string(a[i][j]);
		}
	}
	return s;
}

//���������Ԫ��
void swapA(int ax,int ay,int bx,int by) {
	swap(a[ax][ay], a[bx][by]);
}

//����
bool bfs() {
	queue<string> q;
	string s = getS();
	q.push(s);
	vis[s] = true;
	cout<<"��ʼ״̬��" << s << '\n';
	while (!q.empty()) {
		
		s = q.front(); q.pop();
		
		if (s == "123456780") break;
		sToA(s);
		PII t = getZero();
		for (int i = 0; i < 4; i++) {
			int x = t.first + dx[i], y = t.second + dy[i];
			if (x <= 0 || y <= 0 || x > 3 || y > 3) continue;
			swapA(t.first, t.second, x, y);
			string s1	=getS();
			 if (!vis[s1]) {
				 q.push(s1);
				vis[s1] = true;
				pre[s1] = s;
			}
			 swapA(t.first, t.second, x, y);

		}


	}
	return s == "123456780";
	
}


/*ÿ����ʾ����ͼƬ*/
void mergeImage(Mat& dst, vector<Mat>& images)
{
	int imgCount = (int)images.size();

	if (imgCount <= 0)
	{
		printf("the number of images is too small\n");
		return;
	}

	//printf("imgCount = %d\n", imgCount);

	/*��ÿ��ͼƬ��СΪָ����С*/
	int rows = 150;
	int cols = 165;
	for (int i = 0; i < imgCount; i++)
	{
		images[i] = imread(adr[ord[i]]);
		
		/*
		cout << adr[ord[i]] << '\n';
		printf("i: %d size %d \n",i,images[i].size());
		*/
		resize(images[i], images[i], Size(cols, rows)); //ע������Size���������������ֱ�Ϊ����͸ߣ����Ӧcols���߶�Ӧrows
	}


	/*������ͼƬ�ĳߴ�
		�ߣ�rows * imgCount/3
		��cols * 3
	*/
	dst.create(rows * imgCount / 3, cols * 3, CV_8UC3);

	for (int i = 0; i < imgCount; i++)
	{
		images[i].copyTo(dst(Rect((i % 3) * cols, (i / 3) * rows, images[0].cols, images[0].rows)));
	}
}

//����������չʾ������ջ�����Ԫ��ȡ�������ظ������
void toProcess(Mat &dst) {
	int step = 0;
	while (st.size()>1) {
		
		string s = st.top(); st.pop();
		
		for (int i = 0; i < 9; i++) {
			ord[i] = s[i] - '0';
		}

		cout<<"��"<<++step<<"��  " << s << '\n';
		cout << "ͼƬ���:\n";
		for (int i = 0; i < 9; i++) {
			//images[i] = imread(adr[ord[i]]);
			cout << ord[i] << " ";
		}
		cout << "\n\n";
		mergeImage(dst, images);
		imshow("dst", dst); 
		waitKey(500);
	}
	string s = st.top(); st.pop();
	cout << "��" << ++step << "��  " << s << '\n';
	cout << "ͼƬ���:\n";
	for (int i = 0; i < 9; i++) {
		ord[i] = s[i] - '0';
		cout << ord[i] << " ";
	}
	cout << "\n";
	for (int i = 0; i < 9; i++) {
		images[i] = imread(adr[ord[i]]);
	}
	mergeImage(dst, images);
	imshow("dst", dst);

	waitKey(0);
}
void init() {
	int o[] = {2, 1, 4, 3, 5, 0, 8, 6, 7};
	vis.clear();
	for (int i = 0; i < 9; i++) {
		ord[i] = o[i];
	}
	printf("Ĭ�������2 1 4 3 5 0 8 6 7\n����ʾ����2 1 3 4 5 6 7 8 0\n");
	printf("�Ƿ�ʹ��Ĭ�ϵ�˳��( 0��1��)\n");
	int flag=0;
	scanf("%d", &flag);
	if (!flag) {
		for (int i = 0; i < 9; i++) {
			scanf("%d", &ord[i]);
		}
	}
	printf("��ǰ˳��\n");

	for (int i = 1, num = 0; i <= 3; i++) {
		for (int j = 1; j <= 3; j++, num++) a[i][j] = ord[num],cout<<ord[num]<<" ";
		
	}
	cout << "\n";
}

int main()
{
	
	Mat dst;
	
	adr[0] = "./yiyandingzhen/00.png";
	adr[1] = "./yiyandingzhen/01.png";
	adr[2] = "./yiyandingzhen/02.png";
	adr[3] = "./yiyandingzhen/03.png";
	adr[4] = "./yiyandingzhen/04.png";
	adr[5] = "./yiyandingzhen/05.png";
	adr[6] = "./yiyandingzhen/06.png";
	adr[7] = "./yiyandingzhen/07.png";
	adr[8] = "./yiyandingzhen/08.png";
	
	init();
	while (1) {
		if (bfs()) {
			cout << "�ɹ���\n";
			string s = "123456780";
			while (pre[s] != "") {
				st.push(s);
				s = pre[s];
			}
			st.push(s);
			toProcess(dst);
		}
		else {
			Mat img;
			img=imread("./yiyandingzhen/error.png",1);
			
			imshow("error",img);
			waitKey(0);
		}
		destroyAllWindows();
		init();

	}

	

	return 0;
}