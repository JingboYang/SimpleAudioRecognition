# We print things as float because Microblaze cannot handle double
# in an effective manner

PER_LINE = 11
SPACE_EACH = 15

# convert filter bank to string for C
def print_2d_c_arr(arr, name):
    output = "float " + name
    n_rows = len(arr)
    n_cols = len(arr[0])

    output += '[' + str(n_rows) + ']' + '[' + str(n_cols) + '] = {'
    header = "float " + name + '[' + str(n_rows) + ']' + '[' + str(n_cols) + '];'

    for i, r in enumerate(arr):
        output += ' {\n'
        for j, c in enumerate(r):
            
            output += ' ' + str(c).ljust(SPACE_EACH) + ' '
            if j != len(r) - 1:
                output += ','
            if (j + 1) % PER_LINE == 0:
                output += '\n'

        output += '} '
        if i != len(arr) - 1:
            output += ',\n\n'
    
    output += '};'

    return output, header


# convert filter bank to string for C
# print as 1d array to accomodate C
def print_2d_c_arr_as_1d(arr, name):
    n_rows = len(arr)
    n_cols = len(arr[0])

    output = "// " + name + " is " + '[' + str(n_rows) + ']' + '[' + str(n_cols) + ']\n'
    output += "float " + name

    output += '[' + str(n_rows * n_cols) + '] = {'
    header = "float " + name + '[' + str(n_rows * n_cols) + '];'

    for i, r in enumerate(arr):
        output += '\n'
        for j, c in enumerate(r):
            
            output += ' ' + str(c).ljust(SPACE_EACH) + ' '

            if not (i == n_rows - 1 and j == n_cols - 1):
                output += ','
                if (j + 1) % PER_LINE == 0:
                    output += '\n'

    output += '\n'
    output += '};'

    return output, header


# convert filter bank to string for C
def print_1d_c_arr(arr, name, d_type='float'):
    output = d_type + ' ' + name
    n_cols = len(arr)

    output += '[' + str(n_cols) + ']' + ' = {\n'
    header = d_type + ' ' + name + '[' + str(n_cols) + '];'

    for j, c in enumerate(arr):
        
        output += ' ' + str(c).ljust(SPACE_EACH) + ' '
        if j != len(arr) - 1:
            output += ','
        if (j + 1) % PER_LINE == 0:
            output += '\n'

    output += '};'

    return output, header



GROUPS = [['open', 'close', 'kai', 'guan', 'enter', 'light'], 
                ['deng', 'door', 'on', 'off']]

WORD_VALUE = {'open': '001', 'close': '002',
                'kai': '003', 'guan': '004',
                'enter': '005', 'light': '008',
                'door': '006', 'deng': '007',
                'on': '009', 'off': '00A'}

# copied from audio_library to avoid circular include
def get_word(string):
    w = string.strip().split('_')[0]
    #print(w)
    return w


def generate_c_lib(mfcc_map):

    keylist = list(mfcc_map.keys())
    keylist.sort()
    
    c_file = []
    vartables = []

    h_file = []
    defs = []
    vartables_h = []

    for i, g in enumerate(GROUPS):
      
        c_file.append('//--- Start of group ' + str(i + 1) + ' -----')
        h_file.append('//--- Start of group ' + str(i + 1) + ' -----')

        for k in keylist:
            if get_word(k) in g:
                c_t, h_t = print_2d_c_arr_as_1d(mfcc_map[k], k)
                c_file.append(c_t)
                h_file.append(h_t)

        c_file.append('//--- End of group ' + str(i + 1) + ' -----')
        h_file.append('//--- End of group ' + str(i + 1) + ' -----')


        cur_count = 0
        vartable = "vartab varTable_" + str(i + 1) + "[] = {\n"
        vartable_h = "vartab varTable_" + str(i + 1) + ";"
        for j, k in enumerate(keylist):
            
            if get_word(k) in g:
                vartable += '    {\"' + k + '\", '

                for s in WORD_VALUE:
                    if s in k:
                        vartable += s.upper() + ' ,'             
                        break        
                vartable += '&' + k + ', ' + str(len(mfcc_map[k])) + '}'
                cur_count += 1
                if j != len(keylist) - 1:
                    vartable += ',\n'
                else:
                    vartable += '\n'
            
        vartable += "};"
        vartables.append(vartable)
        vartables_h.append(vartable_h)
        

        defs.append('#define LEN_AUDIO_LIB_' + str(i + 1) + ' ' + str(cur_count))
        for s in WORD_VALUE:
            if s.lower() in g:
                defs.append('#define ' + s.upper().ljust(8) + ' 0x' + WORD_VALUE[s])



        
        

    # Print all the things we need to print
    print ('// For .c file')

    for v in vartables:
        print (v)
        print ('')

    print('\n\n')
    for t in c_file:
        print(t)
        print('')


    print('\n\n// For .h file')
    print ('// We must declare this stuff last so that referenced variables \n// are defined in the file')

    for d in defs:
        print(d)
    
    for v in vartables_h:
        print(v)

    print('\n\n')
    for t in h_file:
        print(t)
        


        
        
            