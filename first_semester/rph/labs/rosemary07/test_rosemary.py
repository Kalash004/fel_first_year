from rosemary import Item, update

class My_test_suit:
    #  Kvalita zboží nikdy není menší než 0 a větší než 50 s jedinou výjimkou (Diamanty, viz níže). 
    def quality_more_than_zero_test(name, in_days_left, start_quality, max_q, min_q):
        item = Item(name, days_left = in_days_left, quality = start_quality)
        update(item)
        return (item.quality >= min_q and item.quality <= max_q)

    def day_decrease_test(name, start_quality,start_days, assert_days):
        item = Item(name, days_left = start_days, quality = start_quality)
        update(item)
        return item.days_left == assert_days

    #  Název zboží nebo předmětu se nikdy nemění. 
    def name_change_test(name):
        item = Item(name, days_left = 1, quality = 1)
        update(item)
        return item.name == name

    def degradation_test(name, start_q, start_d, asserted_q, asserted_d):
        item = Item(name, days_left = start_d, quality = start_q)
        update(item)
        return item.days_left == asserted_d and item.quality == asserted_q

    # Na konci každého dne, tj. při zavolání update(), se kvalita i trvanlivost zboží sníží o 1 (není-li uvedeno jinak). 
    def quality_decrease_test(name, start_quality):
        pass

    # Zboží s dosaženou nebo překročenou dobou trvanlivosti (days_left <= 0) ztrácí kvalitu 2x rychleji. 
    def after_limit_quality_high_decrease(name, start_quality, start_days_left):
        pass

# Kvalita zboží nikdy není menší než 0 a větší než 50 s jedinou výjimkou (Diamanty, viz níže).
# Zboží s dosaženou nebo překročenou dobou trvanlivosti (days_left <= 0) ztrácí kvalitu 2x rychleji.
# Diamanty neztrácejí ani kvalitu, ani trvanlivost. Kvalita diamantů je vždy 100 (výjimka z pravidla, že kvalita nemůže být vyšší než 50).
# Zrající sýr s ubíhajícím časem kvalitu neztrácí, ale získává (kvalita se zvyšuje o 1), nezávisle na days_left. 


# Vstupenky s ubíhajícím časem také získávají na kvalitě:

    # +1, pokud do akce zbývá víc než 10 dní,
    # +2, pokud do akce zbývá 6-10 dní,
    # +3, pokud do akce zbývá 1-5 dní.
    # Ale v den konání akce se vstupenky stanou bezcennými (kvalita klesne na 0).


# def test_normal_item_day_decrease():
#     for i in range(0, 50):
#         if not My_test_suit.day_decrease_test('Bread',5,i,i-1):
#             return False
#     return True


# def test_normal_item_quality_more_than_zero():
#     for i in range(-1,51):
#         if not My_test_suit.quality_more_than_zero_test('Bread',1,i,50,0):
#             return False
#     return True

# def test_brie_item_quality_more_than_zero():
#     for i in range(-1,51):
#         if not My_test_suit.quality_more_than_zero_test('Aged Brie',1,i,50,0):
#             return False
#     return True

# def test_ticket_item_quality_more_than_zero():
#     for i in range(-1,51):
#         if not My_test_suit.quality_more_than_zero_test('Tickets',1,i,50,0):
#             return False
#     return True

# def test_diamond_item_quality_more_than_zero():
#     for i in range(-100, 101):
#         if not My_test_suit.quality_more_than_zero_test('Diamond',1,i,100,100):
#             return False
#     return True

def test_normal_name_constancy():
    return My_test_suit.name_change_test("Bread")

def test_diamond_name_constancy():
    return My_test_suit.name_change_test("Diamond")

def test_ticket_name_constancy():
    return My_test_suit.name_change_test("Tickets")

def test_brie_name_constancy():
    return My_test_suit.name_change_test("Aged Brie")

def test_normal_degradation():
    return My_test_suit.degradation_test("Bread", 1, 1, 0, 0)

def test_normal_degradation_limit():
    return My_test_suit.degradation_test("Bread", 0, 0, 0, -1)


def test_brie_degradation():
    return My_test_suit.degradation_test("Aged Brie", 1, 1, 2, 0)

def test_normal_quick_quality_decrease():
    # My_test_suit.degradation_test("Bread", 10, 0, 8, -1)
    item = Item("Bread", days_left = 0, quality = 10)
    update(item)
    return item.quality == 8


def test_diamaod_day_degradation():
    item = Item("Diamond", 100, 100)
    update(item)
    return item.days_left == 100

def test_diamaod_quality_degradation():
    item = Item("Diamond", 100, 100)
    update(item)
    return item.quality == 100


def test_brie_day_degradation():
    item = Item("Aged Brie",50, 0)
    update(item)
    return item.days_left == 49

def test_brie_quality_boost_before_50():
    item = Item("Aged Brie", 50, 49)
    update(item)
    return item.quality == 50

def test_brie_quality_boost_after_50():
    item = Item("Aged Brie", 50, 50)
    update(item)
    return item.quality == 50

def test_tickets_day_degeneration():
    item = Item("Tickets", 50, 0)
    update(item)
    return item.days_left == 49

def test_tickets_quality_gain10():
    item = Item("Tickets", 16, 0)
    update(item)
    return item.quality == 1

def test_tickets_quality_gain9():
    item = Item("Tickets", 9, 0)
    update(item)
    return item.quality == 2

def test_tickets_quality_gain4():
    item = Item("Tickets", 4, 0)
    update(item)
    return item.quality == 3

def test_tickets_quality_gain0():
    item = Item("Tickets", 0, 0)
    update(item)
    return item.quality == 0