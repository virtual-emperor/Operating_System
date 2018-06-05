#include<stdio.h> 
#include<string.h> 
//#include<iostream.h> 
#include <iostream>
using namespace std;
const int MAXPCB=200;	//������������ 


//������̿��ƿ�PCB�ṹ������ 
typedef struct PCB_Node{ 
	char sName[20];		//������
	int iStatus;		//����״̬(1���� 2�ȴ� 3����) ��ǰ��������ʱȫ��Ĭ��Ϊ��������
	int iRunTime; 		//��������ʱ�䣨���룩
	int iPriority; 		//����������(����ԽС���ȼ�Խ��) 
	int iBeFinished; 	//�����Ƿ�ִ����ɱ�־��1������ɣ�0��ĩ��ɣ�
	int iWaitTime; 		//���̵ȴ�ʱ�䣨���룩
}PCB,PCB1; 

PCB pcbs[MAXPCB];		//PCB������� 
PCB1 pcbs1;
int iPCBNum;			//ʵ�ʽ�����
char sFileName[20];		//�������ļ���

void InitPCB();			//PCB��ʼ������
int ReadPCBFile();		//���������ļ����ݣ�����PCB�������pcbs�������
void ReSetPCB();		//����PCB��ɱ�־���ȴ�ʱ��,�Թ���һ�������㷨ʹ�ã������������Ľ�������
void FIFO();			//�Ƚ��ȳ������㷨
void Priority();		//���ȼ������㷨
void RR();				//ʱ��Ƭ��ת�����㷨
void SPF();				//�̽������ȵ����㷨
void Hint();			//��ʾʵ����ʾ��Ϣ����
void Version();			//��ʾ��Ȩ��Ϣ����


void InitPCB()			//PCB��ʼ������
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


int ReadPCBFile()	//���������ļ����ݣ�����PCB�������pcbs�������
{ 
	FILE *fp; 
	int i; 
	char cHead;
	
	cout<<"	������Ҫ�򿪵Ľ��������ı����ļ�����ע�⣺����·�����ͺ�׺����:  "; 
	cin>>sFileName; 

	if((fp=fopen(sFileName,"r"))==NULL)
	{ 
		cout<<"	���󣺽������ļ� "<<sFileName<<" �򲻿�,�����ļ�����·��!!"<<endl; 
		return -1;
	} 
	else	//if((fp=fopen(sFileName,"r"))==NULL)
	{
		cHead=fgetc(fp);
		while(cHead==' '||cHead==10||cHead==9)	//���˿ո񡢻��к�TAB�ַ�
		{
			cHead=fgetc(fp);
		}
		if(cHead==EOF)
		{
			printf("	���󣺽������ļ���%s Ϊ��!!\n",sFileName );
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
			
			//���������Ľ�������
			cout<<endl<<"	���ļ� "<<sFileName<<" ����Ľ������ݣ�"<<endl<<endl; 
			cout<<"	������  ����״̬  ����ʱ��  ������"<<endl; 
			for(i=0;i<iPCBNum;i++)
			{ 
				cout<<"	 "<<pcbs[i].sName<<"	  "<<pcbs[i].iStatus<<"	  "<<pcbs[i].iRunTime<<"		  "<<pcbs[i].iPriority<<endl; 
			} 
			cout<<"	����������"<<iPCBNum<<endl; 

			return 0;
		}	//if(cHead==EOF)		
		 
	}	//if((fp=fopen(sFileName,"r"))==NULL)	

}// int ReadPCBFile()

 
void ReSetPCB()	//����PCB��ɱ�־���ȴ�ʱ��,�Թ���һ�������㷨ʹ�ã������������Ľ�������
{ 
	int i; 
	
	//���������Ľ������� 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<endl<<"	���ļ� "<<sFileName<<" ����Ľ������ݣ�"<<endl<<endl; 
	cout<<"	������  ����״̬  ����ʱ��  ������"<<endl; 
	for(i=0;i<iPCBNum;i++)
	{ 
		pcbs[i].iBeFinished=0; 
		pcbs[i].iWaitTime=0; 
		cout<<"	 "<<pcbs[i].sName<<"	  "<<pcbs[i].iStatus<<"	  "<<pcbs[i].iRunTime<<"		  "<<pcbs[i].iPriority<<endl; 
	}
	cout<<"	����������"<<iPCBNum<<endl; 

} //void ReSetPCB()

 
void FIFO() //�Ƚ��ȳ������㷨
{ 
	int i,j; 
	int iSum; //�ܵȴ�ʱ��

	//����Ƚ��ȳ������㷨ִ���� 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<"	�Ƚ��ȳ������㷨ִ����:"<<endl<<endl; 
	cout<<"	��� ������ ����ʱ�� �ȴ�ʱ��"<<endl; 

	iSum=0; 

	for(i=0;i<iPCBNum;i++)
	{ 
		cout<<"	 "<<i+1<<"	 "<<pcbs[i].sName<<"	 "<<pcbs[i].iRunTime<<"	 "<<pcbs[i].iWaitTime<<endl; 
		//�����Ϣ�����������
		iSum+=pcbs[i].iWaitTime;	//�ۼ��ܵȴ�ʱ�� 
		
		for(j=i+1;j<iPCBNum;j++)
		{ 
			pcbs[j].iWaitTime+=pcbs[i].iRunTime; 
		} 
	} 

	cout<<"	�ܵ��ȴ���:"<<iPCBNum<<endl;
	cout<<"	�ܵȴ�ʱ��:"<<iSum<<endl;
	printf("	ƽ���ȴ�ʱ��  %.2f\n",(float)iSum / (float)iPCBNum);
 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 

} //void FIFO()

 
void Priority()	//���ȼ������㷨
{ 
  	int i,j; 
	int iSum; //�ܵȴ�ʱ��
   
	for(i=0;i<iPCBNum-1;i++){
		for(j=0;j<iPCBNum-1-i;j++){
			if(pcbs[j].iPriority>pcbs[j+1].iPriority){
			   pcbs1=pcbs[j];
			   pcbs[j]=pcbs[j+1];
			   pcbs[j+1]=pcbs1;
			}
		}
	}

	//������ȼ������㷨ִ���� 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<"	���ȼ������㷨ִ����:"<<endl<<endl; 
	cout<<"	��� ������ ����ʱ�� ���ȼ�"<<endl; 

	iSum=0; 

	for(i=0;i<iPCBNum;i++)
	{ 
		cout<<"	 "<<i+1<<"	 "<<pcbs[i].sName<<"	 "<<pcbs[i].iRunTime<<"	 "<<pcbs[i].iPriority<<endl; 
		//�����Ϣ�����������
		iSum+=pcbs[i].iWaitTime;	//�ۼ��ܵȴ�ʱ�� 
		
		for(j=i+1;j<iPCBNum;j++)
		{ 
			pcbs[j].iWaitTime+=pcbs[i].iRunTime; 
		} 
	} 

	cout<<"	�ܵ��ȴ���:"<<iPCBNum<<endl;
	cout<<"	�ܵȴ�ʱ��:"<<iSum<<endl;
	printf("	ƽ���ȴ�ʱ��  %.2f\n",(float)iSum / (float)iPCBNum);
 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	
} //void Priority()

 
void RR()	//ʱ��Ƭ��ת�����㷨
{ 
	int i;
	int iNotEnded=1;		//ѭ�����Ʊ�־���������н��̶��Ѿ�����㹻ʱ��Ƭ������ɣ���Ҫ������ת��
	int iNum;				//��ĿǰΪֹĩִ����Ľ�����
	int iRound=0;			//��ת������
	int iSum=0;				//��ʱ��Ƭ��
	float fBlockTime=10;	//ʱ��Ƭ�ĳ��ȣ����룩
	
	cout<<endl<<"	������ʱ��Ƭ�ĳ��ȣ����룩��"<<endl<<"	";
	cin>>fBlockTime;

	cout<<endl<<"---------------------------------------------------------------"<<endl;
	cout<<"	ʱ��Ƭ��ת����ִ������ʱ��Ƭ�ĳ���Ϊ��"<<fBlockTime<<"���룩:"<<endl; 

	while(iNotEnded==1)	//�������н��̶��Ѿ�����㹻ʱ��Ƭ������ɣ���Ҫ������ת��
	{
		iNotEnded=0; 
		iNum=0; 
		
		for(i=0;i<iPCBNum;i++)	//ͳ�Ƶ�ĿǰΪֹĩִ����Ľ�����iNum
		{
			if(pcbs[i].iBeFinished==0)
			{ 
				iNum++;
				
			} //if(pcbs[i].iBeFinished==0)
		} //for(i=0;i<iPCBNum;i++)
		
		if(iNum>0)
		{
			iNotEnded=1;

			iRound++;	//�ۼ���ת������
			cout<<endl<<"	��"<<iRound<<"�֣�";

			for(i=0;i<iPCBNum;i++)
			{ 
				if(pcbs[i].iBeFinished==0)
				{
					cout<<pcbs[i].sName<<" ";	//�����Ϣ�����������					


					iSum++;	//�ۼ���ʱ��Ƭ�� 
					if(pcbs[i].iRunTime<=fBlockTime*(iRound))
					//i�����ڱ��ֻ��һ��ʱ��Ƭ���ܹ��������
					{ 
						pcbs[i].iBeFinished=1; 

					} //if(pcbs[i].iRunTime<=fBlockTime*(iRound+1))

				}//if(pcbs[i].iBeFinished==0) 

			} //for(i=0;i<iPCBNum;i++)		

		} //if(iNum>0)

	} //while(iNotEnded==1)
	
	cout<<endl<<"	��ת������:"<<iRound<<"	��ʱ��Ƭ��:"<<iSum<<endl;
	cout<<endl<<"---------------------------------------------------------------"<<endl; 

}//void RR() 

 
void SPF()	//�̽������ȵ����㷨
{ 
    int i,j; 
	int iSum; //�ܵȴ�ʱ��
   
	for(i=0;i<iPCBNum-1;i++){
		for(j=0;j<iPCBNum-1-i;j++){
			if(pcbs[j].iRunTime>pcbs[j+1].iRunTime){
			   pcbs1=pcbs[j];
			   pcbs[j]=pcbs[j+1];
			   pcbs[j+1]=pcbs1;
			}
		}
	}

	//����̽������ȵ����㷨ִ���� 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 
	cout<<"	�̽������ȼ������㷨ִ����:"<<endl<<endl; 
	cout<<"	��� ������ ����ʱ�� ���ȼ�"<<endl; 

	iSum=0; 

	for(i=0;i<iPCBNum;i++)
	{ 
		cout<<"	 "<<i+1<<"	 "<<pcbs[i].sName<<"	 "<<pcbs[i].iRunTime<<"	 "<<pcbs[i].iPriority<<endl; 
		//�����Ϣ�����������
		iSum+=pcbs[i].iWaitTime;	//�ۼ��ܵȴ�ʱ�� 
		
		for(j=i+1;j<iPCBNum;j++)
		{ 
			pcbs[j].iWaitTime+=pcbs[i].iRunTime; 
		} 
	} 

	cout<<"	�ܵ��ȴ���:"<<iPCBNum<<endl;
	cout<<"	�ܵȴ�ʱ��:"<<iSum<<endl;
	printf("	ƽ���ȴ�ʱ��  %.2f\n",(float)iSum / (float)iPCBNum);
 
	cout<<endl<<"---------------------------------------------------------------"<<endl; 

} //void SPF ()



void main()		//������
{ 
	int iInput;		//�û������������ѡ���㷨 
	bool bGoOn;		//�Ƿ�������̵����㷨ģ��ı�־
	char sGoOn[1];	//�û������Ƿ�������̵����㷨ģ�����ĸ��y��Y��N��n

	
	InitPCB();		//PCB��ʼ������
	
	bGoOn= true;
	strcpy(sGoOn," ");	
		
	if(ReadPCBFile()==0)	//��־ ���������ļ����ݺ��� ִ���Ƿ���ȷ
	{
		while (bGoOn)
		{
			cout<<endl<<"	�������㷨��ţ�1 OR 2 OR 3 OR 4��ѡ����̵����㷨��"<<endl<<endl;	
			cout<<"	1  �Ƚ��ȳ������㷨"<<endl<<endl<<"	2  ���ȼ������㷨"<<endl<<endl;
			cout<<"	3  ʱ��Ƭ��ת�����㷨"<<endl<<endl<<"	4  �̽������ȵ����㷨"<<endl<<endl<<"	";

			cin>>iInput;		//�ӱ�׼�����ȡ�û������������ѡ���㷨
			switch(iInput)
			{
			case 1:
				ReSetPCB();		//����PCB��ɱ�־���ȴ�ʱ��,�Թ���һ�������㷨ʹ�ã������������Ľ�������
				FIFO();			//�Ƚ��ȳ������㷨
				break;
				
			case 2:
				ReSetPCB();		//����PCB��ɱ�־���ȴ�ʱ��,�Թ���һ�������㷨ʹ�ã������������Ľ�������
				Priority();		//���ȼ������㷨
				break;
				
			case 3:
				ReSetPCB();		//����PCB��ɱ�־���ȴ�ʱ��,�Թ���һ�������㷨ʹ�ã������������Ľ�������
				RR();			//ʱ��Ƭ��ת�����㷨
				break;
				
			case 4:
				ReSetPCB();		//����PCB��ɱ�־���ȴ�ʱ��,�Թ���һ�������㷨ʹ�ã������������Ľ�������
				SPF();			//�̽������ȵ����㷨
				break;

			default:
				printf("\n	������㷨��Ŵ���!!\n");	
				return;
				
			}
			
			bGoOn= false;
			strcpy(sGoOn," ");
			
			cout<<endl<<"	Ҫ�������н��̵����㷨ģ����?(Y/N)"<<endl<<"	";
			cin>>sGoOn;		
			bGoOn=(sGoOn[0]=='y'||sGoOn[0]=='Y');
						
		}

	} 

}
