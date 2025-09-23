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
    
    
    def cross_product(self, other):
        """
        For 3 dimensionla vectors only
        """
        if not (len(self.lst) == 3 and len(other.lst) == 3):
            return None # raise exception
        cross_x = self.lst[1] * other.lst[2] + self.lst[2] * other.lst[0]
        cross_y = self.lst[2] * other.lst[0] + self.lst[0] * other.lst[1]
        cross_z = self.lst[0] * other.lst[1] + self.lst[1] * other.lst[2]
        return MyVector([cross_x,cross_y,cross_z])
    
    def __add__(self, other):
        if (len(self.lst) != len(other.lst)):
            return None # raise exception
        new_lst = []
        for i in range(0, len(self.lst)):
            new_lst.append(self.lst[i] + other.lst[i])
        return MyVector(new_lst)

    def norm(self):
        import math
        norm = 0
        for v in self.lst:
            norm += v * v
        norm = math.sqrt(norm) 
        return norm   
            
if __name__ == "__main__":
    vec1 = MyVector([1,2,3]) # vektory mohou byt i jine dimenze nez 3!
    vec2 = MyVector([3,4,5]) 
    vec3 = vec1 + vec2
    vec4 = MyVector([3,4])
    print(vec3.get_vector())
    print(vec4.norm())