import os, sys, math
import numpy as np
import pprint as pp
import matplotlib.pyplot as plt


def uniqueish_color():
    """There're better ways to generate unique colors, but this isn't awful."""
    return plt.cm.gist_ncar(np.random.random())

color_list = [uniqueish_color() for i in range(200)]


def cosine_dist(v1, v2):
    assert len(v1) == len(v2)

    sum_top = 0
    sum_v1 = 0
    sum_v2 = 0
    for i in range(len(v1)):
        sum_top += v1[i] * v2[i]
        sum_v1+= v1[i] * v1[i]
        sum_v2+= v2[i] * v2[i]
    
    result = sum_top / math.sqrt(sum_v1) / math.sqrt(sum_v2)

    return 1 - result


def cart_dist(v1, v2):
    assert len(v1) == len(v2)

    s = 0
    for i in range(len(v1)):
        s += (v1[i] - v2[i]) * (v1[i] - v2[i])

    s = math.sqrt(s)
    return s


def dtw(a1, a2, need_path=False):
    assert len(a1.shape) == 2
    assert len(a2.shape) == 2

    assert a1.shape[1] == a2.shape[1]

    dtw_array = np.zeros((len(a1) + 1, len(a2) + 1))
    dtw_path = [[None for i in range(len(a2) + 1)] for j in range(len(a1) + 1)]
    #print(dtw_array)

    for i in range(1, len(a1) + 1):
        dtw_array[i][0] = np.linalg.norm(a1[i - 1])

    for i in range(1, len(a2) + 1):
        dtw_array[0][i] = np.linalg.norm(a2[i - 1])
    
    dtw_array[0][0] = 0

    
    #len_diff = 1
    for i in range(1, len(a1) + 1):
        for j in range(1, len(a2) + 1):
            #cost = cosine_dist(a1[i - 1], a2[j - 1])
            cost = cart_dist(a1[i - 1], a2[j - 1])
            #print(cost)
            #print(dtw_array)
            
            #three = [dtw_array[i - 1][j], dtw_array[i][j - 1], dtw_array[i-1][j-1]]
            #avg_three = np.mean(three)

            len_diff = max(abs(i - j), 1)

            d1 = cost * len_diff + dtw_array[i - 1][j]
            d2 = cost * len_diff + dtw_array[i][j - 1]
            d3 = cost * max(len_diff / 2, 1) + dtw_array[i-1][j-1]

            min_val = d3
            if need_path:
                coords = [(i-1, j), (i, j-1), (i-1, j-1)]
                #min_arg = np.argsort([dtw_array[i - 1][j], dtw_array[i][j - 1], dtw_array[i-1][j-1]])[0]
                min_arg = np.argsort([d1, d2, d3])[0]
                min_coord = coords[min_arg]
                #min_val = dtw_array[min_coord[0]][min_coord[1]]

                #dtw_array[i][j] = cost + min_val
                dtw_path[i][j] = min_coord
            else:
                min_val = min(d1, d2, d3)

            dtw_array[i][j] = min_val

        
    '''
    #print(dtw_array)
    pp.pprint(dtw_path)
    
    #pp.pprint(dtw_array)
    outstring = ''
    for j in range(len(dtw_array[0])):
        if j == 0:
            outstring += ' '.ljust(10 * 2)
        else:
            outstring += str(a2[j - 1]).ljust(10) 
    print(outstring)
    '''

    '''
    for i, a in enumerate(dtw_array):
        if i != 0 and False:
            outstring = str(a1[i - 1]).ljust(10)
        else:
            outstring = ' '.ljust(10)
        for j, b in enumerate(a):
            outstring += str(round(b, 3)).ljust(12)
        print(outstring)
    '''
    
    last_dist = dtw_array[len(a1)][len(a2)]
    #avg_dist = last_dist / len(a1)
    #extra_dist = (abs(len(a1) - len(a2)) ** 2) * avg_dist
    #extra_dist = (abs(len(a1) - len(a2))) * (avg_dist * abs(len(a1) - len(a2)) / min(len(a1), len(a2)))
    #total_dist = last_dist + extra_dist
    total_dist = last_dist

    #print('Total dist: ' + str(total_dist))
    return total_dist, dtw_path



def trace_path(dtw_path):
    path = []
    last = dtw_path[-1][-1]

    while last is not None:
        path.append(last)
        last = dtw_path[last[0]][last[1]]

    path.reverse()

    result = ''
    for p in path:
        result += str(p) + '->'

    result = result[:-2]
    print(result)
    return path


def fill_vectors(path, v1, v2):

    v1_filled = []
    v2_filled = []
    v1_repeated = []
    v2_repeated = []

    v1_prev = -1
    v2_prev = -1
    for p in path:
        v1_filled.append(v1[p[0] - 1])
        v2_filled.append(v2[p[1] - 1])

        if p[0] == v1_prev:
            v1_repeated.append(True)
        else:
            v1_repeated.append(False)

        if p[1] == v2_prev:
            v2_repeated.append(True)
        else:
            v2_repeated.append(False)

        v1_prev = p[0]
        v2_prev = p[1]

    return v1_filled, v2_filled, v1_repeated, v2_repeated


def generate_segments(vector, repeated):

    segs = []
    seg_repeated = []
    
    cur_seg = []
    cur_is_repeated = repeated[0]
    for i, v in enumerate(vector):
        if repeated[i] != cur_is_repeated:
            cur_seg.append(vector[i])
            segs.append(cur_seg)
            seg_repeated.append(cur_is_repeated)
            cur_seg = []
        
        cur_seg.append(v)
        cur_is_repeated = repeated[i]

    if len(cur_seg) > 0:
        segs.append(cur_seg)
        seg_repeated.append(cur_is_repeated)

    return segs, seg_repeated


def sequence_generator(a1_dim, a2_dim):
    
    if a1_dim > a2_dim:
        long_dim = a1_dim
        short_dim = a2_dim
        long_v = 1
    else:
        long_dim = a2_dim
        short_dim = a1_dim
        long_v = 2
    
    vlong = np.random.rand(long_dim[0], long_dim[1]) * 10
    vshort_indicies = np.random.choice(long_dim[0], short_dim[0], replace=False)
    vshort_indicies.sort()
    vshort = vlong[vshort_indicies]

    vshort += np.random.rand(short_dim[0], short_dim[1])

    if long_v == 1:
        return vlong, vshort
    else:
        return vshort, vlong


def plot_sequences(s1, s2):

    s1_norm = [np.linalg.norm(s) for s in s1]
    s2_norm = [np.linalg.norm(s) for s in s2]

    plt.subplot(211)
    plt.bar(np.arange(len(s1_norm)), s1_norm)

    plt.subplot(212)
    plt.bar(np.arange(len(s2_norm)), s2_norm)

    plt.show()


def plot_segments(segs1, segs2, segs1_repeated, segs2_repeated):
    
    max_val = -1
    min_val = 999999
    for seg in segs1:
        max_val = max(max([np.linalg.norm(s) for s in seg]), max_val)
        min_val = min(min([np.linalg.norm(s) for s in seg]), min_val)
    
    for seg in segs2:
        max_val = max(max([np.linalg.norm(s) for s in seg]), max_val)
        min_val = min(min([np.linalg.norm(s) for s in seg]), min_val)


    offset = 1.5 * (max_val - min_val)


    base = 0
    for i, seg in enumerate(segs1):
        s_norms = np.array([np.linalg.norm(s) for s in seg])
        
        color = 'b'
        if segs1_repeated[i]:
            color = 'r'
        #plt.bar(np.arange(len(s_norms)) + base, s_norms, color=color)
        plt.plot(np.arange(len(s_norms)) + base, s_norms + offset, color=color)
        #plt.vlines(len(s_norms) + base - 1, min_val, max_val + offset)
        base += len(s_norms) - 1


    base = 0
    for i, seg in enumerate(segs2):
        s_norms = [np.linalg.norm(s) for s in seg]
        color = 'b'
        if segs2_repeated[i]:
            color = 'r'
        #plt.bar(np.arange(len(s_norms)) + base, s_norms, color=color)
        plt.plot(np.arange(len(s_norms)) + base, s_norms, color=color)
        #plt.vlines(len(s_norms) + base - 1, min_val, max_val + offset)
        base += len(s_norms) - 1

    plt.show()


def dtw_workflow(v1, v2, extra_work=False):
    dtw_dist, dtw_path = dtw(v1, v2, extra_work)
    #print('Total dist: ' + str(dtw_dist))

    if extra_work:
        path = trace_path(dtw_path)

        v1_filled, v2_filled, v1_repeated, v2_repeated = fill_vectors(path, v1, v2)
        v1_v, v1_r = generate_segments(v1_filled, v1_repeated)
        v2_v, v2_r = generate_segments(v2_filled, v2_repeated)
        plot_segments(v1_v, v2_v, v1_r, v2_r)

    return dtw_dist


if __name__ == '__main__':

    a1_dim = (20, 10)
    a2_dim = (90, 10)
    
    #v1, v2 = sequence_generator(a1_dim, a2_dim)

    #TEST_V1 = np.array([1,2,2,3]).reshape(4,1)
    #TEST_V2 = np.array([1,0,2]).reshape(3,1)

    TEST_V1 = np.array([ [1,1],
                        [2,2],
                        [1,1],
                        [2,2],
                        [2,1],
                        [2,2]])

    TEST_V2 = np.array([ [1,1],
                        [2,2],
                        [2,2.5],
                        [2,1],
                        [2,2],
                        [1,1],
                        [2,2],
                        [1,2],
                        [2,2]])

    result = dtw_workflow(TEST_V1, TEST_V2, True) 

    print(result)