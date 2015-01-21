#include <stdio.h>
#include <stdlib.h>

#define MAX(a,b) (((a)>(b))?(a):(b))

/* for card encoding */
char *suits = "CDHS";
char *values = "23456789TJQKA";
int value_map[100] = {0,};

int rank_card(char suit, char value) { //ex : 2C -> 0, 2D -> 1, 2H -> 2, 2S -> 3, 3C -> 4
  int i,j;

  for (i=0; i<4; i++)
    if (suits[i] == suit) break;
  for (j=0; j<13; j++)
    if (values[j] == value) break;

  return j*4 + i;
}

char suit(int card) {
  return suits[card%4];
}

int value(int card) {
  return value_map[values[card/4]];
}

#define BASE_ONE_PAIR (13*13*13*13*13)
#define BASE_TWO_PAIR (BASE_ONE_PAIR + (13*13*13*13))
#define BASE_THREE_CARD (BASE_TWO_PAIR + (13*13))
#define BASE_STRAIGHT (BASE_THREE_CARD + 13)
#define BASE_FLUSH (BASE_STRAIGHT + 13)
#define BASE_FULL_HOUSE (BASE_FLUSH + (13*13*13*13*13))
#define BASE_FOUR_CARD (BASE_FULL_HOUSE + 13)
#define BASE_STRAIGHT_FLUSH (BASE_FOUR_CARD + 13)

int is_straight_flush(int cards[]) {
  int i; //loop var
  int v0,s0; //value and suit of previous card
  int v1,s1; //value and suit of current card

  v0 = value(cards[0]);
  s0 = suit(cards[0]);

  for (i=1; i<5; i++) {
    v1 = value(cards[i]);
    s1 = suit(cards[i]);

    if (s0 != s1) return -1;
    if (v0 - 1 != v1) return -1;
    v0 = v1;
  }

  return value(cards[0]) + BASE_STRAIGHT_FLUSH; //return largest card value with offset
}

int is_four_card(int cards[]) {
  int i,offset;
  int v0, v1;

  //two cases. v[0]~v[3] are equal, v[1]~v[4] are equal
  for (offset=0; offset<2; offset++) {
    v0 = value(cards[offset]);

    for (i=offset+1; i<offset+4; i++) {
      v1 = value(cards[i]);
      if (v1 != v0) break;
    }
    if (i == offset+4) return v0 + BASE_FOUR_CARD;
  }
  return -1;
}

int is_full_house(int cards[]) {
  int i;
  int v0, v1;

  //two cases
  //1. v[0]~v[2] are equal, v[3]==v[4]
  //2. v[0]==v[1], v[2] ~ v[4] are equal

  // Case 1
  v0 = value(cards[0]);
  for (i=1; i<3; i++) {
    v1 = value(cards[i]);

    if (v1 != v0) break;
  }

  if (i == 3) {
    if (value(cards[3]) == value(cards[4])) 
      return v0 + BASE_FULL_HOUSE;
    else
      return -1;
  }

  // Case 2
  if (value(cards[0]) != value(cards[1])) 
    return -1;
  v0 = value(cards[2]);
  for (i=3; i<5; i++) {
    v1 = value(cards[i]);

    if (v1 != v0) return -1;
  }

  return v0 + BASE_FULL_HOUSE;
}

int is_flush(int cards[]) {
  int i;
  int s0, v0, s1;

  s0 = suit(cards[0]);

  for (i=1; i<5; i++) {
    s1 = suit(cards[i]);
    if (s0 != s1) return -1;
  }

  v0 = 0;
  for (i=0; i<5; i++) {
    v0 = v0*13 + value(cards[i]);
  }

  return BASE_FLUSH + v0;
}

int is_straight(int cards[]) {
  int i;
  int v0, v1;

  v0 = value(cards[0]);
  for (i=1; i<5; i++) {
    v1 = value(cards[i]);
    if (v0 - 1 != v1) return -1;
    v0 = v1;
  }

  return value(cards[0]) + BASE_STRAIGHT;
}

int is_three_card(int cards[]) {
  int offset;
  int i;
  int v0, v1;

  for (offset=0; offset<3; offset++) {
    v0 = value(cards[offset]);
    for (i=offset+1; i<offset+3; i++) {
      v1 = value(cards[i]);
      if (v0 != v1) break;
    }

    if (i == offset+3) return v0 + BASE_THREE_CARD;
  }

  return -1;
}

int is_two_pair(int cards[]) {
  int i; //loop var
  int v0, v1; //value of each pair
  int s; //sum of all cards value

  //select first pair
  //first pair only can be seen at the (0,1) or (1,2)
  for (i=0; i<2; i++) {
    if (value(cards[i]) == value(cards[i+1])) break;
  }
  if (i == 2) return -1; //first pair not found
  v0 = value(cards[i]);

  //select second pair
  for (i=i+2; i<4; i++) {
    if (value(cards[i]) == value(cards[i+1])) break;
  }
  if (i == 4) return -1; //second pair not found
  v1 = value(cards[i]);

  for (i=0; i<5; i++) s += value(cards[i]);

  return BASE_TWO_PAIR + MAX(v0, v1)*13 + (s - (v0+v1)*2);
}

int is_one_pair(int cards[]) {
  int i,j; //loop var
  int s; //base 13 represented value of other cards
  for (i=0; i<4; i++) {
    if (value(cards[i]) == value(cards[i+1])) {
      s = 0;
      for (j=0; j<5; j++) {
        if (j == i || j == i+1) continue;
        s = s*13 + value(cards[j]);
      }

      return BASE_ONE_PAIR + value(cards[i])*13*13*13 + s;
    }
  }
  return -1;
}

int high_card(int cards[]) {
  int s = 0;
  int i;

  for (i=0; i<5; i++) {
    /*printf("%c%c ", values[value(cards[i])], suit(cards[i]));*/
    s = s*13 + value(cards[i]);
  }
  /*printf("\n");*/

  return s;
}

/* for determining hand rank */
int rank_hand(int cards[]) {
  int rank; 
  if ((rank = is_straight_flush(cards)) >= 0) {
    /*printf("straight flush\n");*/
    return rank;
  }
  if ((rank = is_four_card(cards)) >= 0) {
    /*printf("four cards\n");*/
    return rank;
  }
  if ((rank = is_full_house(cards)) >= 0) {
    /*printf("full house\n");*/
    return rank;
  }
  if ((rank = is_flush(cards)) >= 0) {
    /*printf("flush\n");*/
    return rank;
  }
  if ((rank = is_straight(cards)) >= 0) {
    /*printf("straight\n");*/
    return rank;
  }
  if ((rank = is_three_card(cards)) >= 0) {
    /*printf("three cards\n");*/
    return rank;
  }
  if ((rank = is_two_pair(cards)) >= 0) {
    /*printf("two pairs\n");*/
    return rank;
  }
  if ((rank = is_one_pair(cards)) >= 0) {
    /*printf("one pair\n");*/
    return rank;
  }
  /*printf("high card\n");*/
  return high_card(cards);
}

int card_compare(const void *i, const void *j) {
  return value(*(int *)j) - value(*(int *)i);
}

int main() {
  char buf[BUFSIZ];
  int i,j; //loop var
  char s,v; //suit and value for card
  char *pch;
  int card;
  int hands[2][5];
  
  int rank0, rank1; //0 for black, 1 for white

  //initialize value map (char => int)
  for (i=0; i<13; i++) {
    char v = values[i];
    value_map[(int)v] = i;
  }

  /*printf("AH = %d, KD = %d\n", rank_card('H', 'A'), rank_card('D', 'K')); return 0;*/

  while(fgets(buf, BUFSIZ-1, stdin) != NULL) {
    pch = buf;
    for (i=0; i<2; i++) {
      for (j=0; j<5; j++) {
        v = *pch++;
        s = *pch++;
        card = rank_card(s, v);
        hands[i][j] = card;
        pch++; //for space char or newline
      }
    }

    //sort hands for comparing
    qsort(hands[0], 5, sizeof(int), card_compare);
    qsort(hands[1], 5, sizeof(int), card_compare);

    /*for (i=0; i<2; i++) {*/
      /*for (j=0; j<5; j++) {*/
        /*printf("%c%c ", values[value(hands[i][j])], suit(hands[i][j]));*/
      /*}*/
      /*printf("\n");*/
    /*}*/

    /*printf("Black: ");*/
    int rank0 = rank_hand(hands[0]);
    /*printf("White: ");*/
    int rank1 = rank_hand(hands[1]);

    /*printf("Black : %d, White: %d\n", rank0, rank1);*/

    //print result
    if (rank0 == rank1) {
      printf("Tie.\n");
    } else if (rank0 > rank1) {
      printf("Black wins.\n");
    } else {
      printf("White wins.\n");
    }
  }
  return 0;
}
