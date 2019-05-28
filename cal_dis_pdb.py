from Bio.PDB.MMCIF2Dict import MMCIF2Dict
import Bio
from Bio.PDB.MMCIFParser import MMCIFParser
from Bio import PDB
from Bio.PDB.PDBParser import PDBParser
from Bio import SeqIO
import numpy as np
p = PDBParser(PERMISSIVE=0)

seqdic={}
for seq_record in SeqIO.parse("pdbaa", "fasta"):
	seqdic[seq_record.id]=seq_record.seq

f=open('cullpdb_pc25_res2.0_R0.25_d181126_chains9311','r')
s=f.readline()
pdbid=[]
pdbchain=[]
pdbs=[]
while 1:
	s=f.readline().strip('\n')
	if not s:
		break
	t=s.split()[0][:4]
	t2=s.split()[0][4:]
	pdbid.append(t)
	pdbchain.append(t2)

for i in range(len(pdbid)):
	print(pdbid[i],pdbchain[i])
	pdb_name='pdb_/pdb'+pdbid[i].lower()+'.ent'
	s = p.get_structure("1",pdb_name)
	s = s[0][pdbchain[i]]
	res_list = PDB.Selection.unfold_entities(s, 'R')
	
	aa_list = []
	for a in res_list:
		if PDB.is_aa(a):
			aa_list.append(a)

	error=0
	t=aa_list[0].get_id()[1]
	aa_list_full=[]
	for a in aa_list:
		while 1:
			if a.get_id()[1]<t:
				error=1
				break
			if a.get_id()[1]==t:
				aa_list_full.append(a)
				t+=1
				break
			else:
				aa_list_full.append(None)
				t+=1
	if error==1:
		continue
	ca_list=[]
	for a in aa_list_full:
		try:
			t=a['CA']
			ca_list.append(t)
		except:
			t=None
			ca_list.append(t)
	ca_num=len(ca_list)
	ca_dist=[]
	for j in range(len(ca_list)):
		for k in range(len(ca_list)):
			if ca_list[j]!=None and ca_list[k]!=None:
				ca_dist.append(ca_list[j]-ca_list[k])
			else:
				ca_dist.append(None)
	ca_dist=np.array(ca_dist)
	ca_dist=ca_dist.reshape(ca_num,ca_num)
	out_name='pdb_dis/'+pdbid[i].lower()+pdbchain[i]+'.dat'
	f=open(out_name,'w')
	for j in range(len(ca_dist)):
		print(*ca_dist[j],file=f)
