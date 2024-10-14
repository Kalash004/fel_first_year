class MyPlayer:
    """_summary_ Player plays move that give the most points meaning if in matrix of pay-off 
        C gives more points than D, player plays C and vica-versa.
    """    
    def __init__(self, pay_off_matrix:[[],[]], iteration_count=None):
        self.PLAYER = 0
        self.COOPERATE = 0
        self.DEFECT = 1
        
        if iteration_count is not None:
            self.iter_count = iteration_count
        else:
            self.iter_count = iteration_count
        self.pay_off_matrix = pay_off_matrix
        self.move_history: [()] = []
            
            
    def select_move(self) -> bool:
        return self.count_best_move()
    
    def record_last_moves(self, input1, input2):
        self.move_history.append((input1, input2))    

    def count_best_move(self):
        defect_max = self.pay_off_matrix[self.DEFECT][self.DEFECT][self.PLAYER] + self.pay_off_matrix[self.DEFECT][self.COOPERATE][self.PLAYER]
        cooperate_max = self.pay_off_matrix[self.COOPERATE][self.DEFECT][self.PLAYER] + self.pay_off_matrix[self.DEFECT][self.COOPERATE][self.PLAYER]
        if defect_max > cooperate_max:
            return True
        if cooperate_max > defect_max:
            return False
        if cooperate_max == defect_max:
            # TODO 
        
        
if __name__ == "__main__":
    pass