def compute_word_frequencies(path):
    words_and_counts = {}
    with open(path, "r") as f:
        line = 1
        while line!= None:
            line = f.readline()
            if line == '':
                break
            words = line.split()
            for word in words:
                if word == '\n':
                    continue
                if word not in words_and_counts.keys():
                    words_and_counts[word] = 1
                else:
                    words_and_counts[word] += 1
    return words_and_counts

if __name__ == "__main__":
    print("Started\n")
    print(compute_word_frequencies("./test.txt"))