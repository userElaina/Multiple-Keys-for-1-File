import os
from random import choice 
from _psw import key_maker

_debug=False
if __name__=='__main__':
	_debug=True

l_ln=4

files=list()

files_name=None
files_len=None
files_nw=None
n=None
len_pm=None
lst_i=None
lst_nw=None

def getsz(i):
	a=int(os.path.getsize(files_name[i]))
	files_len[i]=a
	ans=[0]*l_ln
	k=l_ln-1
	while a:
		ans[k]=a&255
		a>>=8
		k-=1
	return ans+[int(j) for j in open(files_name[i],'rb').read()]

def geti(i:int):
	j=lst_nw[i]
	lst_nw[i]=(lst_nw[i]+1)%n
	return lst_i[i][j]

def getbit(i):
	j=files_nw[i]
	files_nw[i]+=1
	if i>=len(files):
		return choice([0,1]),False
	if (j>>3)>=len(files[i]):
		return choice([0,1]),False
	return (files[i][j>>3]>>(7-(j&7)))&1,True

def starts(_files_name:list,_lst_i:list,_n:int=None):
	global files_name,files_len,files_nw,n,len_pm,lst_i,lst_nw
	files_name=_files_name.copy()
	if not _n:
		n=len(_files_name)
	else:
		n=_n
	files_len=[0]*n
	files_nw=[0]*n
	lst_i=_lst_i.copy()
	len_pm=len(lst_i[0])
	lst_nw=[0]*n

def mian(pth:str,_files_name:list,_n:int=None,len_psw:int=None,lst_psw:list=None,xor8:int=0):

	starts(_files_name,key_maker(pth,_n,len_psw,lst_psw,xor8),_n)

	for i in range(len(files_name)):
		ans=[xor8,]
		for j in getsz(i):
			ans.append(ans[-1]^j)
		files.append(ans[1:])
		# print(ans)

	ans=list()
	file_out=pth+'.qwq3'
	open(file_out,'wb')

	while True:
		flg=True
		bts=[0]*n
		for i in range(n):
			j=geti(i)
			bts[j],ctn=getbit(i)
			if ctn:
				flg=False
		ans+=bts
		while len(ans)>=8:
			a=0
			for i in range(8):
				a=a<<1|ans[i]
			open(file_out,'ab').write(bytes([a,]))
			ans=ans[8:]
		if flg and len(ans)==0:
			break
		

if _debug:
	mian('0test_np',['1.txt','2.txt'],8,8,['12345678','7!S6eIo4'],233)
