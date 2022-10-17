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
vector<string> adr(10); //图片序号
vector<Mat> images(9);//图片的地址

//把字符串转换成数组
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
//获取零的坐标
PII getZero() {
	for (int i = 1; i <=3; i++) {

		for (int j = 1; j <= 3 ; j++) {
			if (a[i][j] == 0) return { i,j };
		}
	}

}
//把数组转换成字符串
string getS() {
	string s = "";
	for (int i = 1; i <= 3; i++) {
		for (int j = 1; j <= 3; j++) {
			s += to_string(a[i][j]);
		}
	}
	return s;
}

//交换数组的元素
void swapA(int ax,int ay,int bx,int by) {
	swap(a[ax][ay], a[bx][by]);
}

//宽搜
bool bfs() {
	queue<string> q;
	string s = getS();
	q.push(s);
	vis[s] = true;
	cout<<"初始状态：" << s << '\n';
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


/*每行显示三个图片*/
void mergeImage(Mat& dst, vector<Mat>& images)
{
	int imgCount = (int)images.size();

	if (imgCount <= 0)
	{
		printf("the number of images is too small\n");
		return;
	}

	//printf("imgCount = %d\n", imgCount);

	/*将每个图片缩小为指定大小*/
	int rows = 150;
	int cols = 165;
	for (int i = 0; i < imgCount; i++)
	{
		images[i] = imread(adr[ord[i]]);
		
		/*
		cout << adr[ord[i]] << '\n';
		printf("i: %d size %d \n",i,images[i].size());
		*/
		resize(images[i], images[i], Size(cols, rows)); //注意区别：Size函数的两个参数分别为：宽和高，宽对应cols，高对应rows
	}


	/*创建新图片的尺寸
		高：rows * imgCount/3
		宽：cols * 3
	*/
	dst.create(rows * imgCount / 3, cols * 3, CV_8UC3);

	for (int i = 0; i < imgCount; i++)
	{
		images[i].copyTo(dst(Rect((i % 3) * cols, (i / 3) * rows, images[0].cols, images[0].rows)));
	}
}

//把整个过程展示出来（栈里面的元素取出来，重复输出）
void toProcess(Mat &dst) {
	int step = 0;
	while (st.size()>1) {
		
		string s = st.top(); st.pop();
		
		for (int i = 0; i < 9; i++) {
			ord[i] = s[i] - '0';
		}

		cout<<"第"<<++step<<"步  " << s << '\n';
		cout << "图片序号:\n";
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
	cout << "第" << ++step << "步  " << s << '\n';
	cout << "图片序号:\n";
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
	printf("默认情况：2 1 4 3 5 0 8 6 7\n错误示例：2 1 3 4 5 6 7 8 0\n");
	printf("是否使用默认的顺序？( 0否，1是)\n");
	int flag=0;
	scanf("%d", &flag);
	if (!flag) {
		for (int i = 0; i < 9; i++) {
			scanf("%d", &ord[i]);
		}
	}
	printf("当前顺序：\n");

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
			cout << "成功了\n";
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