#include<stdio.h> 
#include<string.h> 
//#include<iostream.h> 
#include <iostream>
using namespace std;
const int MAXPCB=200;	//定义最大进程数 


//定义进程控制块PCB结构体类型 
typedef struct PCB_Node{ 
	char sName[20];		//进程名
	int iStatus;		//进程状态(1就绪 2等待 3运行) 当前程序中暂时全部默认为就绪处理
	int iRunTime; 		//进程运行时间（纳秒）
	int iPriority; 		//进程优先数(数字越小优先级越高) 
	int iBeFinished; 	//进程是否执行完成标志（1：已完成；0：末完成）
	int iWaitTime; 		//进程等待时间（纳秒）
}PCB,PCB1; 

PCB pcbs[MAXPCB];		//PCB数组队列 
PCB1 pcbs1;
int iPCBNum;			//实际进程数
char sFileName[20];		//进程流文件名

void InitPCB();			//PCB初始化函数
int ReadPCBFile();		//读进程流文件数据，存入PCB数组队列pcbs，并输出
void ReSetPCB();		//重置PCB完成标志、等待时间,以供另一个调度算法使用，并输出所读入的进程数据
void FIFO();			//先进先出调度算法
void Priority();		//优先级调度算法
void RR();				//时间片轮转调度算法
void SPF();				//短进程优先调度算法
void Hint();			//显示实验提示信息函数
void Version();			//显示版权信息函数


void InitPCB()			//PCB初始化函数
{ 
	int i; 

	for(i=0;i<MAXPCB;i++)
	{ 
		strcpy(pcbs[i].sName,""); 
		pcbs[i].iStatus=-1; 
		pcbs[i].iRunTime=-1; 
		pcbs[i].iPriority=-1; 
		
		pcbs[i].iBeFinished=0; 
		pcbs[i].iWaitTime=0; 
	} //for(i=0;i<MAXPCB;i++)
	
	iPCBNum=0; 
} //void InitPCB()


int ReadPCBFile()	//读进程流文件数据，存入PCB数组队列pcbs，并输出
{ 
	FILE *fp; 
	int i; 
	char cHead;
	
	cout<<"	请输入要打开的进程流（文本）文件名（注意：包括路径名和后缀名）:  "; 
	cin>>sFileName; 

	if((fp=fopen(sFileName,"r"))==NULL)
	{ 
		cout<<"	错误：进程流文件 "<<sFileName<<" 打不开,请检查文件名和路径!!"<<endl; 
		return -1;
	} 
	else	//if((fp=fopen(sFileName,"r"))==NULL)
	{
		cHead=fgetc(fp);
		while(cHead==' '||cHead==10||cHead==9)	//过滤空格、换行和TAB字符
		{
			cHead=fgetc(fp);
		}
		if(cHead==EOF)
		{
			printf("	错误：进程流文件：%s 为空!!\n",sFileName );
			return -1;
		}
		else	//if(cHead==EOF)
		{
			fseek( fp, -1, SEEK_CUR); 
			while(!feof(fp))
			{ 
				fscanf(fp,"%s %d %d %d",pcbs[iPCBNum].sName,&pcbs[iPCBNum].iStatus,&pcbs[iPCBNum].iRunTime,&pcbs[iPCBNum ].iPriority); 
				iPCBNum++; 
			}	//while(!feof(fp))
			
			//输出所读入的进程数据
			cout<<endl<<"	从文件 "<<sFileName<<" 读入的进程数据："<<endl<<endl; 
			cout<<"	进程名  进程状态  运行时间  优先数"<<endl; 
			for(i=0;i<iPCBNum;i++)
			{ 
				cout<<"	 "<<pcbs[i].sName<<"	  "<<pcbs[i].iStatus<<"	  "<<pcbs[i].iRunTime<<"		  "<<pcbs[i].iPriority<<endl; 
			} 
			cout<<"	进程总数："<<iPCBNum<<endl; 

			return 0;
		}	//if(cHead==EOF)		
		 
	}	//if((fp=fopen(sFileName,"r"))==NULL)	

}// int ReadPCBFile()

 
void ReSetPCB()	//重置PCB完成标志、等待时间,以供另一个调度算法使用，并输出所读入的进程数据
{ 
	int i; 
	
	//输出所读入的进程数据 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<endl<<"	从文件 "<<sFileName<<" 读入的进程数据："<<endl<<endl; 
	cout<<"	进程名  进程状态  运行时间  优先数"<<endl; 
	for(i=0;i<iPCBNum;i++)
	{ 
		pcbs[i].iBeFinished=0; 
		pcbs[i].iWaitTime=0; 
		cout<<"	 "<<pcbs[i].sName<<"	  "<<pcbs[i].iStatus<<"	  "<<pcbs[i].iRunTime<<"		  "<<pcbs[i].iPriority<<endl; 
	}
	cout<<"	进程总数："<<iPCBNum<<endl; 

} //void ReSetPCB()

 
void FIFO() //先进先出调度算法
{ 
	int i,j; 
	int iSum; //总等待时间

	//输出先进先出调度算法执行流 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<"	先进先出调度算法执行流:"<<endl<<endl; 
	cout<<"	序号 进程名 运行时间 等待时间"<<endl; 

	iSum=0; 

	for(i=0;i<iPCBNum;i++)
	{ 
		cout<<"	 "<<i+1<<"	 "<<pcbs[i].sName<<"	 "<<pcbs[i].iRunTime<<"	 "<<pcbs[i].iWaitTime<<endl; 
		//输出信息摸拟进程运行
		iSum+=pcbs[i].iWaitTime;	//累加总等待时间 
		
		for(j=i+1;j<iPCBNum;j++)
		{ 
			pcbs[j].iWaitTime+=pcbs[i].iRunTime; 
		} 
	} 

	cout<<"	总调度次数:"<<iPCBNum<<endl;
	cout<<"	总等待时间:"<<iSum<<endl;
	printf("	平均等待时间  %.2f\n",(float)iSum / (float)iPCBNum);
 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 

} //void FIFO()

 
void Priority()	//优先级调度算法
{ 
  	int i,j; 
	int iSum; //总等待时间
   
	for(i=0;i<iPCBNum-1;i++){
		for(j=0;j<iPCBNum-1-i;j++){
			if(pcbs[j].iPriority>pcbs[j+1].iPriority){
			   pcbs1=pcbs[j];
			   pcbs[j]=pcbs[j+1];
			   pcbs[j+1]=pcbs1;
			}
		}
	}

	//输出优先级调度算法执行流 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<"	优先级调度算法执行流:"<<endl<<endl; 
	cout<<"	序号 进程名 运行时间 优先级"<<endl; 

	iSum=0; 

	for(i=0;i<iPCBNum;i++)
	{ 
		cout<<"	 "<<i+1<<"	 "<<pcbs[i].sName<<"	 "<<pcbs[i].iRunTime<<"	 "<<pcbs[i].iPriority<<endl; 
		//输出信息摸拟进程运行
		iSum+=pcbs[i].iWaitTime;	//累加总等待时间 
		
		for(j=i+1;j<iPCBNum;j++)
		{ 
			pcbs[j].iWaitTime+=pcbs[i].iRunTime; 
		} 
	} 

	cout<<"	总调度次数:"<<iPCBNum<<endl;
	cout<<"	总等待时间:"<<iSum<<endl;
	printf("	平均等待时间  %.2f\n",(float)iSum / (float)iPCBNum);
 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	
} //void Priority()

 
void RR()	//时间片轮转调度算法
{ 
	int i;
	int iNotEnded=1;		//循环控制标志：不是所有进程都已经获得足够时间片运行完成，需要继续轮转。
	int iNum;				//到目前为止末执行完的进程数
	int iRound=0;			//轮转周期数
	int iSum=0;				//总时间片数
	float fBlockTime=10;	//时间片的长度（纳秒）
	
	cout<<endl<<"	请输入时间片的长度（纳秒）："<<endl<<"	";
	cin>>fBlockTime;

	cout<<endl<<"---------------------------------------------------------------"<<endl;
	cout<<"	时间片轮转调度执行流（时间片的长度为："<<fBlockTime<<"纳秒）:"<<endl; 

	while(iNotEnded==1)	//不是所有进程都已经获得足够时间片运行完成，需要继续轮转。
	{
		iNotEnded=0; 
		iNum=0; 
		
		for(i=0;i<iPCBNum;i++)	//统计到目前为止末执行完的进程数iNum
		{
			if(pcbs[i].iBeFinished==0)
			{ 
				iNum++;
				
			} //if(pcbs[i].iBeFinished==0)
		} //for(i=0;i<iPCBNum;i++)
		
		if(iNum>0)
		{
			iNotEnded=1;

			iRound++;	//累加轮转周期数
			cout<<endl<<"	第"<<iRound<<"轮：";

			for(i=0;i<iPCBNum;i++)
			{ 
				if(pcbs[i].iBeFinished==0)
				{
					cout<<pcbs[i].sName<<" ";	//输出信息摸拟进程运行					


					iSum++;	//累加总时间片数 
					if(pcbs[i].iRunTime<=fBlockTime*(iRound))
					//i进程在本轮获得一个时间片后能够运行完成
					{ 
						pcbs[i].iBeFinished=1; 

					} //if(pcbs[i].iRunTime<=fBlockTime*(iRound+1))

				}//if(pcbs[i].iBeFinished==0) 

			} //for(i=0;i<iPCBNum;i++)		

		} //if(iNum>0)

	} //while(iNotEnded==1)
	
	cout<<endl<<"	轮转周期数:"<<iRound<<"	总时间片数:"<<iSum<<endl;
	cout<<endl<<"---------------------------------------------------------------"<<endl; 

}//void RR() 

 
void SPF()	//短进程优先调度算法
{ 
    int i,j; 
	int iSum; //总等待时间
   
	for(i=0;i<iPCBNum-1;i++){
		for(j=0;j<iPCBNum-1-i;j++){
			if(pcbs[j].iRunTime>pcbs[j+1].iRunTime){
			   pcbs1=pcbs[j];
			   pcbs[j]=pcbs[j+1];
			   pcbs[j+1]=pcbs1;
			}
		}
	}

	//输出短进程优先调度算法执行流 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<"	短进程优先级调度算法执行流:"<<endl<<endl; 
	cout<<"	序号 进程名 运行时间 优先级"<<endl; 

	iSum=0; 

	for(i=0;i<iPCBNum;i++)
	{ 
		cout<<"	 "<<i+1<<"	 "<<pcbs[i].sName<<"	 "<<pcbs[i].iRunTime<<"	 "<<pcbs[i].iPriority<<endl; 
		//输出信息摸拟进程运行
		iSum+=pcbs[i].iWaitTime;	//累加总等待时间 
		
		for(j=i+1;j<iPCBNum;j++)
		{ 
			pcbs[j].iWaitTime+=pcbs[i].iRunTime; 
		} 
	} 

	cout<<"	总调度次数:"<<iPCBNum<<endl;
	cout<<"	总等待时间:"<<iSum<<endl;
	printf("	平均等待时间  %.2f\n",(float)iSum / (float)iPCBNum);
 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 

} //void SPF ()



void main()		//主函数
{ 
	int iInput;		//用户输入的整数以选择算法 
	bool bGoOn;		//是否继续进程调度算法模拟的标志
	char sGoOn[1];	//用户输入是否继续进程调度算法模拟的字母：y、Y、N、n

	
	InitPCB();		//PCB初始化函数
	
	bGoOn= true;
	strcpy(sGoOn," ");	
		
	if(ReadPCBFile()==0)	//标志 读进程流文件数据函数 执行是否正确
	{
		while (bGoOn)
		{
			cout<<endl<<"	请输入算法编号（1 OR 2 OR 3 OR 4）选择进程调度算法："<<endl<<endl;	
			cout<<"	1  先进先出调度算法"<<endl<<endl<<"	2  优先级调度算法"<<endl<<endl;
			cout<<"	3  时间片轮转调度算法"<<endl<<endl<<"	4  短进程优先调度算法"<<endl<<endl<<"	";

			cin>>iInput;		//从标准输入获取用户输入的整数以选择算法
			switch(iInput)
			{
			case 1:
				ReSetPCB();		//重置PCB完成标志、等待时间,以供另一个调度算法使用，并输出所读入的进程数据
				FIFO();			//先进先出调度算法
				break;
				
			case 2:
				ReSetPCB();		//重置PCB完成标志、等待时间,以供另一个调度算法使用，并输出所读入的进程数据
				Priority();		//优先级调度算法
				break;
				
			case 3:
				ReSetPCB();		//重置PCB完成标志、等待时间,以供另一个调度算法使用，并输出所读入的进程数据
				RR();			//时间片轮转调度算法
				break;
				
			case 4:
				ReSetPCB();		//重置PCB完成标志、等待时间,以供另一个调度算法使用，并输出所读入的进程数据
				SPF();			//短进程优先调度算法
				break;

			default:
				printf("\n	输入的算法编号错误!!\n");	
				return;
				
			}
			
			bGoOn= false;
			strcpy(sGoOn," ");
			
			cout<<endl<<"	要继续进行进程调度算法模拟吗?(Y/N)"<<endl<<"	";
			cin>>sGoOn;		
			bGoOn=(sGoOn[0]=='y'||sGoOn[0]=='Y');
						
		}

	} 

}
