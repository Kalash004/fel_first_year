
class MyPlayer:
    """_summary_ Player plays moves that give the most points.
    """    
    def __init__(self, pay_off_matrix, round_count=None): # type: ignore
        self.PLAYER = 0
        self.COOPERATE = 0
        self.DEFECT = 1
        self.round_count = round_count
        self.last_round_num = 0
        self.pay_off_matrix = pay_off_matrix
        self.move_history: [()] = [] # type: ignore
        self.play_this_round = None
        self.smart_opponent = True
        self.local_history = []
            
            
    def select_move(self) -> bool:
        if self.last_round_num == 0:
            self.start_tactic()
            self.increment_round_by_one()
            return self.play_this_round
        
        if self.last_round_num > 2:
            self.smart_opponent = self.are_opponents_moves_similar()
            
        if not self.smart_opponent:
                self.find_best_move_based_on_stupid_enemy()
    
        self.increment_round_by_one()
        self.save_local_history()
        return self.play_this_round 
    
    def record_last_moves(self, my_last_move, opponent_last_moves):
        my_l_move = 1 if my_last_move else 0
        opp_move = 1 if opponent_last_moves else 0
        if self.local_history[self.last_round_num] != my_l_move: ## corruption check
            self.move_history.append((None,None)) ## corrupted data 
        self.move_history.append((my_l_move, opp_move))    

    def start_tactic(self) -> bool:
        defect_max = self.pay_off_matrix[self.DEFECT][self.DEFECT][self.PLAYER] + self.pay_off_matrix[self.DEFECT][self.COOPERATE][self.PLAYER]
        cooperate_max = self.pay_off_matrix[self.COOPERATE][self.COOPERATE][self.PLAYER] + self.pay_off_matrix[self.COOPERATE][self.DEFECT][self.PLAYER]
        if defect_max > cooperate_max:
            self.play_this_round = True # Defect
        if cooperate_max > defect_max:
            self.play_this_round = False # Cooperate
        if cooperate_max == defect_max: # Default
            self.play_this_round = False
        
    # TODO: If we see that opponent always chooses defect/cooperate - find a better move
    # TODO: If we see that opponent uses same tactic as me - find a better move
    def are_opponents_moves_similar(self):
        pre_last_round = self.get_history(self.last_round_num - 1)
        last_round = self.get_history(self.last_round_num)
        if None in (pre_last_round, last_round):
            return None
        return self.get_history(self.last_round_num - 2) == self.get_history(self.last_round_num - 1)
        
    def increment_round_by_one(self):
        self.last_round_num += 1
    
    def get_history(self, round):
        if self.move_history[round][0] == None or self.move_history[round][1]:
            return None
        return self.move_history[round]
        
    def find_best_move_based_on_stupid_enemy(self):
        enemy_move = self.get_history(self.last_round_num)
        defect = self.pay_off_matrix[self.DEFECT][enemy_move]
    
    def save_local_history(self):
        self.local_history.append(self.play_this_round)
    
if __name__ == "__main__":
        matrix =  (((4,4),(1,6)),((6,1),(2,2)))
        mp = MyPlayer(matrix)
        print(mp.select_move())