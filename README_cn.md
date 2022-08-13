# Multiple Keys for 1 File

多文件多密钥打包成一个大文件.

给一个正确的密钥,可以提取对应文件.

同时,你可以生成多个部分真部分假的密钥序列.这样,你可以在受到胁迫的情况下安全地隐藏想要隐藏的文件.

## 算法介绍

### 理想状态的函数

$$
\rm Encrypt(key_1,\cdots,key_n,file_1,\cdots,file_n)=bigfile,
$$

$$
\rm Decrypt(key_x,bigfile)=file_x.
$$

就算知道了一个 $\rm key,$ 还是只能暴力破解.

### 序列

编码过程中,需要用每个 $\rm key_x$ 生成序列 $\rm l_x.$ 它通过环状结构成为无限序列,且满足

$$
\rm l_i[k]\neq l_j[k],l_i[k]\in\N\cap[0,n),\forall i\neq j,\forall k.
$$

可见,这个要求还是很苛刻的,所以建议使用程序生成随机密码序列.

$\rm key\to int(base\ n)\to l_x.$

### 文件结构

大文件的第 $\rm k$ 段连续的 $\rm n$ 个bit中的第 $\rm l_x[k]+1$ 个,恰好是小文件 $\rm file_x$ 的第 $\rm k$ 位,即

$$
\rm bigfile[(k-1)*n+l_x[k]]=file_x[k].
$$

当娶不到 $\rm file_x[k],$ 随机生成即可.

### 暴力破解时间复杂度

已知 $k$ 个 $\rm key,$ 长度 $l,$ 文件数量 $n.$

则下标序列的最大最小循环节长度为 $m=\lceil\log_n256^l\rceil=\lceil8l\frac{\ln2}{\ln n}\rceil,$ 遍历所有可能密码的时间复杂度为 $O((n-k)^m)\simeq O(256^l).$

### 其它可能的破解方式

对方知道其中一个文件 $\rm file_x[k],$ 但不知道文件数量、任何密码及其长度。此时，暴力破解出此文件对应的密码所需时间复杂度望为 $O(m^2n\log n).$

## 接口

### encrypt

#### 函数声明

```cpp
int encrypt(int n, int kn, char **ks, char **fs, const char *big = "big.bin");
```

#### 变量

|Name   |Type           |Explain|
|-      |-              |-|
|`n`    |`int`          |文件(及密钥)数量 $n$|
|`ks`   |`int`          |密钥长度 $l$|
|`fs`   |`char **`      |小文件路径|
|`big`  |`const char *` |大文件(生成)路径|
|return |`int`          |`0` 为正常返回|

#### 功能

保存密钥并生成大文件.

### decrypt

#### 函数声明

```cpp
int decrypt(const char *fk, const char *big, const char *fx);
```

#### 变量

|Name   |Type           |Explain|
|-      |-              |-|
|`fk`   |`const char *` |密钥文件路径|
|`big`  |`const char *` |大文件路径|
|`fx`   |`const char *` |对应小文件(生成)路径|
|return |`int`          |`0` 为正常返回|

#### 功能介绍

Decrypt 出对应小文件.
