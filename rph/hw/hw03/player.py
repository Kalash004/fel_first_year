class MyPlayer:
    """_summary_ Player plays moves that give the most points.
    """   
    ###
    # My player has 3 types of tactics:
    # 1) count tactic : counts highest possible gain from both Defect and Cooperate  func name [_set_counting_tactic]
    # 2) play against dumb player : this partially counters my own tactic - after 2 rounds player checks if last 2 moves are the same and if so finds a better play depending on payoff matrix
    # 3) random tactic : i found that my player is bad in scenarios when DD is bigger than CC so this tries to counter it somehow
    ###
    
    # My player is bad when the payoff matrix DD has biggest numbers example:
        # (((4,4),(10,6)),((6,10),(20,20)))
        #    CC     CD      DC      DD
        # I use random tactic at this point
        
    # My player is bad if some of the elements have big differences. I dont have time to fix it now so it is what it is 
    #         matrix =  ( ((5,5),(1,70)) , ((70,1),(200,200)) )
        
    PLAYER = 0
    OPPONENT = 1
    COOPERATE = 0
    DEFECT = 1
    
    def __init__(self, pay_off_matrix, round_count=None): # type: ignore
        self.round_count = round_count
        self.last_round_num = 0
        self.pay_off_matrix = pay_off_matrix
        self.move_history: [()] = [] # type: ignore
        self.play_this_round = None
        self.smart_opponent = True
        self.local_history = []
        self.playing_start_tactic = True
            
            
    def select_move(self) -> bool:
        try:
            if self.last_round_num == 0:
                self._set_play_start_tactic()
                self._increment_round_by_one()
                return self.play_this_round
            
            if self.last_round_num >= 2:
                self._set_is_opponent_smart()
                
            if not self.smart_opponent and self.last_round_num >= 2:
                self._set_playing_start_tactic(False)
                self._set_play_best_move_based_on_stupid_enemy()
            elif not self.playing_start_tactic:
                self._set_playing_start_tactic(True)
                self._set_play_start_tactic()
        
            self._record_local_history()
            self._increment_round_by_one()
        except:
            return False # incase something really bad happens ;-;
        return self.play_this_round 
    
    def _set_playing_start_tactic(self, status):
        self.playing_start_tactic = status
    
    def record_last_moves(self, my_last_move, opponent_last_moves):
        my_l_move = 1 if my_last_move else 0
        opp_move = 1 if opponent_last_moves else 0
        my_l_move = None if my_last_move is None else my_l_move # If None is recorded
        opp_move = None if my_last_move is None else opp_move # 
        if self.last_round_num != 1 and self.local_history[self.last_round_num-2] != my_l_move: ## corruption check
            self.move_history.append((None,None)) ## corrupted data 
        self.move_history.append((my_l_move, opp_move))    

    def _set_play_start_tactic(self) -> bool:
        if self.pay_off_matrix[self.DEFECT][self.DEFECT][self.PLAYER] > self.pay_off_matrix[self.COOPERATE][self.COOPERATE][self.PLAYER]:
            self._set_random_tactic()
            return
        self._set_counting_tactic()

    # TODO: If we see that opponent always chooses defect/cooperate - find a better move
    # TODO: If we see that opponent uses same tactic as me - find a better move
    def _are_opponents_moves_similar(self):
        pre_last_round = self._get_enemy_move_history(self.last_round_num - 1)
        last_round = self._get_enemy_move_history(self.last_round_num)
        if None in (pre_last_round, last_round):
            return True
        return not pre_last_round == last_round
        
    def _set_is_opponent_smart(self):
        self.smart_opponent = self._are_opponents_moves_similar()
        
    def _increment_round_by_one(self):
        self.last_round_num += 1
    
    def _get_history(self, round):
        round -= 1 # converting round format to array format
        if self.move_history[round][0] == None or self.move_history[round][1] == None:
            return None
        return self.move_history[round]
        
    def _set_play_best_move_based_on_stupid_enemy(self):
        enemy_move = self._get_enemy_move_history(self.last_round_num)
        defect_points = self.pay_off_matrix[self.DEFECT][enemy_move][self.PLAYER]
        cooperate_points = self.pay_off_matrix[self.COOPERATE][enemy_move][self.PLAYER]
        self.play_this_round =  False if cooperate_points > defect_points else True
    
    def _set_random_tactic(self):
        import random
        choice = (True, False)
        self.play_this_round = random.choice(choice)
    
    def _set_counting_tactic(self):
        defect_max = self.pay_off_matrix[self.DEFECT][self.DEFECT][self.PLAYER] + self.pay_off_matrix[self.DEFECT][self.COOPERATE][self.PLAYER]
        cooperate_max = self.pay_off_matrix[self.COOPERATE][self.COOPERATE][self.PLAYER] + self.pay_off_matrix[self.COOPERATE][self.DEFECT][self.PLAYER]
        if defect_max > cooperate_max:
            self.play_this_round = True # Defect
        if cooperate_max > defect_max:
            self.play_this_round = False # Cooperate
        if cooperate_max == defect_max: # Default
            self.play_this_round = False
    
    def _get_enemy_move_history(self,round):
        ret = self._get_history(round)
        if ret is None:
            return None
        return ret[self.OPPONENT]
    
    def _record_local_history(self):
        my_move = 1 if self.play_this_round else 0
        self.local_history.append(my_move)
    
    
class MyStupidPlayer:
    def __init__(self, x ,y):
        pass
    
    def select_move(self):
        return True
    
    def record_last_moves(self, one, two):
        pass
    
    
class MyRandomPlayer:
    def __init__(self, x ,y):
        pass
    
    def select_move(self):
        import random
        choice = (True, False)
        return random.choice(choice)
    
    def record_last_moves(self, one, two):
        pass
    
    
if __name__ == "__main__":
        matrix =  ( ((5,5),(1,70)) , ((70,1),(200,200)) )
        
        round_count = 100
        
        player1 = MyPlayer(matrix, round_count)
        player2 = MyRandomPlayer(matrix, round_count)
        
        points_p1 = 0
        points_p2 = 0
        
        for i in range(0, round_count):
            p1_move = player1.select_move()
            p2_move = player2.select_move()
            points_p1 += matrix[p1_move][p2_move][0]
            points_p2 += matrix[p1_move][p2_move][1]
            print(f"Player 1: {p1_move} points: {points_p1} X Player 2: {p2_move} points: {points_p2}")
            player1.record_last_moves(p1_move, False)
            player2.record_last_moves(None, None)
        pass    
        