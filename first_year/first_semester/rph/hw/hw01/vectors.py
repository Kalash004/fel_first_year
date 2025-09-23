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

    def is_perpendicular_to(self, other):
        return self * other == 0
    
    
    def cross_product(self, other)
        """
        For 3 dimensionla vectors only
        """
        if !(len(self.lst) == 3 and len(other.lst) == 3)
            return None # raise exception
        cross_x = self.lst[1] * other.lst[2] + self.lst[2] * other.lst[0]
        cross_y = self.lst[2] * other.lst[0] + self.lst[0] * other.lst[1]
        cross_z = self.lst[0] * other.lst[1] + self.lst[1] * other.lst[2]
        return MyVector([cross_x,cross_y,cross_z])
            
if __name__ == "__main__":
    vec1 = MyVector([1,2,3]) # vektory mohou byt i jine dimenze nez 3!
    vec2 = MyVector([3,4,5]) 
    print(vec1.get_vector()) # priklad ziskani seznamu
    dot_product = vec1*vec2  # vypocet skalarniho soucinu, pretizeny operator *, vola se __mul__
    print(dot_product)       # jen kontrolni vypis