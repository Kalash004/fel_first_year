def get_matrix_data(matrix, center):
    data = ""
    max_columns = len(matrix)
    max_rows = len(matrix[0])
    """Matrix = [rows[]]"""
    for i in range(0, max_columns):
        for j in range(0, max_rows):
            s = f"{matrix[i][j]}"
            data += s.center(center)
        data += "\n"
    return data

def generate_matrix(num_rows, num_cols):
    matrix = []
     num_rows * 10
    for i in range(0, num_rows):
        row = []
        for j in range(0, num_cols):
            row.append(i*10 + j)
        matrix.append(row)
    return matrix

if __name__ == "__main__":
    m = [[1, 2, 3],[4, 5, 6],[7, 8, 9]]
    print(get_matrix_data(m, 5))
    mn = generate_matrix(4, 5)
    print(get_matrix_data(mn, 4))