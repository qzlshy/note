#include "readfile.h"

int Read_dcd::loadfile(const char *s) {

	int frame;
	frame=0;
	fp=fopen(s,"rb");
	frame=readheader();
	xt=new float[N];
	yt=new float[N];
	zt=new float[N];
	return(frame);
}

int Read_dcd::getN() {

	return(N);
}

int Read_dcd::loadfile(string& s) {

	int frame;
	frame=0;
	fp=fopen(s.c_str(),"rb");
	frame=readheader();
	xt=new float[N];
	yt=new float[N];
	zt=new float[N];
	return(frame);
}

int Read_dcd::closefile() {

	delete []xt;
	delete []yt;
	delete []zt;
	fclose(fp);
}

int Read_dcd::readheader() {

	int i,num,t;
	fread(&num,4,1,fp);
	fread(CORD,1,4,fp);
	fread(&NFILE,4,1,fp);
	fread(&NPRIV,4,1,fp);
	fread(&NSAVC,4,1,fp);
	fread(&NSTEP,4,1,fp);
	for(i=0;i<5;i++)
		fread(&t,4,1,fp);
	fread(&DELTA,4,1,fp);
	fread(&with_unitcell,4,1,fp);
	for(i=0;i<9;i++)
		fread(&t,4,1,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(&title_string,1,164,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(&N,4,1,fp);
	fread(&num,4,1,fp);
	return(NFILE);
}

int Read_dcd::readstep() {

	int num,t;
	double t_d;

	if(with_unitcell) {

		fread(&num,4,1,fp);
		fread(&cellt[0],8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&cellt[1],8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&cellt[2],8,1,fp);
		fread(&num,4,1,fp);
	}

	fread(&num,4,1,fp);
	fread(xt,1,num,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(yt,1,num,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(zt,1,num,fp);
	fread(&num,4,1,fp);
}

int Read_dcd::getstep(float *x,float *y,float *z) {

	int i;
	for(i=0;i<N;i++) {

		x[i]=xt[i];
		y[i]=yt[i];
		z[i]=zt[i];
	}
}

int Read_dcd::getstep(float *x,float *y,float *z,int *i_c,int n_c) {

	int i;
	for(i=0;i<n_c;i++) {

		x[i]=xt[i_c[i]];
		y[i]=yt[i_c[i]];
		z[i]=zt[i_c[i]];
	}
}

int Read_dcd::getstep(float *x,float *y,float *z,double *cell) {

	int i;
	for(i=0;i<N;i++) {

		x[i]=xt[i];
		y[i]=yt[i];
		z[i]=zt[i];
	}

	for(i=0;i<3;i++)
		cell[i]=cellt[i];
}

int Read_dcd::getstep(float *x,float *y,float *z,double *cell,int *i_c,int n_c) {

	int i;
	for(i=0;i<n_c;i++) {

		x[i]=xt[i_c[i]];
		y[i]=yt[i_c[i]];
		z[i]=zt[i_c[i]];
	}

	for(i=0;i<3;i++)
		cell[i]=cellt[i];
}

int Read_dcd::readstep(float *x,float *y,float *z) {

	int num,t;
	double cx,cy,cz,t_d;

	if(with_unitcell) {
		
		fread(&num,4,1,fp);
		fread(&cx,8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&cy,8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&cz,8,1,fp);
		fread(&num,4,1,fp);
	}

	fread(&num,4,1,fp);
	fread(x,1,num,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(y,1,num,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(z,1,num,fp);
	fread(&num,4,1,fp);
}

int Read_dcd::readstep(float *x,float *y,float *z,double *cell) {

	int num,t;
	double t_d;

	if(with_unitcell) {

		fread(&num,4,1,fp);
		fread(&cell[0],8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&cell[1],8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&t_d,8,1,fp);
		fread(&cell[2],8,1,fp);
		fread(&num,4,1,fp);
	}

	fread(&num,4,1,fp);
	fread(x,1,num,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(y,1,num,fp);
	fread(&num,4,1,fp);
	fread(&num,4,1,fp);
	fread(z,1,num,fp);
	fread(&num,4,1,fp);
}

long Read_dcd::getoffset() {

	long start,end;
	start=ftell(fp);
	readstep(xt,yt,zt);
	end=ftell(fp);
	step_offset=end-start;
	fseek(fp,start,0);
	return(step_offset);
}

int Read_dcd::skipstep(int m) {

	fseek(fp,step_offset*long(m),1);
}

int Read_psf::loadfile(const char *file) {

	int i,j;
	string s;
	char c[100];
	ifstream fp1(file);
	while(1) {

		getline(fp1,s);
		if(fp1.eof()) break;
		if(s.find("!NATOM")!=s.npos) {

			sscanf(s.c_str(),"%d",&NATOM);
			break;
		}
	}

	atom=new PSF_atom[NATOM];
	i_c=new int[NATOM];
	i_or=new int[NATOM];
	for(i=0;i<NATOM;i++) {

		i_or[i]=0;
		i_c[i]=i;
		atom[i].index=i;
		fp1>>atom[i].serial>>atom[i].segid>>atom[i].resid>>atom[i].resname>>atom[i].name>>atom[i].type>>atom[i].charge>>atom[i].mass;
		fp1.get(c,100,'\n');
	}

	n_c=NATOM;
	n_or=0;
	fp1.clear();
	fp1.seekg(0);

	while(1) {

		getline(fp1,s);
		if(fp1.eof()) break;
		if(s.find("!NBOND")!=s.npos) {

			sscanf(s.c_str(),"%d",&NBOND);
			break;
		}
	}

	bond=new int*[NBOND];
	for(i=0;i<NBOND;i++)
		bond[i]=new int[2];
	for(i=0;i<NBOND;i++)
		fp1>>bond[i][0]>>bond[i][1];
	fp1.clear();
	fp1.seekg(0);
 
	while(1) {

		getline(fp1,s);
		if(fp1.eof()) break;
		if(s.find("!NTHETA")!=s.npos) {

			sscanf(s.c_str(),"%d",&NTHETA);
			break;
		}
	}

	angle=new int*[NTHETA];
	for(i=0;i<NTHETA;i++)
		angle[i]=new int[3];
	for(i=0;i<NTHETA;i++)
		fp1>>angle[i][0]>>angle[i][1]>>angle[i][2];
	fp1.clear();
	fp1.seekg(0);

	while(1) {

		getline(fp1,s);
		if(fp1.eof()) break;
		if(s.find("!NPHI")!=s.npos) {

			sscanf(s.c_str(),"%d",&NPHI);
			break;
		}
	}

	dihedral=new int*[NPHI];
	for(i=0;i<NPHI;i++)
		dihedral[i]=new int[4];
	for(i=0;i<NPHI;i++)
		fp1>>dihedral[i][0]>>dihedral[i][1]>>dihedral[i][2]>>dihedral[i][3];
	fp1.clear();
	fp1.seekg(0);

	while(1) {

		getline(fp1,s);
		if(fp1.eof()) break;
		if(s.find("!NIMPHI")!=s.npos) {

			sscanf(s.c_str(),"%d",&NIMPHI);
			break;
		}
	}

	improper=new int*[NIMPHI];
	for(i=0;i<NIMPHI;i++)
		improper[i]=new int[4];
	for(i=0;i<NIMPHI;i++)
		fp1>>improper[i][0]>>improper[i][1]>>improper[i][2]>>improper[i][3];
	fp1.clear();
	fp1.seekg(0);

	fp1.close();
	return(NATOM);
}

int Read_psf::loadfile(string& file) {

	loadfile(file.c_str());
	return(NATOM);
}

int Read_psf::get_NBOND() {

	return(NBOND);
}

int Read_psf::get_NTHETA() {

	return(NTHETA);
}

int Read_psf::get_NPHI() {
	return(NPHI);
}

int Read_psf::get_NIMPHI() {
	return(NIMPHI);
}

int Read_psf::get_bond(int **a) {

	int i,j;
	for(i=0;i<NBOND;i++)
	for(j=0;j<2;j++)
		a[i][j]=bond[i][j];
}

int Read_psf::get_angle(int **a) {

	int i,j;
	for(i=0;i<NTHETA;i++)
	for(j=0;j<3;j++)
		a[i][j]=angle[i][j];
}

int Read_psf::get_dihedral(int **a) {

	int i,j;
	for(i=0;i<NPHI;i++)
	for(j=0;j<4;j++)
		a[i][j]=dihedral[i][j];
}

int Read_psf::get_improper(int **a) {

	int i,j;
	for(i=0;i<NIMPHI;i++)
	for(j=0;j<4;j++)
		a[i][j]=improper[i][j];
}

int Read_psf::choose_reset() {

	int i;
	for(i=0;i<NATOM;i++)
		i_c[i]=i;
	n_c=NATOM;
}

int Read_psf::choose(int n,string *s,int m) {

	int i,j,k;

	switch(n) {

		case 1:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].segid==s[j]) {
				
				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 3:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].resname==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 4:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].name==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 5:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].type==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;
	}
}

int Read_psf::choose(int n,string s) {

	int i,j,k;

	switch(n) {

		case 1:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].segid==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 3:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].resname==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 4:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].name==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 5:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].type==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;
	}
}


int Read_psf::choose(int n,int *s,int m) {

	int i,j,k;

	switch(n) {
		case 0:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].serial==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 2:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].resid==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;
	}
}

int Read_psf::choose(int n,int s) {

	int i,j,k;

	switch(n) {

		case 0:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].serial==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 2:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].resid==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;
	}
}

int Read_psf::choose(int n,char **s,int m) {

	int i,j,k;
	string tmp;

	switch(n) {

		case 1:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].segid==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 3:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].resname==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 4:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].name==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;

		case 5:
		k=0;
		for(i=0;i<n_c;i++) {

			for(j=0;j<m;j++)
			if(atom[i_c[i]].type==s[j]) {

				i_c[k]=i_c[i];
				k++;
				break;
			}
		}
		n_c=k;
		break;
	}
}

int Read_psf::choose(int n,const char *s) {

	int i,j,k;
	string tmp;

	switch(n) {

		case 1:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].segid==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 3:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].resname==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 4:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].name==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;

		case 5:
		k=0;
		for(i=0;i<n_c;i++) {

			if(atom[i_c[i]].type==s) {

				i_c[k]=i_c[i];
				k++;
			}
		}
		n_c=k;
		break;
	}
}

int Read_psf::choose(const char *w,string *s,int m) {

	int i;
	string use[6]={"serial","segid","resid","resname","name","type"};
	for(i=0;i<6;i++)
	if(use[i]==w) {

		choose(i,s,m);
		break;
	}
}

int Read_psf::choose(const char *w,char **s,int m) {

	int i;
	string use[6]={"serial","segid","resid","resname","name","type"};
	for(i=0;i<6;i++)
	if(use[i]==w) {

		choose(i,s,m);
		break;
	}
}

int Read_psf::choose(const char *w,int *s,int m) {

	int i;
	string use[6]={"serial","segid","resid","resname","name","type"};
	for(i=0;i<6;i++)
	if(use[i]==w) {

		choose(i,s,m);
		break;
	}
}

int Read_psf::choose(const char *w,string s) {

	int i;
	string use[6]={"serial","segid","resid","resname","name","type"};
	for(i=0;i<6;i++)
	if(use[i]==w) {

		choose(i,s);
		break;
	}
}

int Read_psf::choose(const char *w,const char *s) {

	int i;
	string use[6]={"serial","segid","resid","resname","name","type"};
	for(i=0;i<6;i++)
	if(use[i]==w) {
		
		choose(i,s);
		break;
	}
}

int Read_psf::choose(const char *w,int s) {

	int i;
	string use[6]={"serial","segid","resid","resname","name","type"};
	for(i=0;i<6;i++)
	if(use[i]==w) {

		choose(i,s);
		break;
	}
}

int Read_psf::choose_get(int *num) {

	int i;
	for(i=0;i<n_c;i++)
		num[i]=i_c[i];
	return(n_c);
}

int Read_psf::choose_to_tmp() {

	int i,k;
	for(i=0;i<n_c;i++) {

		i_or[i_c[i]]=1;
	}
	k=0;
	for(i=0;i<NATOM;i++)
	if(i_or[i]==1)
		k++;
	n_or=k;
	return(n_or);
}

int Read_psf::choose_reset_tmp() {

	int i;
	n_or=0;
	for(i=0;i<NATOM;i++)
		i_or[i]=0;
}

int Read_psf::choose_get_tmpn() {

	return(n_or);
}

int Read_psf::choose_get_tmp(int *a) {

	int i,k;
	k=0;
	for(i=0;i<NATOM;i++)
	if(i_or[i]==1) {

		a[k]=i;
		k++;
	}
	return(n_or);
}

int Read_psf::choose_getn() {

	return(n_c);
}

int Read_psf::choose_noth() {

	int i,k;
	k=0;
	for(i=0;i<n_c;i++)
	if(atom[i_c[i]].name[0]!='H') {

		i_c[k]=i_c[i];
		k++;
	}
	n_c=k;
	return(n_c);
}

int Read_pdb::loadfile(const char *s) {

	ifstream pdbfile(s);
	string l,l1,tmps,tmpres;
	stringstream stream;
	int i,k,n;
	n=0;
	while(1) {

		getline(pdbfile,l);
		if(pdbfile.eof()) break;
		if(l.empty()) continue;
		if(l.size()<4) continue;
		if(l.compare(0,4,"ATOM")) continue;
		stream.clear();
		stream.str(l);
		stream>>l1;
		if(l1=="ATOM")
			n++;
	}

	N=n;
	pdbfile.clear();
	pdbfile.seekg(0);
	atom=new PDB_atom[N];
	line=new string[N];

	n=0;
	while(1) {

		getline(pdbfile,l);
		if(pdbfile.eof()) break;
		if(l.empty()) continue;
		if(l.size()<4) continue;
		if(l.compare(0,4,"ATOM")) continue;
		line[n]=l;
		stream.clear();
		stream.str(l);
		stream>>l1;
		if(l1=="ATOM") {

			tmps=l.substr(6,5);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].serial;
			tmps=l.substr(12,4);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].name;
			tmps=l.substr(16,1);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].alt;
			tmps=l.substr(17,3);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].resname;
			tmps=l.substr(21,1);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].chain;
			tmps=l.substr(22,4);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].resid;
			tmps=l.substr(30,8);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].x;
			tmps=l.substr(38,8);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].y;
			tmps=l.substr(46,8);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].z;
			tmps=l.substr(60,6);
			stream.clear();
			stream.str(tmps);
			stream>>atom[n].tof;
			if(l.size()>=76) {

				tmps=l.substr(72,4);
				stream.clear();
				stream.str(tmps);
				stream>>atom[n].segid;
			}
			atom[n].index=n;
			n++; 
		}
	}

	pdbfile.clear();
	pdbfile.seekg(0);
	pdbfile.close();
}

int Read_pdb::loadfile(string& s) {

	loadfile(s.c_str());
}

string Read_pdb::pdbline(float xn,float yn,float zn,int n) {

	char rx[10],ry[10],rz[10];
	string s;
	sprintf(rx,"%8.3f",xn);
	sprintf(ry,"%8.3f",yn);
	sprintf(rz,"%8.3f",zn);
	s=line[n];
	s.replace(30,8,rx);
	s.replace(38,8,ry);
	s.replace(46,8,rz);
	return(s);
}

string Read_pdb::pdbline(int n) {

	return(line[n]);
}

int Read_pdb::pdbclose() {

	delete []atom;
	delete []line;
}
