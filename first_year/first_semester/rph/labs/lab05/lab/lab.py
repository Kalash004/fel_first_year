def line_column_similar_elements_size(start_row, start_col, data):
    # TODO: go thru row
    line = row_similar_elements_size(start_row, start_col,data)
    # TODO: go thru collumn
    # row = column_similar_elements_size()
    return line
    

def row_similar_elements_size(start_row, start_col, data):
    count_left = go(start_row, start_col, (0,-1), data)
    count_right = go(start_row, start_col, (0,1), data)
    count_up = go(start_row, start_col, (-1,0), data)
    count_down = go(start_row, start_col, (1,0), data)
    return count_left + count_right + count_up + count_down + 1 # 1 for element given

def region_size(start_row, start_col, data):
    count_left = go(start_row, start_col, (0,-1), data)
    count_right = go(start_row, start_col, (0,1), data)
    count_up = go(start_row, start_col, (-1,0), data)
    count_down = go(start_row, start_col, (1,0), data)
    count_cross_right_up = go(start_row, start_col, (-1,1), data)
    count_cross_left_up = go(start_row, start_col, (-1,-1), data)
    count_cross_right_down = go(start_row, start_col, (1,1), data)
    count_cross_left_down = go(start_row, start_col, (1,-1), data)
    return count_left + count_right + count_up + count_down + count_cross_right_up + count_cross_left_up + count_cross_right_down + count_cross_left_down + 1

def go(start_row, start_col, step, data):
    count = 0
    element = data[start_row][start_col]
    col, row = start_col, start_row
    max_height = len(data) - 1
    max_width = len(data[start_row]) - 1
    while (col < max_width and row < max_height and col >= 0 and row >= 0):
        col += step[1]
        row += step[0]
        test_subject = data[row][col]
        if (test_subject != element):
            return count
        count += 1
    return count
    

def go_left(col, row):
    count = 0
    element_to_look_for = row[col]
    for i in range(len(row), 0):
        if (row[i] != element_to_look_for):
            return count
        count += 1
    return count

def go_right(col, row):
    count = 0
    element_to_look_for = row[col]
    for i in range(col,len(row)-1):
        if (row[i] != element_to_look_for):
            return count
        count += 1
    return count

def main():
    data = [
        [1, 0, 0, 0, 1, 1, 0, 0 ] ,
        [1, 1, 1, 0, 0, 1, 1, 1 ] ,
        [0, 1, 0, 0, 1, 1, 1, 1 ] ,
        [0, 1, 0, 1, 0, 1, 1, 1 ] ,
        [0, 1, 1, 0, 0, 0, 1, 1 ] ,
        [1, 0, 0, 0, 1, 1, 0, 0 ] ,
        [0, 0, 1, 0, 1, 1, 1, 0 ] ,
        [0, 0, 1, 0, 1, 0, 1, 0 ]] 
    print(region_size(2, 6, data))


if __name__ == "__main__":
    main()