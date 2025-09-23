import random


class MyPlayer:
    def __init__(self):
        pass

    def play(self):
        return random.choice(["R","P","S"])


class Game:
    def __init__(self, player1: MyPlayer, player2: MyPlayer, iterations_count):
        self.player1 = player1
        self.player2 = player2
        self.iter_count = iterations_count
        self.played_rounds: [()] = [] # [(who won, player 1 choice, player 2 choice)]
        
    def play(self):
        for i in range(1,self.iter_count+1):
            choice_p1 = self.player1.play()
            choice_p2 = self.player2.play()
            plays = (choice_p1, choice_p2)
            winners_num = self.find_win_choice(plays)
            self.played_rounds.append((winners_num, choice_p1, choice_p2))
            # print(f"Player {winners_num} won game: {i}")             
        
        
    def find_win_choice(self,plays: tuple) -> int:
        """
            returns: int
                0 if draw
                1 if player 1 wins
                2 if player 2 wins
        """
        if plays[0] == plays[1]:
            return 0
        if plays in (("R","S"),("P","R"),("S","P")):
            return 1
        return 2


    def find_game_winner(self):
        p1_score = 0
        p2_score = 0
        for round in self.played_rounds:
            if round[0] == 0:
                continue
            if round[0] == 1:
                p1_score += 1
                continue
            p2_score += 1
            continue
        return (1,p1_score) if p1_score > p2_score else (2,p2_score)

    def __str__(self):
        repr = ""
        for tupl in self.played_rounds:
            repr += f"Winner: {tupl[0]}, P1: {tupl[1]}, P2: {tupl[2]}\n"
        return repr

if __name__ == "__main__":
    p1 = MyPlayer()
    p2 = MyPlayer()
    game = Game(p1, p2, 10)
    game.play()
    print(game)
    print(game.find_game_winner())