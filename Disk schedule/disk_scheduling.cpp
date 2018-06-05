#include<stdio.h> 
#include<iostream>
#include<string.h>
#include<math.h> 

const int MAXQUEUE=200; //定义请求队列最大长度
using namespace std;

//磁道号请求结构体定义 
typedef struct TRACK_Node
{ 
	int iGo;		//要访问的磁道号
	int iBeVisited;	//磁道是否已经访问标志（1：已访问；0：未访问）
}TRACK; 


TRACK queue[MAXQUEUE]; //磁道号请求队列数组


int iReqNum=0;			//磁道访问请求数
int iStart=0;			//磁头初始位置
int iNow=0;				//磁头当前位置
int iSum=0;				//总移动磁道数
int iInput;				//用户当前输入的整数
char sFileName[20];		//文件名

void Init();			//初始化函数
void Reset();			//重置访问标志、磁头当前位置、总移动磁道数
int ReadTrackFile();	//读入磁道号流文件
void FCFS();			//先来先服务调度算法
void SSTF();			//最短寻道优先调度算法
void SCAN();			//电梯调度算法
void Version();			//显示版权信息函数 
void Hint();			//显示实验提示信息函数

 
void Init()	//初始化函数
{
	int i; 
	
	for(i=0;i<MAXQUEUE;i++)
	{ 
		queue[i].iGo=-1;		//设置要访问的磁道号为不可能的数-1，以区别正常请求磁道号
		queue[i].iBeVisited=0;	//设置磁道是否已经访问标志为0：未访问
	} 
	
} //void Init()


void Reset()	//重置访问标志、磁头当前位置、总移动磁道数
{
	int i; 
	
	for(i=0;i<iReqNum;i++)
	{ 
		queue[i].iBeVisited=0; 	//设置磁道是否已经访问标志为0：未访问
	} 

	printf( "\n	请输入磁头的初始磁道号（整数）:  "); 
	scanf("%d",&iStart);	//从标准输入获取用户当前输入的磁头初始位置
	iNow=iStart;			//磁头当前位置
	iSum=0;					//总移动磁道数 
	
} //void Reset()

 
int ReadTrackFile() //读入磁道号流文件
{ 
	FILE *fp; 	
	int iTemp; 
	char cHead;
	
	cout<<"	\n	请输入要打开的磁道号流（文本）文件名（注意：包括路径名和后缀名）:  ";
	cin>>sFileName; 		//从标准输入获取用户当前输入的文件名
	
	if((fp=fopen(sFileName,"r"))==NULL)
	{ 
		cout<<endl<<"	错误：磁道号流文件 "<<sFileName<<" 打不开，请检查文件名和路径!!"<<endl;
		return -1;
		
	} 
	else
	{
		cHead=fgetc(fp);
		while(cHead==' '||cHead==10||cHead==9)	//过滤空格、换行和TAB字符
		{
			cHead=fgetc(fp);
		}
		if(cHead==EOF)
		{
			printf("	错误：磁道号流文件：%s 为空!!\n",sFileName );
			return -1;
		}
		else
		{
			fseek( fp, -1, SEEK_CUR); 
			while(!feof(fp))	//将文件中输入的磁道号依次存入磁道号请求队列数组
			{ 
				fscanf(fp,"%d ",&iTemp); 
				queue[iReqNum].iGo=iTemp; 
				iReqNum++;	//磁道访问请求数 
			} 
		}
	}	//if((fp=fopen(sFileName,"r"))==NULL)

	return 0;

} //void ReadTrackFile()

 
void FCFS() //先来先服务调度算法
{
	int i; 
	
	Reset();	//重置访问标志、磁头当前位置、总移动磁道数
	
	cout<<endl<<"---------------------------------------------"<<endl; 

	cout<<"	先来先服务调度算法的调度结果:  "<<endl<<endl; 
	cout<<"	    初始磁道号:  "<<iStart<<endl; 
	cout<<"	序号  下一磁道号  移动的磁道数"<<endl; 
	
	for(i=0;i<iReqNum;i++)
	{ 
		//输出信息摸拟访问请求的磁道
		cout<<"      	"<<i+1<<"      "<<queue[i].iGo<<"      "<<abs(queue[i].iGo-iNow)<<endl;
		iSum+=abs(queue[i].iGo-iNow);		//累加总移动磁道数
		iNow=queue[i].iGo;					//移动磁头当前位置为当前访问磁道号
	} 
	cout<<endl<<"	总调度次数:  "<<iReqNum<<endl;
	cout<<endl<<"	总移动磁道数:  "<<iSum<<endl;
	printf("\n	平均移动磁道数:  %.2f\n\n",(float)iSum / (float)iReqNum);

} //void FCFS()

 

 
void SCAN()	//电梯调度算法 
{ 
	int i,j; 
	int iNext;		//即将访问的下一未访问磁道号在数组队列queue中的下标 
	int iMinMove;	//当前方向上最短寻道距离

	cout<<endl<<"---------------------------------------------"<<endl; 
	
	cout<<endl<<"	请选择磁头初始方向（1 OR 2）："<<endl<<endl;
	cout<<"	1  磁头初始向内"<<endl<<endl<<"	2  磁头初始向外"<<endl<<endl<<"	";

	cin>>iInput;	//从标准输入获取用户当前输入的整数
	
	switch(iInput)	//用户当前输入的整数
	{
	case 1:			//磁头初始向内
		Reset();	//重置访问标志、磁头当前位置、总移动磁道数
		
		cout<<endl<<"---------------------------------------------"<<endl; 

		cout<<"	电梯调度算法——磁头初始向内的调度结果:  "<<endl<<endl; 
		cout<<"	    初始磁道号:  "<<iStart<<endl; 
		cout<<"	序号  下一磁道号  移动的磁道数"<<endl; 

		for(i=0;i<iReqNum;i++)
		{
			iMinMove=9999; 
			iNext=-1; 
			for(j=0;j<iReqNum;j++)	//寻找当前方向上寻道距离最短的未访问磁道号在数组队列queue中的下标
			{ 
				if((queue[j].iBeVisited==0)&&(queue[j].iGo>=iNow))
				{
					if(abs(queue[j].iGo-iNow)<iMinMove)
					{ 
						iNext=j; 
						iMinMove=abs(queue[j].iGo-iNow); 
					} //if(abs(queue[j].iGo-iNow)<iMinMove)
				} //if((queue[j].iBeVisited==0)&&(queue[j].iGo>=iNow))
			} //for(j=0;j<iReqNum;j++)

			if(iNext!=-1)
			{
				//输出信息摸拟访问请求的磁道
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//累加总移动磁道数
				iNow=queue[iNext].iGo;					//移动磁头当前位置为当前访问磁道号
				queue[iNext].iBeVisited=1;				//设置磁道是否已经访问标志为1：已访问
			} //if(iNext!=-1)

			else	//掉头向外
			{ 
				for(j=0;j<iReqNum;j++)		//寻找当前方向上寻道距离最短的未访问磁道号在数组队列queue中的下标
				{ 
					if((queue[j].iBeVisited==0)&&(queue[j].iGo<iNow))
					{ 
						if(abs(queue[j].iGo-iNow)<iMinMove)
						{
							iNext=j; 
							iMinMove=abs(queue[j].iGo-iNow); 
						} 
					} 
				} //for(j=0;j<iReqNum;j++)

				//输出信息摸拟访问请求的磁道
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//累加总移动磁道数
				iNow=queue[iNext].iGo;				 	//移动磁头当前位置为当前访问磁道号
				queue[iNext].iBeVisited=1;				//设置磁道是否已经访问标志为1：已访问

			} //if(iNext!=-1)
		} //for(i=0;i<iReqNum;i++)

		cout<<endl<<"	总调度次数:  "<<iReqNum<<endl;
		cout<<endl<<"	总移动磁道数:  "<<iSum<<endl;
		printf("\n	平均移动磁道数:  %.2f\n\n",(float)iSum / (float)iReqNum);

		break;

	case 2:	  //磁头初始向外
	  
		Reset();	//重置访问标志、磁头当前位置、总移动磁道数
		
		cout<<endl<<"---------------------------------------------"<<endl; 

		cout<<"	电梯调度算法——磁头初始向外的调度结果:  "<<endl<<endl; 
		cout<<"	    初始磁道号:  "<<iStart<<endl; 
		cout<<"	序号  下一磁道号  移动的磁道数"<<endl; 

		for(i=0;i<iReqNum;i++)
		{
			iMinMove=9999; 
			iNext=-1; 
			for(j=0;j<iReqNum;j++)	//寻找当后方向上寻道距离最短的未访问磁道号在数组队列queue中的下标
			{ 
				if((queue[j].iBeVisited==0)&&(queue[j].iGo<=iNow))
				{
					if(abs(queue[j].iGo-iNow)<iMinMove)
					{ 
						iNext=j; 
						iMinMove=abs(queue[j].iGo-iNow); 
					} //if(abs(queue[j].iGo-iNow)<iMinMove)
				} //if((queue[j].iBeVisited==0)&&(queue[j].iGo>=iNow))
			} //for(j=0;j<iReqNum;j++)

			if(iNext!=-1)
			{
				//输出信息摸拟访问请求的磁道
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//累加总移动磁道数
				iNow=queue[iNext].iGo;					//移动磁头当前位置为当前访问磁道号
				queue[iNext].iBeVisited=1;				//设置磁道是否已经访问标志为1：已访问
			} //if(iNext!=-1)

			else	//掉头向内
			{ 
				for(j=0;j<iReqNum;j++)		//寻找当前方向上寻道距离最短的未访问磁道号在数组队列queue中的下标
				{ 
					if((queue[j].iBeVisited==0)&&(queue[j].iGo>iNow))
					{ 
						if(abs(queue[j].iGo-iNow)<iMinMove)
						{
							iNext=j; 
							iMinMove=abs(queue[j].iGo-iNow); 
						} 
					} 
				} //for(j=0;j<iReqNum;j++)

				//输出信息摸拟访问请求的磁道
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//累加总移动磁道数
				iNow=queue[iNext].iGo;				 	//移动磁头当前位置为当前访问磁道号
				queue[iNext].iBeVisited=1;				//设置磁道是否已经访问标志为1：已访问

			} //if(iNext!=-1)
		} //for(i=0;i<iReqNum;i++)

		cout<<endl<<"	总调度次数:  "<<iReqNum<<endl;
		cout<<endl<<"	总移动磁道数:  "<<iSum<<endl;
		printf("\n	平均移动磁道数:  %.2f\n\n",(float)iSum / (float)iReqNum);
		
		break;
					
	default:
		printf("\n	输入错误!!\n\n");
		return;
	}	//switch(iInput)

} //void SCAN()

void SSTF() //最短寻道优先调度算法
{ 
	int i,j; 
	int iNext;		//即将访问的下一未访问磁道号在数组队列queue中的下标 
	int iMinMove;	//当前方向上最短寻道距离
	Reset();	//重置访问标志、磁头当前位置、总移动磁道数
	
	cout<<endl<<"---------------------------------------------"<<endl; 

	cout<<"	最短寻道优先调度算法的调度结果:  "<<endl<<endl; 
	cout<<"	    初始磁道号:  "<<iStart<<endl; 
	cout<<"	序号  下一磁道号  移动的磁道数"<<endl; 
    for(i=0;i<iReqNum;i++)
	{
	     iMinMove=9999; 
			iNext=-1; 
			for(j=0;j<iReqNum;j++)	//寻找当剩下的上寻道距离最短的未访问磁道号在数组队列queue中的下标
			{ 
				if(queue[j].iBeVisited==0)
				{
					if(abs(queue[j].iGo-iNow)<iMinMove)
					{ 
						iNext=j; 
						iMinMove=abs(queue[j].iGo-iNow); 
					} 
				}
			}
	   //输出信息摸拟访问请求的磁道
		cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
		iSum+=abs(queue[iNext].iGo-iNow);	 	//累加总移动磁道数
		iNow=queue[iNext].iGo;					//移动磁头当前位置为当前访问磁道号
		queue[iNext].iBeVisited=1;				//设置磁道是否已经访问标志为1：已访问
     
	}
        cout<<endl<<"	总调度次数:  "<<iReqNum<<endl;
		cout<<endl<<"	总移动磁道数:  "<<iSum<<endl;
		printf("\n	平均移动磁道数:  %.2f\n\n",(float)iSum / (float)iReqNum);
		

}



void main() 
{ 
	int i; 
	bool bGoOn;		//是否继续磁盘调度算法模拟的标志
	char sGoOn[1];	//用户输入是否继续磁盘调度算法模拟的字母：y、Y、N、n

	
	Init();			//初始化函数
	
	if(ReadTrackFile()==-1)    //读入磁道号流文件
		{
			printf("	读入磁道号流文件失败!!\n\n");
		}
		else
		{
			bGoOn= true;
			while (bGoOn)
			{
				cout<<endl<<"---------------------------------------------"<<endl;
				cout<<"	从磁道号流文件 "<<sFileName<<" 所读入的磁道号流如下:"<<endl<<endl<<"	"; 
				for(i=0;i<iReqNum;i++)
				{ 
					cout<<queue[i].iGo<<" "; 
				} 
				cout<<endl<<endl<<"	请求数为:  "<<iReqNum<<endl<<endl; 

				iInput=-1;		//用户输入的整数以选择算法

                cout<<endl<<"	请输入算法编号（1 OR 2 OR 3）选择调度算法："<<endl<<endl;
				cout<<"	1  先来先服务调度算法"<<endl<<endl<<"	2  电梯调度算法"<<endl<<endl<<"	3  最短寻道优先调度算法 	"<<endl<<endl<<"	"; 

				cin>>iInput;		//从标准输入获取用户当前输入的整数

				switch(iInput)		//用户输入的整数以选择算法
				{
				case 1:
					FCFS();		//先来先服务调度算法
					break;

				case 2:
					SCAN();		//电梯调度算法
					break;
					
			    case 3:
					SSTF();		//最短寻道优先调度算法
					break;
				default:
					printf("\n	输入的算法编号错误!!\n\n");
					return;

				}	//switch(iInput)
				
				bGoOn= false;
				strcpy(sGoOn," ");
				
				cout<<"	要继续进行磁盘调度算法模拟吗?(Y/N)	"<<endl<<"	";
				cin>>sGoOn;		
				bGoOn=(sGoOn[0]=='y'||sGoOn[0]=='Y');

			}  //while bGoOn

		}	//if(ReadTrackFile()==-1)	
	
} //void main() 
