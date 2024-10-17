class MyVector:
    def __init__(self, lst: [int]): 
        self.lst = lst;

    def get_vector(self):
        return self.lst;

    def get_vector_length(self) -> int:
        return len(self.lst);

    def __mul__(self, other):
        if self.get_vector_length() != other.get_vector_length():
            return -1;
        retr = 0;
        for i,this_vector in enumerate(self.lst):
            other_vector = other.lst[i];
            retr += this_vector * other_vector;
        return retr;


if __name__ == "__main__":
    vec1 = MyVector([1,2,3]) # vektory mohou byt i jine dimenze nez 3!
    vec2 = MyVector([3,4,5]) 
    print(vec1.get_vector()) # priklad ziskani seznamu
    dot_product = vec1*vec2  # vypocet skalarniho soucinu, pretizeny operator *, vola se __mul__
    print(dot_product)       # jen kontrolni vypis