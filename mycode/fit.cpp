#include "fit.h"

int fit(float *b1,float *b2,float *b3,float *a1,float *a2,float *a3,float *c1,float *c2,float *c3,int num1,int num2) {

	int i,pd;
	double l,m,n,o,p,q,s,ax,ay,az,bx,by,bz,eax,eay,eaz,ebx,eby,ebz,eq,maxs,maxs2,max1,max2,max3;
	double miua[3],py[3];
	double miu[3][3];
	double v[4]={1,1,1,1};
	double u[4];
	double qg[4];
	float result1,result2,result3;

	pd=0;
	for(i=0;i<num1;i++) {
		if(a1[i]!=b1[i]) { pd++; break; }
		if(a2[i]!=b2[i]) { pd++; break; }
		if(a3[i]!=b3[i]) { pd++; break; }
	}
	if(pd==0) return(1);

	double R[3][3]={0};
	ax=0; ay=0; az=0; bx=0; by=0; bz=0;
	for(i=0;i<num1;i++) {
		ax+=a1[i]; ay+=a2[i]; az+=a3[i];
		bx+=b1[i]; by+=b2[i]; bz+=b3[i];
	}
	eax=ax/num1; eay=ay/num1; eaz=az/num1;
	ebx=bx/num1; eby=by/num1; ebz=bz/num1;

	for(i=0;i<num1;i++) {
		s=(a1[i]-eax)*(b1[i]-ebx); R[0][0]=R[0][0]+s;
		s=(a1[i]-eax)*(b2[i]-eby); R[0][1]=R[0][1]+s;
		s=(a1[i]-eax)*(b3[i]-ebz); R[0][2]=R[0][2]+s;
		s=(a2[i]-eay)*(b1[i]-ebx); R[1][0]=R[1][0]+s;
		s=(a2[i]-eay)*(b2[i]-eby); R[1][1]=R[1][1]+s;
		s=(a2[i]-eay)*(b3[i]-ebz); R[1][2]=R[1][2]+s;
		s=(a3[i]-eaz)*(b1[i]-ebx); R[2][0]=R[2][0]+s;
		s=(a3[i]-eaz)*(b2[i]-eby); R[2][1]=R[2][1]+s;
		s=(a3[i]-eaz)*(b3[i]-ebz); R[2][2]=R[2][2]+s;
	}
	double A1[4][4]={R[0][0]+R[1][1]+R[2][2],R[1][2]-R[2][1],R[2][0]-R[0][2],R[0][1]-R[1][0],R[1][2]-R[2][1],R[0][0]-R[1][1]-R[2][2],R[0][1]+R[1][0],R[0][2]+R[2][0],R[2][0]-R[0][2],R[0][1]+R[1][0],-R[0][0]+R[1][1]-R[2][2],R[1][2]+R[2][1],R[0][1]-R[1][0],R[0][2]+R[2][0],R[1][2]+R[2][1],-R[0][0]-R[1][1]+R[2][2]};

	v[0]=1; v[1]=1; v[2]=1; v[3]=1;
	for(i=0;i<100000;i++) {
		u[0]=A1[0][0]*v[0]+A1[0][1]*v[1]+A1[0][2]*v[2]+A1[0][3]*v[3];
		u[1]=A1[1][0]*v[0]+A1[1][1]*v[1]+A1[1][2]*v[2]+A1[1][3]*v[3];
		u[2]=A1[2][0]*v[0]+A1[2][1]*v[1]+A1[2][2]*v[2]+A1[2][3]*v[3];
		u[3]=A1[3][0]*v[0]+A1[3][1]*v[1]+A1[3][2]*v[2]+A1[3][3]*v[3];

		l=fabs(u[0]); m=fabs(u[1]); n=fabs(u[2]); o=fabs(u[3]);
		if(l>=m) {
			max1=l; maxs=u[0];
		}
		else {
			max1=m; maxs=u[1];
		}
		if(max1<n) {
			max1=n; maxs=u[2];
		}
		if(max1<o) {
			max1=o; maxs=u[3];
		}
		v[0]=u[0]/max1; v[1]=u[1]/max1; v[2]=u[2]/max1; v[3]=u[3]/max1;

		if(i>1)
			max3=fabs((max2-max1)/max1);
		else
			max3=1;
		if(max3<0.0000000000001&&i>10)
		break;
		max2=max1;
		maxs2=maxs;
	}

	qg[0]=v[0]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]);
	qg[1]=v[1]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]);
	qg[2]=v[2]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]);
	qg[3]=v[3]/sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]+v[3]*v[3]);

	if(maxs*maxs2<0) {
		miu[0][0]=-(qg[0]*qg[0]+qg[1]*qg[1]-qg[2]*qg[2]-qg[3]*qg[3]);
		miu[0][1]=-(qg[1]*qg[2]-qg[0]*qg[3]+qg[1]*qg[2]-qg[0]*qg[3]);
		miu[0][2]=-(qg[1]*qg[3]+qg[0]*qg[2]+qg[1]*qg[3]+qg[0]*qg[2]);
		miu[1][0]=-(qg[1]*qg[2]+qg[0]*qg[3]+qg[1]*qg[2]+qg[0]*qg[3]);
		miu[1][1]=-(qg[0]*qg[0]-qg[1]*qg[1]+qg[2]*qg[2]-qg[3]*qg[3]);
		miu[1][2]=-(qg[2]*qg[3]-qg[0]*qg[1]+qg[2]*qg[3]-qg[0]*qg[1]);
		miu[2][0]=-(qg[1]*qg[3]-qg[0]*qg[2]+qg[1]*qg[3]-qg[0]*qg[2]);
		miu[2][1]=-(qg[2]*qg[3]+qg[0]*qg[1]+qg[2]*qg[3]+qg[0]*qg[1]);
		miu[2][2]=-(qg[0]*qg[0]-qg[1]*qg[1]-qg[2]*qg[2]+qg[3]*qg[3]);
	}
	else {
		miu[0][0]=qg[0]*qg[0]+qg[1]*qg[1]-qg[2]*qg[2]-qg[3]*qg[3];
		miu[0][1]=qg[1]*qg[2]-qg[0]*qg[3]+qg[1]*qg[2]-qg[0]*qg[3];
		miu[0][2]=qg[1]*qg[3]+qg[0]*qg[2]+qg[1]*qg[3]+qg[0]*qg[2];
		miu[1][0]=qg[1]*qg[2]+qg[0]*qg[3]+qg[1]*qg[2]+qg[0]*qg[3];
		miu[1][1]=qg[0]*qg[0]-qg[1]*qg[1]+qg[2]*qg[2]-qg[3]*qg[3];
		miu[1][2]=qg[2]*qg[3]-qg[0]*qg[1]+qg[2]*qg[3]-qg[0]*qg[1];
		miu[2][0]=qg[1]*qg[3]-qg[0]*qg[2]+qg[1]*qg[3]-qg[0]*qg[2];
		miu[2][1]=qg[2]*qg[3]+qg[0]*qg[1]+qg[2]*qg[3]+qg[0]*qg[1];
		miu[2][2]=qg[0]*qg[0]-qg[1]*qg[1]-qg[2]*qg[2]+qg[3]*qg[3];
	}

	py[0]=ebx-(miu[0][0]*eax+miu[0][1]*eay+miu[0][2]*eaz);
	py[1]=eby-(miu[1][0]*eax+miu[1][1]*eay+miu[1][2]*eaz);
	py[2]=ebz-(miu[2][0]*eax+miu[2][1]*eay+miu[2][2]*eaz);


	for(i=0;i<num2;i++) {
		result1=miu[0][0]*c1[i]+miu[0][1]*c2[i]+miu[0][2]*c3[i]+py[0];
		result2=miu[1][0]*c1[i]+miu[1][1]*c2[i]+miu[1][2]*c3[i]+py[1];
		result3=miu[2][0]*c1[i]+miu[2][1]*c2[i]+miu[2][2]*c3[i]+py[2];
		c1[i]=result1;
		c2[i]=result2;
		c3[i]=result3;
	}

	return(1);
}
