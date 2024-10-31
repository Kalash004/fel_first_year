class GameState:
    EMPTY = -1
    
    def __init__(self, board, my_color, opponent_color):
        self.my_color = my_color
        self.enemy_color = opponent_color
        self.board = board
        self.my_locations = self.get_locations_of_color(my_color)
        self.playable_cells = self._find_playable_location()
        
    def get_locations_of_color(self, color):
        to_return = []
        rows_count = len(self.board)
        cols_count = len(self.board[0])
        for row in range(0,rows_count):
            for col in range(0, cols_count):
                if (color == self.board[row][col]):
                    to_return.append((row, col))
        return to_return
    
    def _find_playable_location(self):
        all_playable_cells = []
        for my_cell in self.my_locations:
            all_playable_cells.append(self._find_playable_cells_for_this_cell(my_cell[0], my_cell[1]))
        all_playable_cells = [x for x in all_playable_cells if x is not None]
        return all_playable_cells
    
    def _find_playable_cells_for_this_cell(self,start_row, start_col):
        playable_cells_for_n_cell = [
            self._marsh(start_row, start_col, (0,-1), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (0,1), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (-1,0), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (1,0), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (-1,1), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (-1,-1), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (1,1), self.board, self.EMPTY, self.enemy_color),
            self._marsh(start_row, start_col, (1,-1), self.board, self.EMPTY, self.enemy_color)        
        ]
        playable_cells_for_n_cell = [x for x in playable_cells_for_n_cell if x is not None] # remove Nones
        if len(playable_cells_for_n_cell) == 0:
            return None
        return playable_cells_for_n_cell
    
    def _marsh(self, start_row, start_col, step, data, looking_for, between):
        col, row = start_col, start_row
        max_height = len(data)- 1
        max_width = len(data[start_row]) - 1
        between_count = 0
        while (col < max_width and row < max_height and col >= 0 and row >= 0):
            col += step[1]
            row += step[0]
            test_subject = data[row][col]
            if (test_subject == between):
                between_count += 1
                continue
            if (test_subject == looking_for and between_count > 0):
                return (row, col)
            return None
            
    
        
                
            
class MyPlayer:
    """
        Player plays, game games
    """
    def __init__(self, my_color, opponent_color):
        self.my_color = my_color
        self.opponent_color = opponent_color
        self.history = []
        
    def select_move(self, board): #-> tuple('int row','int column') 
        try:
            game_state = GameState(board, self.my_color, self.opponent_color)
            cell = game_state.playable_cells[0][0]
            x = cell[0]
            y = cell[1]
            return (x, y)            
        except Exception as e: 
            # TODO
            # print(e)
            raise e
            return 0
        
        
if __name__ == "__main__":
    
    game = [
        [-1, -1, -1, -1,-1, -1, -1, -1],
        [-1, -1, -1, -1,-1, -1, -1, -1],
        [-1, -1, -1, -1,-1, -1, -1, -1],
        [-1, -1, -1, 0,1, -1, -1, -1],
        [-1, -1, -1, 1,0, -1, -1, -1],
        [-1, -1, -1, -1,-1, -1, -1, -1],
        [-1, -1, -1, -1,-1, -1, -1, -1],
        [-1, -1, -1, -1,-1, -1, -1, -1],
    ]
    game2 = [
        [-1,-1,-1,-1, 1,-1,-1, 0],
	    [-1,-1,-1, 1, 1, 1, 0,-1],
	    [-1,-1,	1,-1, 1, 0, 1,-1],
	    [-1,-1,-1, 1, 0, 1, 0, 0],
	    [-1,-1, 1, 0, 1, 0, 0, 0],
	    [-1, 1, 0, 0, 0, 0, 0,-1],
	    [-1,-1,-1, 1,-1,-1,-1, 0],
	    [-1,-1,-1,-1,-1,-1,-1,-1]
    ]
    game3 = [
        [ 0, 0,	0, 0, 1, 1,	1, 0],
        [ 0, 0,	0, 1, 1, 0,	0, 0],
        [ 0, 1,	0, 0, 1, 1,	0, 0],
        [ 0, 1,	1, 0, 1, 1,	0, 0],
        [ 0, 1,	1, 1, 0, 1,	0, 1],
        [ 0, 1,	1, 1, 1, 1,	1, 1],
        [ 0, 1,	1, 1, 1, 1, 0,-1],
        [ 0, 0,	0, 0, 0, 0,	0,-1],
    ]
    # problem 7:5   
    player = MyPlayer(0,1)
    move = player.select_move(game3)
    print(move)
    