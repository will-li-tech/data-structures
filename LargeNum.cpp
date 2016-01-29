

/*
Data structure LargeNum for calculating large fibonacci number

The actual 41434767994th number in the sequence has about 8E9 digits. (Removing all vector resize() in the code will give the complete result after very long time if you are patient and have a very fast PC, still running in my PC anyway...)

Due to time and computation limit, only lowest 108 digits are calculated and given here:
726357264043698812117477996558506141029764334078345942972614042351597594295741563375552187159386862981593367

Therefore its lowest order 5 digits are:
93367

The program execution time taken:
0.03 second(s)
*/

#include <iostream>
#include <ctime>
#include <iomanip>
#include <vector>

using namespace std;

typedef unsigned long long ull;

struct LargeNum
{
	static const ull base=1000000000ULL; //base=1E9 in order to avoid operation overflow due to integer limit (~1.8E19) in C++ 
	static const int width=9;	
	
	vector<ull> num;
/*=====================================================================================*/	
	LargeNum operator+(const LargeNum &b) const
	{
		LargeNum ret;
		ull sz1=num.size(), sz2=b.num.size(), i1=0, i2=0, carry=0;
		if(sz1==0 || sz2==0)	return (sz1>0)?(*this):b;	
		while(i1<sz1 || i2<sz2 || carry)
		{
			ull sum=carry;
			if(i1<sz1)	sum+=num[i1++];
			if(i2<sz2)	sum+=b.num[i2++];
			ret.num.push_back(sum%base);
			carry=sum/base;			
		}
		return ret;
	}
/*=====================================================================================*/
	LargeNum operator*(const LargeNum &b) const
	{			
		ull sz1=num.size(), sz2=b.num.size();
		if(sz1==0 || sz2==0)	return 0;
		LargeNum ret;	
		ret.num=vector<ull> (sz1+sz2+3,0);
		//cout<<sz1<<" "<<sz2<<endl;
		for(ull i1=0;i1<sz1;++i1)
			for(ull i2=0;i2<sz2;++i2)
			{
				ret.num[i1+i2]+=num[i1]*b.num[i2];
				for(ull j=i1+i2;j<ret.num.size()-1 && ret.num[j]>=base;++j)					
				{
					ret.num[j+1]+=ret.num[j]/base;
					ret.num[j]%=base;
				}				
			}
		for(ull i=ret.num.size()-1;i>=0;--i)
		{
			if(ret.num[i]==0)	ret.num.pop_back();
			else break;
			if(i==0)	break;
		}
		return ret;
	}
/*=====================================================================================*/
	LargeNum &operator=(const LargeNum &b) 
	{
		if(this!=&b)
		{
			num.clear();
			for(ull i=0;i<b.num.size();++i)
				num.push_back(b.num[i]);
		}
		return *this;
	}	
/*=====================================================================================*/	
	LargeNum (ull m=0)
	{
		num.clear();	
		while(m)
		{
			num.push_back(m%base);
			m/=base;
		}
	}
/*=====================================================================================*/	
	LargeNum (const LargeNum &b)
	{
		num.clear();
		num=vector<ull> (b.num.begin(), b.num.end());
	}
/*=====================================================================================*/	
	void print()
	{
		ull sz=num.size();
		if(sz==0)	
			cout<<0<<endl;
		else
		{
			for(ull i=sz-1;i>=0;--i)
			{
				if(i==sz-1)	cout<<num[i];
				else		cout<<setfill('0')<< setw(width)<<num[i];
				if(i==0)	break;
			}
			cout<<endl;
		}
	}
};


class Solution
{
public:
	void output()
	{
		clock_t begin=clock();		
		ull n=10000;	
		LargeNum res=nthFib(n);		
		cout<<"The lowest 108 digits of the "<<n<<"th number in the sequence is: \n";
		res.print();
		cout<<"\nIts lowest order 5 digits are: \n"<<(res.num[0]%100000)<<endl<<endl;	
		
		cout<<"The program execution time taken:"<<endl;
		cout<<double(clock()-begin)/CLOCKS_PER_SEC<<" second(s)\n\n";			
	}
/*=====================================================================================*/	
private:
	void mulMatrix(LargeNum res[2][2], LargeNum a[2][2], LargeNum b[2][2])
	{    
		LargeNum tmp[4];
		
		tmp[0]=a[0][0]*b[0][0] + a[0][1]*b[1][0];
		tmp[1]=a[0][0]*b[0][1] + a[0][1]*b[1][1];
		tmp[2]=a[1][0]*b[0][0] + a[1][1]*b[1][0];
		tmp[3]=a[1][0]*b[0][1] + a[1][1]*b[1][1];    
		
		res[0][0]=tmp[0]; 
		res[0][1]=tmp[1]; 
		res[1][0]=tmp[2]; 
		res[1][1]=tmp[3]; 
		/*
		for(int i=0;i<2;++i) //resize to only get lowest 15*9 digits of calculated result for simplicity and speeding up program
			for(int j=0;j<2;++j)
				if(res[i][j].num.size()>15)	
					res[i][j].num.resize(15);
		*/
	}
/*=====================================================================================*/	
	void powMatrix(LargeNum res[2][2], LargeNum a[2][2], ull n)
	{
		while(n>0)
		{
			if(n&1) 
				mulMatrix(res, res, a);
			mulMatrix(a, a, a);
			//cout<<n<<endl;
			n >>=1;			
		}
	}
/*=====================================================================================*/
	LargeNum nthFib(ull n)
	{
		if(n<1) 
		{
			cout<<"Input error\n";
			return 0;
		} else if(n<=2) 
			return 1;
		LargeNum a[2][2] = { {LargeNum(1), LargeNum(1)}, {LargeNum(1), LargeNum(0)} };
		LargeNum s[2][2] = { {LargeNum(1), LargeNum(0)}, {LargeNum(0), LargeNum(1)} };
		powMatrix(s, a, n-2);
		LargeNum ret=s[0][0] + s[0][1];
		//if(ret.num.size()>12)  //resize to only get lowest 12*9=108 digits of calculated result for simplicity and speeding up program
			//ret.num.resize(12);
		return ret;
	}
};


	
int main()
{
	Solution solution;
	solution.output();	
	return 0;
}





