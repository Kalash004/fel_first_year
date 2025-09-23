def read_classification_from_file(path_to_file) -> dict:
    target = {}
    with open(path_to_file,'r', encoding="utf-8") as f:
        line = 0
        while line != '':
            line = f.readline()
            if line == '':
                break
            words = line.split(" ")
            target[words[0].replace("\n","")] = words[1].replace("\n","")
    return target