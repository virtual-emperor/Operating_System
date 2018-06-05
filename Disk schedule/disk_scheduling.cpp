#include<stdio.h> 
#include<iostream>
#include<string.h>
#include<math.h> 

const int MAXQUEUE=200; //�������������󳤶�
using namespace std;

//�ŵ�������ṹ�嶨�� 
typedef struct TRACK_Node
{ 
	int iGo;		//Ҫ���ʵĴŵ���
	int iBeVisited;	//�ŵ��Ƿ��Ѿ����ʱ�־��1���ѷ��ʣ�0��δ���ʣ�
}TRACK; 


TRACK queue[MAXQUEUE]; //�ŵ��������������


int iReqNum=0;			//�ŵ�����������
int iStart=0;			//��ͷ��ʼλ��
int iNow=0;				//��ͷ��ǰλ��
int iSum=0;				//���ƶ��ŵ���
int iInput;				//�û���ǰ���������
char sFileName[20];		//�ļ���

void Init();			//��ʼ������
void Reset();			//���÷��ʱ�־����ͷ��ǰλ�á����ƶ��ŵ���
int ReadTrackFile();	//����ŵ������ļ�
void FCFS();			//�����ȷ�������㷨
void SSTF();			//���Ѱ�����ȵ����㷨
void SCAN();			//���ݵ����㷨
void Version();			//��ʾ��Ȩ��Ϣ���� 
void Hint();			//��ʾʵ����ʾ��Ϣ����

 
void Init()	//��ʼ������
{
	int i; 
	
	for(i=0;i<MAXQUEUE;i++)
	{ 
		queue[i].iGo=-1;		//����Ҫ���ʵĴŵ���Ϊ�����ܵ���-1����������������ŵ���
		queue[i].iBeVisited=0;	//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ0��δ����
	} 
	
} //void Init()


void Reset()	//���÷��ʱ�־����ͷ��ǰλ�á����ƶ��ŵ���
{
	int i; 
	
	for(i=0;i<iReqNum;i++)
	{ 
		queue[i].iBeVisited=0; 	//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ0��δ����
	} 

	printf( "\n	�������ͷ�ĳ�ʼ�ŵ��ţ�������:  "); 
	scanf("%d",&iStart);	//�ӱ�׼�����ȡ�û���ǰ����Ĵ�ͷ��ʼλ��
	iNow=iStart;			//��ͷ��ǰλ��
	iSum=0;					//���ƶ��ŵ��� 
	
} //void Reset()

 
int ReadTrackFile() //����ŵ������ļ�
{ 
	FILE *fp; 	
	int iTemp; 
	char cHead;
	
	cout<<"	\n	������Ҫ�򿪵Ĵŵ��������ı����ļ�����ע�⣺����·�����ͺ�׺����:  ";
	cin>>sFileName; 		//�ӱ�׼�����ȡ�û���ǰ������ļ���
	
	if((fp=fopen(sFileName,"r"))==NULL)
	{ 
		cout<<endl<<"	���󣺴ŵ������ļ� "<<sFileName<<" �򲻿��������ļ�����·��!!"<<endl;
		return -1;
		
	} 
	else
	{
		cHead=fgetc(fp);
		while(cHead==' '||cHead==10||cHead==9)	//���˿ո񡢻��к�TAB�ַ�
		{
			cHead=fgetc(fp);
		}
		if(cHead==EOF)
		{
			printf("	���󣺴ŵ������ļ���%s Ϊ��!!\n",sFileName );
			return -1;
		}
		else
		{
			fseek( fp, -1, SEEK_CUR); 
			while(!feof(fp))	//���ļ�������Ĵŵ������δ���ŵ��������������
			{ 
				fscanf(fp,"%d ",&iTemp); 
				queue[iReqNum].iGo=iTemp; 
				iReqNum++;	//�ŵ����������� 
			} 
		}
	}	//if((fp=fopen(sFileName,"r"))==NULL)

	return 0;

} //void ReadTrackFile()

 
void FCFS() //�����ȷ�������㷨
{
	int i; 
	
	Reset();	//���÷��ʱ�־����ͷ��ǰλ�á����ƶ��ŵ���
	
	cout<<endl<<"---------------------------------------------"<<endl; 

	cout<<"	�����ȷ�������㷨�ĵ��Ƚ��:  "<<endl<<endl; 
	cout<<"	    ��ʼ�ŵ���:  "<<iStart<<endl; 
	cout<<"	���  ��һ�ŵ���  �ƶ��Ĵŵ���"<<endl; 
	
	for(i=0;i<iReqNum;i++)
	{ 
		//�����Ϣ�����������Ĵŵ�
		cout<<"      	"<<i+1<<"      "<<queue[i].iGo<<"      "<<abs(queue[i].iGo-iNow)<<endl;
		iSum+=abs(queue[i].iGo-iNow);		//�ۼ����ƶ��ŵ���
		iNow=queue[i].iGo;					//�ƶ���ͷ��ǰλ��Ϊ��ǰ���ʴŵ���
	} 
	cout<<endl<<"	�ܵ��ȴ���:  "<<iReqNum<<endl;
	cout<<endl<<"	���ƶ��ŵ���:  "<<iSum<<endl;
	printf("\n	ƽ���ƶ��ŵ���:  %.2f\n\n",(float)iSum / (float)iReqNum);

} //void FCFS()

 

 
void SCAN()	//���ݵ����㷨 
{ 
	int i,j; 
	int iNext;		//�������ʵ���һδ���ʴŵ������������queue�е��±� 
	int iMinMove;	//��ǰ���������Ѱ������

	cout<<endl<<"---------------------------------------------"<<endl; 
	
	cout<<endl<<"	��ѡ���ͷ��ʼ����1 OR 2����"<<endl<<endl;
	cout<<"	1  ��ͷ��ʼ����"<<endl<<endl<<"	2  ��ͷ��ʼ����"<<endl<<endl<<"	";

	cin>>iInput;	//�ӱ�׼�����ȡ�û���ǰ���������
	
	switch(iInput)	//�û���ǰ���������
	{
	case 1:			//��ͷ��ʼ����
		Reset();	//���÷��ʱ�־����ͷ��ǰλ�á����ƶ��ŵ���
		
		cout<<endl<<"---------------------------------------------"<<endl; 

		cout<<"	���ݵ����㷨������ͷ��ʼ���ڵĵ��Ƚ��:  "<<endl<<endl; 
		cout<<"	    ��ʼ�ŵ���:  "<<iStart<<endl; 
		cout<<"	���  ��һ�ŵ���  �ƶ��Ĵŵ���"<<endl; 

		for(i=0;i<iReqNum;i++)
		{
			iMinMove=9999; 
			iNext=-1; 
			for(j=0;j<iReqNum;j++)	//Ѱ�ҵ�ǰ������Ѱ��������̵�δ���ʴŵ������������queue�е��±�
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
				//�����Ϣ�����������Ĵŵ�
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//�ۼ����ƶ��ŵ���
				iNow=queue[iNext].iGo;					//�ƶ���ͷ��ǰλ��Ϊ��ǰ���ʴŵ���
				queue[iNext].iBeVisited=1;				//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ1���ѷ���
			} //if(iNext!=-1)

			else	//��ͷ����
			{ 
				for(j=0;j<iReqNum;j++)		//Ѱ�ҵ�ǰ������Ѱ��������̵�δ���ʴŵ������������queue�е��±�
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

				//�����Ϣ�����������Ĵŵ�
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//�ۼ����ƶ��ŵ���
				iNow=queue[iNext].iGo;				 	//�ƶ���ͷ��ǰλ��Ϊ��ǰ���ʴŵ���
				queue[iNext].iBeVisited=1;				//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ1���ѷ���

			} //if(iNext!=-1)
		} //for(i=0;i<iReqNum;i++)

		cout<<endl<<"	�ܵ��ȴ���:  "<<iReqNum<<endl;
		cout<<endl<<"	���ƶ��ŵ���:  "<<iSum<<endl;
		printf("\n	ƽ���ƶ��ŵ���:  %.2f\n\n",(float)iSum / (float)iReqNum);

		break;

	case 2:	  //��ͷ��ʼ����
	  
		Reset();	//���÷��ʱ�־����ͷ��ǰλ�á����ƶ��ŵ���
		
		cout<<endl<<"---------------------------------------------"<<endl; 

		cout<<"	���ݵ����㷨������ͷ��ʼ����ĵ��Ƚ��:  "<<endl<<endl; 
		cout<<"	    ��ʼ�ŵ���:  "<<iStart<<endl; 
		cout<<"	���  ��һ�ŵ���  �ƶ��Ĵŵ���"<<endl; 

		for(i=0;i<iReqNum;i++)
		{
			iMinMove=9999; 
			iNext=-1; 
			for(j=0;j<iReqNum;j++)	//Ѱ�ҵ�������Ѱ��������̵�δ���ʴŵ������������queue�е��±�
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
				//�����Ϣ�����������Ĵŵ�
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//�ۼ����ƶ��ŵ���
				iNow=queue[iNext].iGo;					//�ƶ���ͷ��ǰλ��Ϊ��ǰ���ʴŵ���
				queue[iNext].iBeVisited=1;				//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ1���ѷ���
			} //if(iNext!=-1)

			else	//��ͷ����
			{ 
				for(j=0;j<iReqNum;j++)		//Ѱ�ҵ�ǰ������Ѱ��������̵�δ���ʴŵ������������queue�е��±�
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

				//�����Ϣ�����������Ĵŵ�
				cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
				iSum+=abs(queue[iNext].iGo-iNow);	 	//�ۼ����ƶ��ŵ���
				iNow=queue[iNext].iGo;				 	//�ƶ���ͷ��ǰλ��Ϊ��ǰ���ʴŵ���
				queue[iNext].iBeVisited=1;				//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ1���ѷ���

			} //if(iNext!=-1)
		} //for(i=0;i<iReqNum;i++)

		cout<<endl<<"	�ܵ��ȴ���:  "<<iReqNum<<endl;
		cout<<endl<<"	���ƶ��ŵ���:  "<<iSum<<endl;
		printf("\n	ƽ���ƶ��ŵ���:  %.2f\n\n",(float)iSum / (float)iReqNum);
		
		break;
					
	default:
		printf("\n	�������!!\n\n");
		return;
	}	//switch(iInput)

} //void SCAN()

void SSTF() //���Ѱ�����ȵ����㷨
{ 
	int i,j; 
	int iNext;		//�������ʵ���һδ���ʴŵ������������queue�е��±� 
	int iMinMove;	//��ǰ���������Ѱ������
	Reset();	//���÷��ʱ�־����ͷ��ǰλ�á����ƶ��ŵ���
	
	cout<<endl<<"---------------------------------------------"<<endl; 

	cout<<"	���Ѱ�����ȵ����㷨�ĵ��Ƚ��:  "<<endl<<endl; 
	cout<<"	    ��ʼ�ŵ���:  "<<iStart<<endl; 
	cout<<"	���  ��һ�ŵ���  �ƶ��Ĵŵ���"<<endl; 
    for(i=0;i<iReqNum;i++)
	{
	     iMinMove=9999; 
			iNext=-1; 
			for(j=0;j<iReqNum;j++)	//Ѱ�ҵ�ʣ�µ���Ѱ��������̵�δ���ʴŵ������������queue�е��±�
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
	   //�����Ϣ�����������Ĵŵ�
		cout<<"      	"<<i+1<<"      "<<queue[iNext].iGo<<"      "<<abs(queue[iNext].iGo-iNow)<<endl;
		iSum+=abs(queue[iNext].iGo-iNow);	 	//�ۼ����ƶ��ŵ���
		iNow=queue[iNext].iGo;					//�ƶ���ͷ��ǰλ��Ϊ��ǰ���ʴŵ���
		queue[iNext].iBeVisited=1;				//���ôŵ��Ƿ��Ѿ����ʱ�־Ϊ1���ѷ���
     
	}
        cout<<endl<<"	�ܵ��ȴ���:  "<<iReqNum<<endl;
		cout<<endl<<"	���ƶ��ŵ���:  "<<iSum<<endl;
		printf("\n	ƽ���ƶ��ŵ���:  %.2f\n\n",(float)iSum / (float)iReqNum);
		

}



void main() 
{ 
	int i; 
	bool bGoOn;		//�Ƿ�������̵����㷨ģ��ı�־
	char sGoOn[1];	//�û������Ƿ�������̵����㷨ģ�����ĸ��y��Y��N��n

	
	Init();			//��ʼ������
	
	if(ReadTrackFile()==-1)    //����ŵ������ļ�
		{
			printf("	����ŵ������ļ�ʧ��!!\n\n");
		}
		else
		{
			bGoOn= true;
			while (bGoOn)
			{
				cout<<endl<<"---------------------------------------------"<<endl;
				cout<<"	�Ӵŵ������ļ� "<<sFileName<<" ������Ĵŵ���������:"<<endl<<endl<<"	"; 
				for(i=0;i<iReqNum;i++)
				{ 
					cout<<queue[i].iGo<<" "; 
				} 
				cout<<endl<<endl<<"	������Ϊ:  "<<iReqNum<<endl<<endl; 

				iInput=-1;		//�û������������ѡ���㷨

                cout<<endl<<"	�������㷨��ţ�1 OR 2 OR 3��ѡ������㷨��"<<endl<<endl;
				cout<<"	1  �����ȷ�������㷨"<<endl<<endl<<"	2  ���ݵ����㷨"<<endl<<endl<<"	3  ���Ѱ�����ȵ����㷨 	"<<endl<<endl<<"	"; 

				cin>>iInput;		//�ӱ�׼�����ȡ�û���ǰ���������

				switch(iInput)		//�û������������ѡ���㷨
				{
				case 1:
					FCFS();		//�����ȷ�������㷨
					break;

				case 2:
					SCAN();		//���ݵ����㷨
					break;
					
			    case 3:
					SSTF();		//���Ѱ�����ȵ����㷨
					break;
				default:
					printf("\n	������㷨��Ŵ���!!\n\n");
					return;

				}	//switch(iInput)
				
				bGoOn= false;
				strcpy(sGoOn," ");
				
				cout<<"	Ҫ�������д��̵����㷨ģ����?(Y/N)	"<<endl<<"	";
				cin>>sGoOn;		
				bGoOn=(sGoOn[0]=='y'||sGoOn[0]=='Y');

			}  //while bGoOn

		}	//if(ReadTrackFile()==-1)	
	
} //void main() 
