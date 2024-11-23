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
            self._find_playable_cells_for_this_cell(cell[0], cell[1], self.opponent_color)
            for cell in self.my_locations
        ]
        return [cell for sublist in all_playable_cells if sublist for cell in sublist]
    
    def _find_enemy_playable_locations(self):
        all_playable_cells = [
            self._find_playable_cells_for_this_cell(cell[0], cell[1], self.player_color)
            for cell in self.my_locations
        ]
        return [cell for sublist in all_playable_cells if sublist for cell in sublist]
    
    def _find_playable_cells_for_this_cell(self,start_row, start_col, opp_color):
        playable_cells_for_n_cell = [
            self._marsh(start_row, start_col, (0,-1), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (0,1), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (-1,0), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (1,0), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (-1,1), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (-1,-1), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (1,1), self.board, self.EMPTY, opp_color),
            self._marsh(start_row, start_col, (1,-1), self.board, self.EMPTY, opp_color)        
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
    [-50, -50,  5,  5,  5,  5, -50, -50],
    [ 10,   5,  2,  2,  2,  2,   5,  10],
    [ 10,   5,  2,  0,  0,  2,   5,  10],
    [ 10,   5,  2,  0,  0,  2,   5,  10],
    [ 10,   5,  2,  2,  2,  2,   5,  10],
    [-50, -50,  5,  5,  5,  5, -50, -50],
    [100, -50, 10, 10, 10, 10, -50, 100],
    ]
    
    def __init__(self, my_color, opponent_color):
        self.player_color = my_color
        self.opponent_color = opponent_color
        self.history = []
        self.game_state = None
        self.selected_tactic = self.__TACTIC_heuristic_best_move
 
        
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


    def __TACTIC_with_opponent_disruption(self):
        """
        - Prioritize corners and stable edges.
        - Consider moves that disrupt the opponent by reducing their mobility.
        """
        if not self.game_state.playable_cells:
            return self.__TACTIC_exception_happened()

        CORNER_BONUS = 1000
        EDGE_BONUS = 100
        EDGE_PENALTY = -50
        DISRUPTION_WEIGHT = 100
        CORNERS = [(0, 0), (0, 7), (7, 0), (7, 7)]
        EDGES = [
            (0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (0, 6),  # Top edge
            (7, 1), (7, 2), (7, 3), (7, 4), (7, 5), (7, 6),  # Bottom edge
            (1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0),  # Left edge
            (1, 7), (2, 7), (3, 7), (4, 7), (5, 7), (6, 7),  # Right edge
        ]

        best_move = None
        max_score = -float('inf')

        for move in self.game_state.playable_cells:
            row, col = move

            # Start with position weight
            position_score = self.__evaluate_move_weight(move)

            # Corner priority
            if move in CORNERS:
                position_score += CORNER_BONUS

            # Edge stability
            if move in EDGES:
                if self.__is_stable_edge(row, col):
                    position_score += EDGE_BONUS  # Reward stable edges
                else:
                    position_score += EDGE_PENALTY  # Penalize vulnerable edges

            # Opponent disruption: calculate reduction in opponent mobility
            future_game_state = self.game_state.apply_move(move, self.player_color)
            opponent_moves = future_game_state._find_enemy_playable_locations()
            disruption_score = -len(opponent_moves) * DISRUPTION_WEIGHT

            # Add flips and disruption to the evaluation
            flip_score = self.game_state.count_flips(move)
            move_score = position_score + flip_score * 10 + disruption_score

            # Update the best move
            if move_score > max_score:
                max_score = move_score
                best_move = move

        return best_move if best_move else self.__TACTIC_exception_happened()

    def __TACTIC_heuristic_with_corner_and_edge_priority(self):
        """
        - Evaluate moves with corner priority and edge stability.
        - Reward stable edges and penalize moves that open edge vulnerabilities.
        """
        if not self.game_state.playable_cells:
            return self.__TACTIC_exception_happened()

        CORNER_BONUS = 1000
        EDGE_BONUS = 100
        EDGE_PENALTY = -50
        CORNERS = [(0, 0), (0, 7), (7, 0), (7, 7)]
        EDGES = [
            (0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (0, 6),  # Top edge
            (7, 1), (7, 2), (7, 3), (7, 4), (7, 5), (7, 6),  # Bottom edge
            (1, 0), (2, 0), (3, 0), (4, 0), (5, 0), (6, 0),  # Left edge
            (1, 7), (2, 7), (3, 7), (4, 7), (5, 7), (6, 7),  # Right edge
        ]

        best_move = None
        max_score = -float('inf')

        for move in self.game_state.playable_cells:
            row, col = move

            # Start with position weight
            position_score = self.__evaluate_move_weight(move)

            # Corner priority
            if move in CORNERS:
                position_score += CORNER_BONUS

            # Edge stability
            if move in EDGES:
                if self.__is_stable_edge(row, col):
                    position_score += EDGE_BONUS  # Reward stable edges
                else:
                    position_score += EDGE_PENALTY  # Penalize vulnerable edges

            # Add flips to the evaluation
            flip_score = self.game_state.count_flips(move)
            move_score = position_score + flip_score * 10

            # Update the best move
            if move_score > max_score:
                max_score = move_score
                best_move = move

        return best_move if best_move else self.__TACTIC_exception_happened()

    def __TACTIC_heuristic_with_corner_priority(self):
        if not self.game_state.playable_cells:
            return self.__TACTIC_exception_happened()

        CORNER_BONUS = 1000  # High reward for corner positions
        CORNERS = [(0, 0), (0, 7), (7, 0), (7, 7)]  # The four corner positions

        best_move = None
        max_score = -float('inf')

        for move in self.game_state.playable_cells:
            row, col = move

            # Start with the position weight
            position_score = self.__evaluate_move_weight(move)

            # Add a bonus if the move is a corner
            if move in CORNERS:
                position_score += CORNER_BONUS

            # Add a score for the number of flips
            flip_score = self.game_state.count_flips(move)

            # Total move score combines all factors
            move_score = position_score + flip_score * 10  # Adjust multiplier if needed

            # Update best move if the current move is better
            if move_score > max_score:
                max_score = move_score
                best_move = move

        return best_move if best_move else self.__TACTIC_exception_happened()

    def __TACTIC_simple_look_into_future(self):
        LOOK_AHEAD_STEPS = 0
        LOOK_WIDTH = 5
        
        return self.__future_check_handler(LOOK_WIDTH)     
            
    def __future_check_handler(self, look_width):
        best_move = None
        worst_enemy_score = 1000
        for i in range (0,look_width):
            # - find best move based on weights
            move = self.__basic_best_weight_play_handler(self.player_color, i)
            if not move:
                return None
            # - check how board changes
            possible_future_game_state = self.game_state.apply_move(move)
            # - check if best move opens a good place for enemy
            enemy_best_move = self.__basic_best_weight_play_handler(self.opponent_color, 0)
            if not enemy_best_move:
                return move # best move - makes opponent skip
            
            score = self.__evaluate_move_weight(move)
            if score < enemy_best_move:
                best_move = move
                worst_enemy_score = score        
        
        return best_move    
        # - if yes : choose next best move (find an optimal count of tests)
        # - if no: play it
 
    def __TACTIC_heuristic_best_move(self):
        """
        - Evaluate all possible moves using a heuristic that combines position weight
        and potential flips to select the optimal move.
        """
        if not self.game_state.playable_cells:
            return self.__TACTIC_exception_happened()

        best_move = None
        max_score = -float('inf')

        for move in self.game_state.playable_cells:
            # Evaluate the move
            position_score = self.__evaluate_move_weight(move)
            flip_score = self.game_state.count_flips(move)
            move_score = position_score + flip_score * 10  # Adjust weights as needed

            # Update best move if the current move is better
            if move_score > max_score:
                max_score = move_score
                best_move = move

        return best_move if best_move else self.__TACTIC_exception_happened()         
      
    def __TACTIC_basic_best_weight_play(self):
        """
        - Find best weighted move
        - Play it
        - If less than 30 empty spaces change to greedy  
        """ 
        EMPTY_SPACE_THRESHOLD = 7
        
        empty_space_count = self.game_state.count_empty_places()
        if (empty_space_count < EMPTY_SPACE_THRESHOLD):
            return self.__basic_best_weight_play_handler(self.player_color, 0)
        else:
            return self.__TACTIC_greedy()    
        
        
    def __basic_best_weight_play_handler(self, player_color, best_move_id): 
            best_moves = self.__find_best_moves(self.game_state.playable_cells,30)
            if not best_moves:
                return None
            return best_moves[best_move_id] # return best evaluated move
        
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
            self.game_state = GameState(board, self.player_color, self.opponent_color)
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