def count_rows_and_words(path):
    line_count = 0
    word_count = 0
    with open(path, "r") as f:
        line = 1
        while line!= None:
            line = f.readline()
            if line == '':
                break
            line_count += 1
            words = line.split(" ")
            for w in words:
                if w == '\n':
                    continue
                word_count += 1
    return (line_count, word_count)
            

if __name__ == "__main__":
    #testing
    print("Started")
    print(count_rows_and_words('./test.txt'))