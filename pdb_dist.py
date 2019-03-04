#!/usr/bin/python3

import numpy as np
from Bio.PDB.PDBParser import PDBParser
from Bio import PDB
from Bio.Seq import Seq
from Bio.Alphabet import ProteinAlphabet
import sys

pdb_name=sys.argv[1]
save_name=pdb_name[0:-4]+"_data.npy"

p = PDBParser(PERMISSIVE=0)
s = p.get_structure("1",pdb_name)
s = s[0]

res_list = PDB.Selection.unfold_entities(s, 'R')

aa_list = []
for a in res_list:
	if PDB.is_aa(a):
		aa_list.append(a)

res_name = [res.get_resname() for res in aa_list]

cb_list=[]
for a in aa_list:
	try:
		t=a['CB']
		cb_list.append(t)
	except:
		t=a['CA']
		cb_list.append(t)

cb_num=len(cb_list)
cb_dist=[a-b for a in cb_list for b in cb_list]
cb_dist=np.array(cb_dist)
cb_dist=cb_dist.reshape(cb_num,cb_num)

dssp=PDB.DSSP(s, pdb_name)
dssp_dict=dssp.property_dict

SS=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if dssp_dict.get(aa_id):
		SS.append(dssp_dict[aa_id][2])
	else:
		SS.append(None)

res_depth=[]
rd=PDB.ResidueDepth(s)
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	residue_depth, ca_depth=rd[aa_id]
	res_depth.append(residue_depth)

dic={}
dic['SS']=SS
dic['RD']=res_depth
dic['RN']=res_name
dic['DT']=cb_dist

np.save(save_name,dic)

#data=np.load(save_name).item()




