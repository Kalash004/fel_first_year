class GameState:
    EMPTY = -1
    
    def __init__(self, board, my_color, opponent_color):
        self.player_color = my_color
        self.opponent_color = opponent_color
        self.board = board
        self.my_locations = self.get_locations_of_color(my_color)
        self.playable_cells = self._find_playable_location()
    
    def apply_move(self, move, color):
        """
        Apply a move to the board and return a new GameState.
        """
        row, col = move
        new_board = [r.copy() for r in self.board]
        new_board[row][col] = color
        flipped = self.__get_flipped_pieces(move, color, new_board)
        for r, c in flipped:
            new_board[r][c] = color
        return GameState(new_board, self.player_color, self.opponent_color)

    def __get_flipped_pieces(self, move, color, board):
        """
        Get all pieces that would be flipped by making a move.
        """
        flipped = []
        directions = [ (0,-1), (0,1), (-1,0), (1,0),
                      (-1,1), (-1,-1), (1,1), (1,-1) ]
        for d in directions:
            pieces_to_flip = []
            row, col = move
            dr, dc = d
            row += dr
            col += dc
            while 0 <= row < len(board) and 0 <= col < len(board[0]):
                if board[row][col] == self.opponent_color:
                    pieces_to_flip.append((row, col))
                elif board[row][col] == color:
                    flipped.extend(pieces_to_flip)
                    break
                else:
                    break
                row += dr
                col += dc
        return flipped

    def generate_new_board(self, board, move, player_color, opponent_color):
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1),  # Vertical and horizontal
                    (-1, -1), (-1, 1), (1, -1), (1, 1)]  # Diagonal

        # Place the player's piece
        new_board = [row[:] for row in board]  # Deep copy the board
        move_row, move_col = move
        new_board[move_row][move_col] = player_color

        # Flip opponent's pieces in all directions
        for direction in directions:
            self.flip_pieces(move_row, move_col, direction)

        return new_board
    
    def is_valid_position(self, row, col):
        return 0 <= row < len(self.board) and 0 <= col < len(self.board[0])    
    
    def flip_pieces(self, row, col, direction, board):
        """
        Flips the opponent's pieces in a given direction if valid.
        """
        r, c = row + direction[0], col + direction[1]
        cells_to_flip = []
        while self.is_valid_position(r, c, board) and board[r][c] == self.opponent_color:
            cells_to_flip.append((r, c))
            r += direction[0]
            c += direction[1]
        if self.is_valid_position(r, c, board) and board[r][c] == self.player_color:
            for flip_row, flip_col in cells_to_flip:
                board[flip_row][flip_col] = self.player_color
  
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
        all_playable_cells = [
            self._find_playable_cells_for_this_cell(cell[0], cell[1])
            for cell in self.my_locations
        ]
        return [cell for sublist in all_playable_cells if sublist for cell in sublist]
    
    
    def _find_playable_cells_for_this_cell(self,start_row, start_col):
        playable_cells_for_n_cell = [
            self._marsh(start_row, start_col, (0,-1), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (0,1), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (-1,0), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (1,0), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (-1,1), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (-1,-1), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (1,1), self.board, self.EMPTY, self.opponent_color),
            self._marsh(start_row, start_col, (1,-1), self.board, self.EMPTY, self.opponent_color)        
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
        while (col + step[1] <= max_width and row + step[0] <= max_height and col + step[1] >= 0 and row + step[0] >= 0):
            col += step[1]
            row += step[0]
            test_subject = data[row][col]
            if (test_subject == between):
                between_count += 1
                continue
            if (test_subject == looking_for and between_count > 0):
                return (row, col)
            return None
        return None
    
    def count_empty_places(self):
        empty_count = 0
        for row in self.board:
            empty_count += row.count(self.EMPTY)
        return empty_count
        
    def count_flips_in_direction(self, start_row, start_col, direction):
        row, col = start_row + direction[0], start_col + direction[1]
        line_flips = 0
        while self.is_valid_position(row, col) and self.board[row][col] == self.opponent_color:
            line_flips += 1
            row += direction[0]
            col += direction[1]
        if self.is_valid_position(row, col) and self.board[row][col] == self.player_color:
            return line_flips
        return 0
                
    def count_flips(self, move):
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1),  # Horizontal and vertical
                    (-1, -1), (-1, 1), (1, -1), (1, 1)]  # Diagonal
        flips = 0
        for direction in directions:
            flips += self.count_flips_in_direction(move[0], move[1], direction)
        return flips
            
class MyPlayer:
    """
        Player plays, game games
    """
    POSITION_WEIGHTS = [
    [100, -50, 10, 10, 10, 10, -50, 100],
    [-50, -50,  1,  1,  1,  1, -50, -50],
    [ 10,   1,  5,  5,  5,  5,   1,  10],
    [ 10,   1,  5,  0,  0,  5,   1,  10],
    [ 10,   1,  5,  0,  0,  5,   1,  10],
    [ 10,   1,  5,  5,  5,  5,   1,  10],
    [-50, -50,  1,  1,  1,  1, -50, -50],
    [100, -50, 10, 10, 10, 10, -50, 100],
    ]
    
    def __init__(self, my_color, opponent_color):
        self.my_color = my_color
        self.opponent_color = opponent_color
        self.history = []
        self.game_state = None
        self.selected_tactic = self.__TACTIC_basic_best_weight_play
        
    def __evaluate_move_weight(self, move):
        row, col = move
        return self.POSITION_WEIGHTS[row][col]


    def __filter_moves(self):
        pass
      
    
    def __find_best_moves(self, moves, count):
        if not moves or count <= 0:
            return []
        evaluated_moves = [(move, self.__evaluate_move_weight(move)) for move in moves]
        evaluated_moves.sort(key=lambda item: item[1], reverse=True)
        best_moves = [move for move, score in evaluated_moves[:count]]
        return best_moves

      
    def __TACTIC_basic_best_weight_play(self):
        """
        - Find best weighted move
        - Play it
        - If less than 30 empty spaces change to greedy  
        """ 
        empty_space_count = self.game_state.count_empty_places()
        if (empty_space_count > 30):
            best_moves = self.__find_best_moves(self.game_state.playable_cells,30)
            if not best_moves:
                return None
            return best_moves[0] # return best evaluated move
        else:
            return self.__TACTIC_greedy()
        
    def __TACTIC_greedy(self):
        if not self.game_state.playable_cells:
            return None

        # Find the move with the maximum flips
        best_move = max(self.game_state.playable_cells, key=lambda move: self.game_state.count_flips(move))
        return best_move
            
    def __TACTIC_exception_happened(self):
        """
        find ANY possible move
        """  
        if not self.game_state.playable_cells[0]:
            return None
        return self.game_state.playable_cells[0]
      
      
    def select_move(self, board): #-> tuple('int row','int column') 
        try:
            self.game_state = GameState(board, self.my_color, self.opponent_color)
            if not self.game_state.playable_cells: # Deal with no playable cells 
                return None
            
            return self.selected_tactic()
            
        except Exception as e: 
           return self.__TACTIC_exception_happened()
   
        
if __name__ == "__main__":
    board = [
    [-1, 1, 1, 1, 1, -1, -1, 0],
    [-1, 0, -1, 1, 0, 1, 0, -1],
    [-1, -1, 1, -1, 0, 0, 1, 1],
    [-1, 1, 1, 1, 0, 0, 1, 0],
    [1, 1, 1, 0, 0, 0, 0, 0],
    [-1, 1, 0, 1, 1, 0, 0, -1],
    [-1, 0, 1, 1, -1, 0, -1, 0],
    [-1, -1, -1, -1, -1, 0, -1, -1]
    ]
    
    player = MyPlayer(my_color=0, opponent_color=1)
    print(player.select_move(board=board))