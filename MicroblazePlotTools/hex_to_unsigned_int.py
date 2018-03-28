
numbers = []
with open("temp.txt", "r") as f:
    lines = f.readlines()

    for l in lines:
        l = l.strip()
        me = l.split("Me:")[-1]

        num = int(me, 16)

        if (num > 2 ** 15):
            print(num)

        numbers.append(num)



print(numbers)
