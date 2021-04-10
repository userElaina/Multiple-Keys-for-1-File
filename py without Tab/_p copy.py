from math import log

_debug=False
if __name__=='__main__':
    _debug=True

n=None
len_pm=None

l_nw=0
lst_i=list()

def getj():
    global l_nw
    j=l_nw
    l_nw=(l_nw+1)%n
    return lst_i[j]


def starts(_psw:str,_n:int):
    global n,len_pm,lst_i
    n=_n
    len_pm=int(log(95)/log(n)*len(_psw))

    a=0
    for i in _psw:
        a=ord(i)-32+a*95
    b=list()
    while a:
        b.append(a%n)
        a//=n
    b=b[-1::-1]

    if len(b)>len_pm:
        b=b[:len_pm]
    while len(b)<len_pm:
        b.append(0)

    lst_i=b.copy()
    print(b)
    
def funcs(f:bytes,ans_xor:int=0):
    big=0
    big_flg=False
    ans=list()
    ans_bt=list()
    ans_bts=list()

    for i in f:
        a=int(i)
        bts=[0]*8
        for j in range(8):
            bts[7-j]=a&1
            a>>=1
        ans_bts+=bts
        while len(ans_bts)>=n:
            j=getj()
            ans_bt.append(ans_bts[j])
            ans_bts=ans_bts[n:]
        if len(ans_bt)>=8:
            a=0
            for j in range(8):
                a=a<<1|ans_bt[j]
            ans.append(a^ans_xor)
            ans_xor=a
            ans_bt=ans_bt[8:]
        if len(ans)>=4 and not big_flg:
            for j in range(4):
                big=big<<8|ans[j]
            big_flg=True
            ans=ans[4:]
        if big_flg and len(ans)==big:
            return ans
    
    return ans


def mian(_psw:str,in_pth:str,out_pth:str=None,_n:int=None,xor8:int=0):
    starts(_psw,_n)

    open(out_pth,'wb').write(bytes(funcs(open(in_pth,'rb').read(),xor8)))

if _debug:
    mian('12345678','0test_np.qwq3','01.txt',8,233)
    mian('7!S6eIo4','0test_np.qwq3','02.txt',8,233)

