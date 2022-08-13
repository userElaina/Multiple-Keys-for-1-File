# Multiple Keys for 1 File

[中文](README_cn.md)

Multiple keys for multiple files packed into one large file.

Given a key, you can extract the corresponding file.

At the same time, you can generate multiple partly true and partly false key sequences. In this way, you can safely hide the files you want to hide under duress.

## Algorithm

### Function

$$
\rm Encrypt(key_1,\cdots,key_n,file_1,\cdots,file_n)=bigfile,
$$

$$
\rm Decrypt(key_x,bigfile)=file_x.
$$

Even if know one $\rm key$, the rest still needs to be brute-force attacked.

### Sequence

In the encoding process, each $\rm key_x$ is used to generate the sequence $\rm l_x.$ It becomes an infinite sequence through the ring structure, and satisfies

$$
\rm l_i[k]\neq l_j[k],l_i[k]\in\N\cap[0,n),\forall i\neq j,\forall k.
$$

As you can see, these requirements are very demanding. So it is recommended to use the program to generate key sequences.

$\rm key\to int(base\ n)\to l_x.$

### File Structure

The $\rm l_x$-th bit of the $\rm k$-th consecutive $\rm n$ bits of the large file, is exactly the $\rm k$-th bit of the small file $\rm file_x,$ i.e.

$$
\rm bigfile[(k-1)*n+l_x[k]]=file_x[k].
$$

When $\rm file_x[k]$ is not available, randomly generate it.

### Brute-force Attack Time Complexity

$k$ keys, key length $l$ and number of files $n$ are known.

The maximum minimum cycle length of the subscript sequence is $m=\lceil\log_n256^l\rceil=\lceil8l\frac{\ln2}{\ln n}\rceil,$ and the time complexity of traversing all possible passwords is $O((n-k)^m)\simeq O(256^l).$

### Other Possible Ways to Crack

One of the files is known, but the number of files, any keys or its length are not known. In this case, the time complexity required to crack the key corresponding to this file by means of brute-force attacking is $O(m^2n\log n).$

## Interface

### encrypt

#### Declaration

```cpp
int encrypt(int n, int kn, char **ks, char **fs, const char *big = "big.bin");
```

#### Variables

|Name   |Type           |Explain|
|-      |-              |-|
|`n`    |`int`          |the number of files (and keys) $n$|
|`ks`   |`int`          |key length $l$|
|`fs`   |`char **`      |small file path|
|`big`  |`const char *` |large file (generation) path|
|return |`int`          |`0` is the normal return|

#### Function

Save the keys and generate the large file.

### decrypt

#### Declaration

```cpp
int decrypt(const char *fk, const char *big, const char *fx);
```

#### Variables

|Name   |Type           |Explain|
|-      |-              |-|
|`fk`   |`const char *` |key file path|
|`big`  |`const char *` |large file path|
|`fx`   |`const char *` |corresponding small file (generation) path|
|return |`int`          |`0` is the normal return|

#### Function

Decrypt the corresponding small file.
