import random
import subprocess

with open('training.txt') as f:
    all_data = map(str.strip, f.readlines())

random.shuffle(all_data)

total = len(all_data)

k_fold = 5
group_count = total / k_fold
remainder = total  % k_fold
groups = []

index = 0
for i in range(0,remainder):
    new_index = index + group_count + 1
    groups.append(all_data[index:new_index])
    index = new_index

new_iter = k_fold - remainder

for i in range(0,new_iter):
    new_index = index + group_count + 1
    groups.append(all_data[index: new_index])
    index = new_index


total_match = 0
total_count = 0

for iteration in range(0, k_fold):
    test_data = groups[iteration]

    data = []

    for temp in range(0,k_fold):
        if temp == iteration:
            continue
        data.extend(groups[temp])

    with open("testing.data", "w") as f:
        for val in test_data:
            val = val.split(" ", 1)
            graph = val[0].strip()
            phones = val[1].strip()
            phones_count = len(phones.split(" "))
            f.write(str(phones_count) + "\n" + phones + "\n" + graph + "\n")
        f.write("0")

    with open("training.data", "w") as f:
        for val in data:
            f.write(val + "\n")
        f.write("--")

    output = subprocess.check_output("./a.out")

    output = output.strip()
    output = output.split('\n')

    for val in output:
        total_match += int(val)
        total_count += 65

    print "One LOOP Done"


percentage = (float(total_match)/total_count)*100

print "Accuracy in 5 fold validation is %.2f%%" % percentage

