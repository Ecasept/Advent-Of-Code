with open("input.txt") as f:
    data = f.readlines()

class Hand:
    def __init__(self, cards: str, bid:  int):
        self.cards = cards
        self.bid = bid
    
    def get_type_value(self) -> str:
        cards = self.cards
        counts = []
        while cards:
            symbol = cards[0]
            count = cards.count(symbol)
            counts.append(count)
            cards = cards.replace(symbol, "")
        match sorted(counts):
            case [5]: return "6" # five of a kind
            case [1, 4]: return "5" # four of a kind
            case [2, 3]: return "4" # full house
            case [1, 1, 3]: return "3" # three of a kind
            case [1, 2, 2]: return "2" # two pair
            case [1, 1, 1, 2]: return "1" # one pair
            case [1, 1, 1, 1, 1]: return "0" # high card
            case x: raise Exception(f"Unknown pattern {x}")
    def __repr__(self) -> str:
        return f"{self.cards}/{self.bid}"
    
    def get_cards_value(self) -> str:
        to_num = {
            "A": "E",
            "K": "D",
            "Q": "C",
            "J": "B",
            "T": "A"}
        cards = list(self.cards)
        for i in range(len(cards)):
            if cards[i] in to_num:
                cards[i] = to_num[cards[i]]
        return "".join(cards)

    def get_sorting_value(self) -> str:
        return self.get_type_value() + self.get_cards_value()
hands: list[Hand] = []
for line in data:
    line = line.strip()
    cards, bid = line.split(" ")
    hand = Hand(cards, int(bid))
    hands.append(hand)

hands.sort(key=lambda hand: hand.get_sorting_value())


total_winnings = 0
for i in range(len(hands)):
    hand = hands[i]
    winnings = hand.bid * (i+1)
    print(hand,i+1, hand.get_sorting_value())
    total_winnings += winnings
print(total_winnings)

