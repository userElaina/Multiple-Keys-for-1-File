import os
import random

def basesize(x:int,maxlen:int=8,unbase:bool=False)->list:
    ans=list()
    n=1
    r=x
    for i in range(1,maxlen+1):
        l=256**i
        if unbase:
            while l>=r*x:
                n+=1
                r*=x
            p=l/r
        else:
            while l>r:
                n+=1
                r*=x
            p=r/l
        ans.append((p,n,i))
        if l==r:
            break
    return sorted(ans)


def pwd2lst(n:int,pwd:bytes):
    if n<=1:
        raise ValueError('n must be greater than 1!')

    blocklen_ptr,blocklen_pwd=basesize(n,5,True)[0][1:]
    blockmax_pwd=(256**blocklen_pwd)-1
    blockmax_ptr=n**blocklen_ptr
    # blocksalt=0
    blocksalt=(blockmax_pwd-n**blocklen_ptr+1)>>1

    len_pwd=len(pwd)
    num_block=(len_pwd-1)//blocklen_pwd+1
    len_pwd=num_block*blocklen_pwd
    len_ptr=num_block*blocklen_ptr
    # print(len_ptr,len_pwd)

    i=int.from_bytes(pwd,byteorder='big')
    lst=list()
    for _j in range(num_block):
        j=((i&blockmax_pwd)-blocksalt)%blockmax_ptr
        for _k in range(blocklen_ptr):
            k=_k+_j*blocklen_ptr
            l=j%n
            lst.append(l)
            j//=n
        i>>=blocklen_pwd<<3

    return lst[::-1]


def key_maker(n:int,len_pwd:int=-1,wishlist:list=list())->list:
    if n<=1:
        raise ValueError('n must be greater than 1!')

    blocklen_ptr,blocklen_pwd=basesize(n,5,True)[0][1:]
    blockmax_pwd=(256**blocklen_pwd)-1
    blockmax_ptr=n**blocklen_ptr
    # blocksalt=0
    blocksalt=(blockmax_pwd-n**blocklen_ptr+1)>>1

    for i in wishlist:
        len_pwd=max(len_pwd,len(i))
    if len_pwd<0:
        len_pwd=0xf
    num_block=(len_pwd-1)//blocklen_pwd+1
    len_pwd=num_block*blocklen_pwd
    len_ptr=num_block*blocklen_ptr
    # print(len_ptr,len_pwd,blocksalt)

    used=list()
    for i in range(len_ptr):
        used.append(list())
    for _i in wishlist:
        i=int.from_bytes(_i,byteorder='big')
        lst=list()
        for _j in range(num_block):
            j=((i&blockmax_pwd)-blocksalt)%blockmax_ptr
            flg=False
            for _k in range(blocklen_ptr):
                k=_k+_j*blocklen_ptr
                l=j%n
                if l in used[k]:
                    flg=True
                    break
                lst.append(l)
                j//=n
            if flg:
                break
            i>>=blocklen_pwd<<3
        else:
            for _j,j in enumerate(lst[::-1]):
                used[_j].append(j)

    pwd=[b'']*n
    ptr=list()
    for i in range(len_ptr):
        ptr.append([n]*n)
    for _j in range(num_block):
        j=[0]*n
        for _k in range(blocklen_ptr):
            k=_k+_j*blocklen_ptr
            l=[_l for _l in range(n) if _l not in used[k]]
            random.shuffle(l)
            used[k]+=l
            for _i in range(n):
                i=used[k][_i]
                j[_i]=j[_i]*n+i
                ptr[k][i]=_i
        for _i in range(n):
            i=(j[_i]+blocksalt)%blockmax_ptr
            _salt=(blockmax_pwd-i)//blockmax_ptr
            i+=blockmax_ptr*random.randint(0,_salt)
            pwd[_i]+=i.to_bytes(blocklen_pwd,'big')

    # for i in used:
    #     print(i)
    for _i,i in enumerate(pwd):
        for _j,j in enumerate(pwd2lst(n,i)):
            assert j==used[_j][_i]

    return pwd,ptr


if __name__=='__main__':
    # n=7
    # pwd,ptr=key_maker(n,4)
    # for i in pwd:
    #     print(pwd2lst(n,i),i)
    # for i in ptr:
    #     print(i)
    import time
    for i in range(2,257):
        a=time.time()
        for _i in range(100):
            __i=key_maker(i,wishlist=[b'qaq'])
        print(i,(time.time()-a)/100,len(__i[0][0]))
