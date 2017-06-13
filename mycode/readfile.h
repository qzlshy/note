#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>
#include <string.h>
#include <stdio.h>

using namespace std;

class Read_dcd {

	public:
	float *xt,*yt,*zt;
	double cellt[3];
	int N,NFILE,NPRIV,NSAVC,NSTEP,with_unitcell;
	float DELTA;
	char title_string[200];
	char CORD[5];
	FILE *fp;
	int loadfile(const char *);
	int loadfile(string&);
	int readheader();
	int getN();
	int readstep();
	int getstep(float *,float *,float *);
	int getstep(float *,float *,float *,double *);
	int getstep(float *,float *,float *,int *,int);
	int getstep(float *,float *,float *,double *,int *,int);
	int readstep(float *,float *,float *);
	int readstep(float *,float *,float *,double *);
	long getoffset();
	int skipstep(int);
	int closefile();

	private:
	long step_offset;
};

struct PSF_atom {

	int index;
	int serial;
	string segid;
	int resid;
	string resname;
	string name;
	string type;
	float charge;
	float mass;
};

class Read_psf {

	public:
	int NATOM,NBOND,NTHETA,NPHI,NIMPHI;
	int **bond,**angle,**dihedral,**improper;
	int *i_c,n_c;
	int *i_or,n_or;
	PSF_atom *atom;
	int loadfile(const char *);
	int loadfile(string&);
	int get_NBOND();
	int get_NTHETA();
	int get_NPHI();
	int get_NIMPHI();
	int get_bond(int **);
	int get_angle(int **);
	int get_dihedral(int **);
	int get_improper(int **);
	int choose_reset();
	int choose(const char *,string);
	int choose(const char *,const char *);
	int choose(const char *,int);
	int choose(const char *,string *,int);
	int choose(const char *,char **,int);
	int choose(const char *,int *,int);
	int choose(int,string);
	int choose(int,const char *);
	int choose(int,int);
	int choose(int,string *,int);
	int choose(int,char **,int);
	int choose(int,int *,int);
	int choose_to_tmp();
	int choose_get_tmpn();
	int choose_get_tmp(int *);
	int choose_reset_tmp();
	int choose_noth();
	int choose_getn();
	int choose_get(int *);
};

struct PDB_atom {

	int index,serial,resid;
	string name,resname,segid;
	char chain,alt;
	float x,y,z,tof;
};

class Read_pdb {

	public:
	int loadfile(const char *);
	int loadfile(string&);
	int pdbclose();
	string pdbline(int);
	string pdbline(float,float,float,int);
	int N;
	PDB_atom *atom;
	string *line;
};
