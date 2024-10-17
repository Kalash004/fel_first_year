
class MyPlayer:
    """_summary_ Player plays moves that give the most points.
    """    
    def __init__(self, pay_off_matrix, iteration_count=None): # type: ignore
        self.PLAYER = 0
        self.COOPERATE = 0
        self.DEFECT = 1
        self.iter_count = iteration_count
        self.pay_off_matrix = pay_off_matrix
        self.move_history: [()] = [] # type: ignore
            
            
    def select_move(self) -> bool:
        
        return self.count_best_move() 
    
    def record_last_moves(self, input1, input2):
        self.move_history.append((input1, input2))    

    def count_best_move(self) -> bool:
        defect_max = self.pay_off_matrix[self.DEFECT][self.DEFECT][self.PLAYER] + self.pay_off_matrix[self.DEFECT][self.COOPERATE][self.PLAYER]
        cooperate_max = self.pay_off_matrix[self.COOPERATE][self.COOPERATE][self.PLAYER] + self.pay_off_matrix[self.COOPERATE][self.DEFECT][self.PLAYER]
        if defect_max > cooperate_max:
            return True # Defect
        if cooperate_max > defect_max:
            return False # Cooperate
        if cooperate_max == defect_max:
            # TODO choose between defect or cooperate as default
            return False
        
    # TODO: If we see that opponent always chooses defect/cooperate - find a better move
    # TODO: If we see that opponent uses same tactic as me - find a better move
     
if __name__ == "__main__":
        matrix =  (((4,4),(1,6)),((6,1),(2,2)))
        mp = MyPlayer(matrix)
        print(mp.select_move())