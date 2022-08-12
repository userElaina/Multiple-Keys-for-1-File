def basesize(x: int, maxlen: int = 8, unbase: bool = False, full: bool = False) -> list:
    ans = list()
    eb = 1 << (maxlen << 3) if full else 256
    es = 1
    ls = 0
    for lb in range(maxlen if full else 1, maxlen + 1):
        assert eb == 1 << (lb << 3)
        assert es == x ** ls

        if unbase:
            while eb >= es * x:
                ls += 1
                es *= x
            assert eb >= es
            assert eb < es * x
            p = es / eb
        else:
            while eb > es:
                ls += 1
                es *= x
            assert eb <= es
            assert eb * x > es
            p = eb / es
        ans.append((p, ls, lb,))
        if eb == es:
            break
        eb <<= 8
    return sorted(ans)


for i in range(2, 256):
    a = basesize(i, unbase = True)
    print(i, '%.2f %d %d' % a[-1])
    # if i % 8 == 0:
    #     print()
    # print('{%d,%d},' % a[-1][1:], end='')

# s = open('rd.key', 'rb').read()
# while s:
#     while len(s) < 5:
#         s = s+b'\x00'
#     a = int.from_bytes(s[:5], 'little')
#     a = int.from_bytes(s[:5], 'big')
#     print('%012d' % a, end=' ')
#     s = s[5:]
