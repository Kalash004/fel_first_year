import random

def generate_doors():
    rand_num = random.randint(1,3)
    doors = {
        1:"empty",
        2:"empty",
        3:"empty"
    }
    doors[rand_num] = "auto"
    return doors

def remove_empty(doors, chosen):
    for k,v in doors.items():
        if k == chosen:
            continue
        if v == "empty":
            doors.pop(k)
            break

def stay(doors):
    doors = doors.copy()
    chosen = random.randint(1,3)
    remove_empty(doors, chosen)
    if doors[chosen] == "auto":
        return True
    return False    

def change(doors):
    doors = doors.copy()
    chosen = random.randint(1,3)
    remove_empty(doors, chosen)
    not_removed_keys = list(doors.keys())
    not_removed_keys.pop(not_removed_keys.index(chosen))
    new_chosen = not_removed_keys[0]
    if doors[new_chosen] == "auto":
        return True
    return False

if __name__ == "__main__":
    stay_wins = 0
    change_wins = 0
    for i in range(100):
        doors = generate_doors()
        if change(doors):
            change_wins += 1
        if stay(doors):
            stay_wins += 1
    print(f"Stay win rate = {stay_wins}/100")
    print(f"Change win rate = {change_wins}/100")