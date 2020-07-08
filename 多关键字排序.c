#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include<iostream>

using namespace std;

#define MAX_NUM_OF_KEY 8            //关键字项数的最大基数，本设计中只用到5
#define RADIX 10                    //关键字基数，此时是10进制整数的基数
#define MAX_SPACE 1000
typedef int KeyType;
#define CHINESE 1      //语文
#define MATH 2         //数学
#define ENGLISH 3      //英语
#define XX 4           //x科  
#define TOTAL 5        //总分

typedef struct{
	KeyType keys[MAX_NUM_OF_KEY+1];            //关键字
	int next;                                  
}SLCell;                                       //静态链表的节点类型

typedef struct{
	SLCell r[MAX_SPACE+1];
	int keynum;                                //记录的当前关键字个数，这里是科目
	int recnum;                                //静态链表的当前长度
}SLList_LSD;                                   //静态链表类型  

typedef int ArrType[RADIX];                    //指针数组内容 

void InitSLList(SLList_LSD& L, int num);								//链表初始化
void Randdata_LSD(SLList_LSD& L);										//生成随机数据
void printSLList(SLList_LSD L);											//打印初始链表
void output_desc_order(SLList_LSD L_LSD, int num);						//打印排序后的链表（打印降序排列）
void output_asc_order(SLList_LSD L_LSD);								//打印排序后的链表（打印升序排列）
int ord(int score, int i);												//关键字映射
void Distribute(SLCell r[], int i, ArrType& f, ArrType& e, int key);	//分配
void Collect(SLCell r[], int i, ArrType f, ArrType e);					//收集
void RadixSort(SLList_LSD& L, short key);								//基数排序
void BubbleSort(SLCell r[MAX_SPACE + 1], int num);						//冒泡排序

//初始化静态链表 
void InitSLList(SLList_LSD &L, int num)     
{
	L.recnum=num;								//num为关键字个数
	L.keynum=5;
}

//基数排序LSD
int ord(int score, int i)			//将记录中第i个关键字映射到【1...RADIX】		
{
	int j;
	switch(i)
	{
		case 1: j=score%10; break;
		case 2: j=score/10%10; break;
		case 3: j=score/100%10; break;
		case 4: j=score/1000%10; break;
		default: j=score/10000;
	}
	return j;
}

void Distribute(SLCell r[], int i, ArrType &f, ArrType &e, int key)		//分配
{
	int j, p;

	for(j=0; j<RADIX; j++)
	{ 
		f[j] = 0; 
		e[j] = 0; 
	}//各子表初次化为空表
	
	for(p=r[0].next; p; p=r[p].next)           //最后r[L.recnum].next是以0结束的
	{		
		j = ord(r[p].keys[key]+r[p].keys[5]*100, i); 	//将总分和关键字合成一个新的关键字进行排序 
		if(!f[j])
			f[j] = p;
		else
			r[e[j]].next = p;
		e[j] = p;      //将p所指的结点插入第j个子表
	}
}

void Collect(SLCell r[], int i, ArrType f, ArrType e)					//收集
{
	int j=0, t=0;
	for(j=0; !f[j]; j++);							//找到第一个非空子表
	r[0].next=f[j];  
	t=e[j];										//r[0].next指向第一个非空子表中的第一个结点
	while(j<RADIX)
	{
		for(j=j+1; j<RADIX-1 && !f[j]; j++);		//找下一个非空子表
		if(f[j] && j<RADIX)
		{
			r[t].next=f[j]; 
			t=e[j];
		}										//连接两个非空子表
	}
	r[t].next=0;
}
 
void RadixSort(SLList_LSD &L, short key)			//用基数排序分配和收集         
{
	ArrType f, e; 
	int i;
	for(i=0; i<L.recnum;i++)  
		L.r[i].next = i+1;						//将L改造成静态链表
	L.r[L.recnum].next = 0;
	for(i=1; i<=L.keynum; i++)				//按最低位优先依次对各关键字进行分配和收集
	{	
		Distribute(L.r,i,f,e,key);				//第i趟分配
		Collect(L.r,i,f,e);						//第i趟收集
	}
}

void printSLList(SLList_LSD L)		//打印初始链表
{
	int i, j;
	for(i = 1; i <= L.recnum; i++)
	{
		for (j = 1; j <= 5; j++)
		{
			printf("%d\t", L.r[i].keys[j]);
		}
    	printf("\n");
	}
}

void output_desc_order(SLList_LSD L_LSD, int num)		//打印排序后的链表（打印降序排列）
{
	int arr[MAX_SPACE+1][5];
	int i, j, t, p, l, k;
	
	//把链表中的数据存入数组中
	for (p = L_LSD.r[0].next, i = 0; p && i < num; p = L_LSD.r[p].next, i++)
	{
		for (l = 1, j = 0; l <= 5 && j < 5; l++, j++)
			arr[i][j] = L_LSD.r[p].keys[l];
	}
		
		int rows = num;
		int cols = sizeof(arr[0]) / sizeof(arr[0][0]);
		
		for (i = 0; i < rows / 2; i++)					//上下对称反转
		{
			for (j = 0; j < cols; j++) 
			{
				t = arr[rows-1-i][j];
				arr[rows-1-i][j] = arr[i][j];
				arr[i][j] = t;
			}
		}
		for (i = 0; i < num; i++)						//打印数组内容
		{
			for (j = 0; j < 5; j++)
				printf("%d\t", arr[i][j]);
			printf("\n");
		}
}

void output_asc_order(SLList_LSD L_LSD)					//打印排序后的链表（打印升序排列）
{
	for (int p = L_LSD.r[0].next; p; p = L_LSD.r[p].next)
	{
		for (int i = 1; i <= 5; i++)
			printf("%d\t", L_LSD.r[p].keys[i]);
		printf("\n");
	}
}

//生成随机数据
void Randdata_LSD(SLList_LSD &L)						
{
	int i, j, sum, score, countID=1;
	for (i = 1; i <= L.recnum; i++)
	{
		sum = 0;
		for (j = 1; j <= 4; j++)
		{
			score = rand() % 101;
			if (score < 60)								//if在这里是为了提高整体成绩,不让太多成绩低于60分
			{
				score = rand() % 101;
			}
			L.r[i].keys[j] = score;
			sum += score;
		}
		L.r[i].keys[5] = sum;
		countID++;
	}
}

//冒泡排序 
void BubbleSort(SLCell r[MAX_SPACE+1], int num, int key)		//冒泡排序
{
	for (int i = 0; i < num; i++)						//next初始化
		r[i].next = i + 1;
	r[num].next = 0;
	int head, end, pre, p, temp;							//end用来记录排好序的最后一个元素的位置，pre p分别为前驱和后当前
	head = 0;
	end = 0;											//排序前end指向链尾NULL
	pre = r[head].next;
	p = r[pre].next;
	
	while (r[head].next != end)							//若head指向结点的next为end，结束循环
	{
		pre = r[head].next;								//pre结点总是从链表的头结点开始
		p = r[pre].next;									//p结点指向pre结点的下一节点

		while (r[pre].next != end)						//若相等，表示已到链尾
		{
			int n = 5;
			if (r[pre].keys[n] == r[p].keys[n] && n >= 1)	//当前关键字相等（总分），则比较上一关键字
			{
				n = key;
			}
			if (r[pre].keys[n] > r[p].keys[n])				//交换数据域（5个关键字全部交换） 
			{
				for (int i = 1; i < 6; i++)
				{
					temp = r[p].keys[i];
					r[p].keys[i] = r[pre].keys[i];
					r[pre].keys[i] = temp;
				}
			}
			pre = p;
			p = r[p].next;
		}
		end = pre;											//end指向最后排好序的结点
	}	
}

int main()
{
	int i, p, key, way, num, out, cont;
	char go;
	SLList_LSD L_LSD;
	do
	{
		system("CLS");
		printf("=============多关键字排序之学生成绩排序=============\n");
		printf("*输入参与排序的学生总数：");
		scanf("%d", &num);
		printf("*请选择排序方式（1.冒泡排序  2.基数排序）：");
		scanf("%d", &way);
		printf("*请选择输出顺序（1.升序排列  2.降序排列）：");
		scanf("%d", &out);
		do
		{
			if (way == 1)
			{
				InitSLList(L_LSD, num);
				Randdata_LSD(L_LSD);
				printf("开始随机生成学生数据..."); 
				printf("\n语文\t数学\t英语\tX科\t总分\n");
				printSLList(L_LSD);
				printf("\n*总分相同时优先录取（1:语文 2:数学 3:英语 4:X科）：");
				scanf("%d", &key);
				clock_t start_time = clock();
				BubbleSort(L_LSD.r, num, key);
				clock_t end_time = clock();
				//printf("*建立完毕，是否输出排序结果(Y/N)？");
				//getchar();
				
				printf("-------------------------------------------------------\n");
				printf("----------------------排序结果--------------------------\n");
				printf("语文\t数学\t英语\tX科\t总分 \n");
				if (out == 1)
					output_asc_order(L_LSD);
				else if (out == 2)
					output_desc_order(L_LSD, num);
				cout << "Running time is: " << (double)(end_time - start_time) / CLOCKS_PER_SEC * 1000 << "s" << endl;
				
			}
			else if (way == 2)
			{
				InitSLList(L_LSD, num);
				Randdata_LSD(L_LSD);
				printf("开始随机生成学生数据..."); 
				printf("\n语文\t数学\t英语\tX科\t总分\n");
				printSLList(L_LSD);
				printf("\n*总分相同时优先录取（1:语文 2:数学 3:英语 4:X科）：");
				scanf("%d", &key);
				clock_t start_time = clock();
				RadixSort(L_LSD, key);
				clock_t end_time = clock();
				//printf("*建立完毕，是否输出排序结果(Y/N)？");
				//getchar();
				scanf("%c", &go);
				
				printf("-------------------------------------------------------\n");
				printf("----------------------排序结果--------------------------\n");
				printf("语文\t数学\t英语\tX科\t总分 \n");
				if (out == 1)
					output_asc_order(L_LSD);
				else if (out == 2)
					output_desc_order(L_LSD, num);
				cout << "Running time is: " << (double)(end_time - start_time) / CLOCKS_PER_SEC << "s" << endl;	
			}
			else
				printf("*输入的操作不存在，请重新输入（1或者2）：");
		} while (way != 1 && way != 2);
		printf("\n*是否进行新的排序：（输入1继续,输入其他终止）：");
		scanf("%d", &cont);
	} while (cont == 1);
	
	return 0;
}


  
