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
typedef pair<int, string> PIS;
int dx[4] = { -1,0,1,0 };
int dy[4] = { 0,1,0,-1 };
int ord[10] ;//ͼƬ���
vector<string> adr(10); 
vector<Mat> images(9);//ͼƬ�ĵ�ַ

//�ж��Ƿ��ܹ��ɰ�����
bool isPossible(string s) {
	int num = 0;
	for (int i = 0; i < 9; i++) {

		for (int j = i; j < 9; j++) {
			if (ord[i] > ord[j] && ord[i] != 0 && ord[j] != 0) num++;
		}
	}
	return num % 2==0;

}
//���ַ���ת��������

//������ת�����ַ���
string getS() {
	string s = "";
	for (int i = 0; i <9 ; i++) {		
			s += to_string(ord[i]);	
	}
	return s;
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
		int tx, ty;
		for (int i = 0; i < 9; i++) {
			if (s[i] == '0') {
				tx = i / 3, ty = i % 3;
				break;
			}
		}
		
		for (int i = 0; i < 4; i++) {
			string s1 = s;
			int x = tx + dx[i], y = ty + dy[i];
			if (x < 0 || y < 0 || x >=3 || y >=3) continue;
			swap(s1[tx*3+ty], s1[x*3+y]);
			
			 if (!vis[s1]) {
				q.push(s1);
				vis[s1] = true;
				pre[s1] = s;
			}

		}


	}
	return s == "123456780";
	
}

//A*�Ĺ�������
int getH(string s) {
	int res = 0;
	for (int i = 0; i < 9; i++) {
		if (s[i] != '0') {
			int t = s[i] - '1';
			res += abs(t / 3 - i / 3) + abs(t % 3 - i % 3);
		}
	}
	return res;
}
//A*�㷨
void Astar() {
	priority_queue<PIS,vector<PIS>,greater<PIS> > q;
	unordered_map<string, int> dis;
	q.push({ 0,getS() });
	while (!q.empty()) {
		PIS t = q.top(); q.pop();
		int w = t.first;
		string s = t.second;
		if (s == "123456780") break;
		int tx, ty;
		for (int i = 0; i < 9; i++) {
			if (s[i] == '0') {
				tx = i / 3, ty = i % 3;
				break;
			}
		}

		for (int i = 0; i < 4; i++) {
			int x = tx + dx[i], y = ty + dy[i];
			if (x < 0 || y < 0 || x >= 3 || y >= 3) continue;
			string s1 = s;
			swap(s1[tx * 3 + ty], s1[x * 3 + y]);
			if (dis.count(s1)==0||dis[s1]>dis[s]+1) {
			
				dis[s1] = dis[s] + 1;
				q.push({ getH(s1)+dis[s1],s1});
				pre[s1] = s;
			}

		}


	}

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

//����������չʾ������ջ�����Ԫ��ȡ���������
void toProcess(Mat &dst) {
	string s = "123456780";
	while (pre[s] != "") {
		st.push(s);
		s = pre[s];
	}
	st.push(s);
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
	 s = st.top(); st.pop();
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

//��ʼ����ǰ��һЩ����
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

	for (int i = 0; i < 9; i++) {
		 cout<<ord[i]<<" ";	
	}
	cout << "\n";
}

int main()
{
	
	Mat dst;
	string boy = "./yiyandingzhen/0";
	string girl = "./girl/0";
	string path = boy;
	printf("ѡ����ŮͼƬ��1��˧�磨0��\n");
	int flag = 1;
	scanf("%d", &flag);
	if (flag) path = girl;
	else path = boy;

	for (int i = 0; i < 9; i++) {
		adr[i] = path + to_string(i) + ".png";
	}
	
	while (1) {
		init();
		string s = getS();
		if (isPossible(s)) {
			printf("ѡ��ʹ��ʲô�㷨 0(BFS),1(Astart)\n");
			int flag = 0;
			scanf("%d", &flag);
			if (flag) {
				Astar();
				cout << "--------ʹ��Astart--------\n";
			}
			else {
				bfs();
				cout << "--------ʹ��BFS--------\n";
			}			
			toProcess(dst);
		}
		else {
			Mat img;
			img=imread("./yiyandingzhen/error.png",1);
			
			imshow("error",img);
			waitKey(0);
		}
		destroyAllWindows();
		

	}

	return 0;
}