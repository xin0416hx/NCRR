#include <cstring>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <list> 
#include <queue>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <iostream.h>
#include <fstream>
#include <algorithm>
#include <VECTOR>
using namespace std;


void ReadCSVFileXG(const char* filename,double**& data,int& sample_number,int& feature_number,int*& class_label,double*& feature_label,vector<int> &num_class)
//小系统修改  读文件到数组中，文件为特征×样本，读出为特征×样本
{
	int r_row=0;
	int r_col=0;
	char *a;
	const int BUFFER_SIZE=8192;
	char buffer[BUFFER_SIZE];
	ifstream inn(filename);
	if(!inn) 
	{
		cout<<"File not found!"<<endl;
		exit(0);
	}
	inn.getline(buffer, BUFFER_SIZE );
    a = strtok( buffer,",");//分割字符串
	while (a!=NULL)
	{
		r_col++;
		a = strtok(NULL,",");		
	}
	sample_number=r_col-1;//第一列为ID号
	while (inn.getline(buffer,BUFFER_SIZE))
	{
		r_row++;
	}
	feature_number=r_row;
	inn.close();
	
	class_label=new int[sample_number];
	feature_label=new double[feature_number];
	data=new double*[feature_number];
	for (int i=0;i<feature_number;i++)
		data[i]=new double[sample_number];
	
	ifstream fin(filename);
	fin.getline(buffer,BUFFER_SIZE);
	a = strtok( buffer,",");
	int i_class=0;
	while (a = strtok( NULL,","))
		class_label[i_class++]=atoi(a);//将字符串转换为浮点数
	int i_feature=0;
	while (fin.getline(buffer,BUFFER_SIZE))
	{
		//	cout<<i_feature<<" : "<<buffer<<endl;
		a = strtok( buffer,",");
		feature_label[i_feature]=atof(a);
		int i=0;
		while (a=strtok(NULL,","))
			data[i_feature][i++]=atof(a);
		i_feature++;
	}
	fin.close();
	i_class=class_label[0];
	int sum=1;
	for(int j=1;j<sample_number;j++)
	{
        if (i_class!=class_label[j])
        {
			num_class.push_back(sum);
			sum=1;
			i_class=class_label[j];
        }
		else
			sum++;
	}
	num_class.push_back(sum);
	
}

double Calculate_TSP(double *dataX,double * dataY,vector<int> num_class, int sample_number)
{
	double  g_number1=0, g_number2=0,p_c1=0,p_c2=0;
	int t;

    for (t=0;t<num_class[0];t++)
    {
		if (dataX[t]<dataY[t])
		{
			g_number1++;
		}
    }
	for (t=num_class[0];t<sample_number;t++)
	{
		if (dataX[t]<dataY[t])
		{
			g_number2++;
		}
	}
	p_c1=g_number1/num_class[0];
	p_c2=g_number2/num_class[1];

    return p_c1-p_c2;
	
}
double Calculate_Coeffient1(double *dataX,double *dataY,int sample_number)
{
	double ave1=0,ave2=0,sx=0,sy=0,sxy=0;
	int t;
	for(t=0;t<sample_number;t++)
	{
		ave1+=dataX[t];
		ave2+=dataY[t];
	}
	ave1/=sample_number;
	ave2/=sample_number;
    for(t=0;t<sample_number;t++)
	{
		sxy+=(dataX[t]-ave1)*(dataY[t]-ave2);
		sx+=(dataX[t]-ave1)*(dataX[t]-ave1);
		sy+=(dataY[t]-ave2)*(dataY[t]-ave2);
	}
	sx=sqrt(sx);
	sy=sqrt(sy);
	double pe=sxy/(sx*sy);
	return pe;
	
}

double Calculate_Coeffient2(double *dataX,double *dataY,int sample_number, int class1)
{
	double ave1=0,ave2=0,sx=0,sy=0,sxy=0;
	int t;
	for(t=class1;t<sample_number;t++)
	{
		ave1+=dataX[t];
		ave2+=dataY[t];
	}
	ave1/=(sample_number-class1);
	ave2/=(sample_number-class1);
    for(t=class1;t<sample_number;t++)
	{
		sxy+=(dataX[t]-ave1)*(dataY[t]-ave2);
		sx+=(dataX[t]-ave1)*(dataX[t]-ave1);
		sy+=(dataY[t]-ave2)*(dataY[t]-ave2);
	}
	sx=sqrt(sx);
	sy=sqrt(sy);
	double pe=sxy/(sx*sy);
	return pe;
	
}

void Pearson(double **data,int sample_number,int feature_number,int *class_label,double *feature_label,vector<int> num_class)
{

	int i,j,num=0;
	double coe,coe1,coe2,dij;
	vector<double> ratio1;
    vector<double> ratio2;
	double threshold=5;
	ofstream pea_file("Net.csv");
	pea_file<<"NO.1"<<","<<"NO.2"<<","<<"diff-correlation"<<endl;
	for (i=0;i<feature_number-1;i++)
	{
		for (j=i+1;j<feature_number;j++)
		{
		
			
			coe=Calculate_TSP(data[i],data[j],num_class,sample_number);
			coe1=Calculate_Coeffient1(data[i],data[j],num_class[0]);
            coe2=Calculate_Coeffient2(data[i],data[j],sample_number,num_class[0]);
			dij=fabs(coe/(coe1+coe2+0.00005));
			if(dij>=threshold)
            {
				pea_file<<i+1<<","<<j+1<<","<<dij<<","<<endl;
			}
		}
	}
	pea_file.close();
}

void main()
{
	double** data=NULL;
    int sample_number=0;
	int feature_number=0;
	int* class_label=NULL;
   	double * feature_label=NULL;
	vector<int> num_class;
    ReadCSVFileXG("LUAD input example.csv",data,sample_number,feature_number,class_label,feature_label,num_class);
	Pearson(data,sample_number,feature_number,class_label,feature_label,num_class);
	
}