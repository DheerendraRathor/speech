import json
import random
import subprocess
import sys
from constants import compilation_string, equal_graph_phones_file, bin_dir, bin_file, \
training_data_file, testing_data_file, initial_probabilities_file

from collections import defaultdict

json_data = open(equal_graph_phones_file).read()


data = json.loads(json_data)
random.shuffle(data)

json_dict = defaultdict(lambda: defaultdict(lambda: defaultdict(lambda: defaultdict(int))))

init_prob_1 = defaultdict(float)

init_prob_2 = defaultdict(float)

total = len(data)

k_fold = 5
group_count = total / k_fold
remainder = total  % k_fold
groups = []

index = 0
for i in range(0,remainder):
    new_index = index + group_count + 1
    groups.append(data[index:new_index])
    index = new_index

new_iter = k_fold - remainder

for i in range(0,new_iter):
    new_index = index + group_count + 1
    groups.append(data[index: new_index])
    index = new_index


diff_count = 0
total_phones = 0

for iteration in range(0,1):
    test_data = groups[iteration]
    test_dict = {}
    data = []

    for temp in range(0,k_fold):
        if temp == iteration:
            continue
        data.extend(groups[temp])

    try:
        with open(testing_data_file, 'w') as outfile:
            for elem in test_data:
                outfile.write(elem[0]+"\n")
                test_dict[elem[0]] = elem[1]
            outfile.write("-\n")
    except IOError as e:
        sys.stderr.write(e)
        sys.exit(0)

    for elem in data:
        graph = elem[0]
        phone = elem[1]
        phones = phone.split(' ')
        phones.append('.')
        phones.append(',')
        graph_list = list(graph)
        for i in range(0,len(phones)-2):
            json_dict[phones[i]][phones[i+1]][phones[i+2]][graph_list[i]] += 1
        init_prob_1[phones[0]] += 1
        init_prob_2[phones[1]] += 1

    probability = defaultdict(lambda: defaultdict(lambda: defaultdict(lambda: defaultdict(float))))
    count_1 = defaultdict(int)

    for phone_1 in json_dict:
        for phone_2 in json_dict[phone_1]:
            for phone_3 in json_dict[phone_1][phone_2]:
                for key in json_dict[phone_1][phone_2][phone_3]:
                    count_1[phone_1] += json_dict[phone_1][phone_2][phone_3][key]


    for phone_1 in json_dict:
        for phone_2 in json_dict[phone_1]:
            for phone_3 in json_dict[phone_1][phone_2]:
                for key in json_dict[phone_1][phone_2][phone_3]:
                    probability[phone_1][phone_2][phone_3][key] = float(json_dict[phone_1][phone_2][phone_3][key])/float(count_1[phone_1])


    data_fin = []
    for phone_1 in sorted(probability):
        for phone_2 in sorted(probability[phone_1]):
            for phone_3 in sorted(probability[phone_1][phone_2]):
                for key in sorted(probability[phone_1][phone_2][phone_3]):
                    string = ""
                    string += phone_1 + " "
                    string += phone_2 + " "
                    string += phone_3 + " "
                    string += key + " "
                    string += str(probability[phone_1][phone_2][phone_3][key])
                    data_fin.append(string)

    try:
        with open(training_data_file, 'w') as outfile:
            for datum in data_fin:
                outfile.write(datum + "\n")
            outfile.write("--\n")
    except IOError as e:
        sys.stderr.write(e)
        sys.exit(0)

    try:
        with open("temp/init_1.data", 'w') as outfile:
            for phone in sorted(init_prob_1):
                outfile.write(phone+" "+str(init_prob_1[phone]/len(data))+"\n")
            outfile.write("--\n")
        with open("temp/init_2.data", 'w') as outfile:
            for phone in sorted(init_prob_2):
                outfile.write(phone+" "+str(init_prob_2[phone]/len(data))+"\n")
            outfile.write("--\n")
    except IOError as e:
        sys.stderr.write(e)
        sys.exit(0)

    print "All data generated"

    output = ""

    try:
        f = open(testing_data_file)
        try:
            output = subprocess.check_output(bin_file, stdin = f)
        except:
            print output
        finally:
            f.close()
    except IOError as e:
        sys.stderr.write(e)
        exit(0)

    output = output.strip()
    output = output.split('\n')
    
    for i in range(0,len(test_data)):
        a = test_data[i][1].split()
        b = output[i].split()
        try:
            for i in range(0,len(a)):
                if a[i] != b[i]:
                    diff_count += 1
                total_phones += 1
        except:
            print a,b,i,diff_count, total_phones

print "Accuracy achieved in %d-fold validation: %.2f%%" % (k_fold, float(total_phones - diff_count)* 100/float(total_phones))