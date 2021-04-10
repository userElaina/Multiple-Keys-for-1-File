from random import choice
from math import log
from json import dumps

_debug=False
if __name__=='__main__':
    _debug=True

def key_maker(pth:str,n:int=None,len_psw:int=None,lst_psw:list=None,xor8:int=0)->list:
    if (not n) or (not lst_psw and not len_psw):
        raise RuntimeError('缺条件你生成个锤子')
        exit()

    if not len_psw:
        len_psw=len(lst_psw[0])

    l=lst_psw.copy()
    len_pm=int(log(95)/log(n)*len_psw)

    _int=[0]*n
    _set=list()
    for i in range(len_pm):
        _set.append(list())

    i=0
    while i<n and i<len(l):
        a=0
        for j in l[i]:
            a=ord(j)-32+a*95
        _int[i]=a

        b=list()
        while a:
            b.append(a%n)
            a//=n
        b=b[-1::-1]

        if len(b)>len_pm:
            del l[i]
            continue
            
        while len(b)<len_pm:
            b.append(0)

        flg=False
        for j in range(len_pm):
            if b[j] in _set[j]:
                flg=True
        
        if flg:
            del l[i]
            continue

        for j in range(len_pm):
            _set[j].append(b[j])

        i+=1

    while i<n:
        a=0
        for j in range(len_pm):
            b=choice(list(set(range(n))-set(_set[j])))
            _set[j].append(b)
            a=b+a*n    
        _int[i]=a

        b=list()
        while a:
            b.append(a%n)
            a//=n
        b=b[-1::-1]

        i+=1
    
    _lst=[[j[i] for j in _set] for i in range(len(_set[0]))]
    ans=list()
    for i in _int:
        a=i
        b=list()
        while a:
            b.append(chr(a%95+32))
            a//=95
        while len(b)<n:
            b.append(chr(32))
        ans.append(''.join(b[-1::-1]))

    js={
        'psw':ans,
        'n':n,
        'pm':len_pm,
        'xor':xor8,
    }

    open(
        pth+'.json',
        'w',
        encoding='utf-8',
        errors='backslashreplace'
    ).write(
        dumps(
            js,
            indent='\t',
            ensure_ascii=False,
            skipkeys=True,
            sort_keys=True
        )
    )

    # for j in _int:
    #     print(j,end=' ')
    # print('\n')

    for i in _set:
        print(i)
    print('')

    for i in _lst:
        for j in i:
            print(j,end=' ')
        print('')
    print('')
    
    for i in ans:
        print(i)
    print('')

    return _lst

if _debug:
    l=['12345678','7!S6eIo4']
    key_maker('nfile/0test_psw',8,8,l)