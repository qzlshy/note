#!/usr/bin/python3

import numpy as np
from Bio.PDB.PDBParser import PDBParser
from Bio import PDB
from Bio.Seq import Seq
from Bio.Alphabet import ProteinAlphabet

p = PDBParser(PERMISSIVE=0)
s = p.get_structure("1","3skpFH.pdb")
s = s[0]

res_list = PDB.Selection.unfold_entities(s, 'R')

aa_list = []
for a in res_list:
	if PDB.is_aa(a):
		aa_list.append(a)

atom_list = PDB.Selection.unfold_entities(aa_list, 'A')

ca_list=[]
for a in atom_list:
	if a.get_name()=='CA':
		ca_list.append(a)

res_name = [res.get_resname() for res in aa_list]

HSEA=PDB.HSExposureCA(s)

HSEA_dict=HSEA.property_dict
HSEA_keys=HSEA.property_keys
HSEA_list=HSEA.property_list

HSEB=PDB.HSExposureCB(s)

HSEB_dict=HSEB.property_dict
HSEB_keys=HSEB.property_keys
HSEB_list=HSEB.property_list

depth=PDB.ResidueDepth(s)
dep_dict=depth.property_dict
dep_keys=depth.property_keys
dep_list=depth.property_list

dssp=PDB.DSSP(s, "3skpFH.pdb")
dssp_dict=dssp.property_dict

nb_dict={}
nb=PDB.NeighborSearch(ca_list)
for a in ca_list:
	t=nb.search(a.get_coord(),8)
	aa=a.get_parent()
	aa_id=(aa.get_parent().get_id(),aa.get_id())
	nb_dict[aa_id]=t
	

dic={}

dic["res_id"]=[]
for a in aa_list:
	dic["res_id"].append(a.get_id())

dic["res_name"]=[]
for a in aa_list:
	dic["res_name"].append(a.get_resname())

dic["HSEA"]=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if HSEA_dict.get(aa_id):
		dic["HSEA"].append(HSEA_dict[aa_id])
	else:
		dic["HSEA"].append(None)

dic["HSEB"]=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if HSEB_dict.get(aa_id):
		dic["HSEB"].append(HSEB_dict[aa_id])
	else:
		dic["HSEB"].append(None)

dic["RDEP"]=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if dep_dict.get(aa_id):
		dic["RDEP"].append(dep_dict[aa_id])
	else:
		dic["RDEP"].append(None)

dic["SS"]=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if dssp_dict.get(aa_id):
		dic["SS"].append(dssp_dict[aa_id][2])
	else:
		dic["SS"].append(None)

dic["rASA"]=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if dssp_dict.get(aa_id):
		dic["rASA"].append(dssp_dict[aa_id][3])
	else:
		dic["rASA"].append(None)

dic["nb"]=[]
for a in aa_list:
	aa_id=(a.get_parent().get_id(),a.get_id())
	if nb_dict.get(aa_id):
		dic["nb"].append(nb_dict[aa_id])
	else:
		dic["nb"].append(None)

