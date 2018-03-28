import math

decimals = []

for i in range(24):
    #decimals.append(math.pow(0.5, i))
    decimals.append(math.pow(2, i))

print(decimals)

'''
decimals = [1.0, 0.5, 0.25, 0.125, 0.0625, 0.03125, \
0.015625, 0.0078125, 0.00390625, 0.001953125, 0.0009765625, \
0.00048828125, 0.000244140625, 0.0001220703125, 6.103515625e-05, \
3.0517578125e-05, 1.52587890625e-05, 7.62939453125e-06, 3.814697265625e-06, \
1.9073486328125e-06, 9.5367431640625e-07, 4.76837158203125e-07, \
2.384185791015625e-07, 1.1920928955078125e-07]
'''

[1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, \
128.0, 256.0, 512.0, 1024.0, 2048.0, \
4096.0, 8192.0, 16384.0, 32768.0, 65536.0, \
131072.0, 262144.0, 524288.0, 1048576.0, \
2097152.0, 4194304.0, 8388608.0]

def convert(x):
    temp = list('{0:0b}'.format(x))
    print(temp)

    orig_length = len(temp)
    for i in range(orig_length, 24):
        temp.insert(0, 0)
    
    assert len(temp) == 24

    if temp[0] == '1':
        mul = -1
        for i, t in enumerate(temp):
            temp[i] = 1 - int(t)
    else:
        mul = 1
        for i, t in enumerate(temp):
            temp[i] = int(t)

    val = 0
    for i, b in enumerate(temp):
        val += decimals[len(decimals) - i - 1] * int(b)

    val = val * mul
    #print(''.join(temp))
    print(temp)
    #print(val)

    return val


def convert_2(x):
    if (x & 0x800000):
        x |= ~0xffffff
    x = float(x) / (2 ** 23)    
    return x


def convert_3(x):
    #print(x & 0xff8000)
    #if (x & 0xffff8000):
    if (x & 0x800000):
        x |= ~0xffffff
    else:
        pass
        #print('positive?')
    x -= 5000
    #x = float(x) / (2 ** 15)
    return x

if __name__ == '__main__':
    print(convert(5786))
    #print(convert_2(16777214))
    #print(convert(16777214))
    #print(convert(-94306268))
    print(convert_3(16776818))
    #print(convert_2(int(convert(16777214))))
    #print(~0xffffff)



    import time
    import math

    start = time.time()
    garbage = 0
    for i in range(1000):
        garbage += math.pow(i, 3)

    end = time.time()
    print(end - start)