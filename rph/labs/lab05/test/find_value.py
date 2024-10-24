def value_count(data, value):
    count = 0
    for row in data:
        for elem in row:
            if (value == elem):
                count += 1
    return count

def value_positions(data, value):
    locations = []
    for row_index,row in enumerate(data):
        for col_index,elem in enumerate(row):
            if (value == elem):
                locations.append((row_index, col_index))
    return locations

def main():
    data = [[0,-1,1],[-1,0,-1],[1,0,-1],[0,-1,1],[0,-1,1],[0,-1,1]]
    print(value_count(data,1))
    print(value_position(data,0))


if __name__ == "__main__":
    pass