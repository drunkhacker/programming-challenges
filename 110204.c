#include <stdio.h>
#include <string.h>

#define MAX_N 1000
#define MAX_LEN 16

char len_dict[MAX_LEN+1][MAX_N][MAX_LEN+1] = {0,};
int len_dict_count[MAX_LEN+1] = {0,};

int find_words(char crypted_words[45][MAX_LEN+1], int word_index, int word_count, char code_map[]);
int update_code_map(char code_map[], char *crypted, char *original, int len);
void revert_code_map(char code_map[], char *crypted, int len);

int main() {
  int n; //number of words
  int i,j;
  char word[MAX_LEN+1];
  int len; //word length

  char buf[BUFSIZ]; //sentence buffer
  char crypted_words[45][MAX_LEN+1] = {0,}; //words buffer for crypted sentence
  char code_map[122+1] = {0,}; //z is asciicode 122
  char *pch;

  //input dictionary
  scanf("%d",&n);
  for (i=0; i<n; i++) {
    scanf("%s", word);
    len = strlen(word);
    strcpy(len_dict[len][len_dict_count[len]++], word);
  }
  fgets(buf, BUFSIZ-1, stdin); //dummy input for newline

  while (fgets(buf, BUFSIZ-1, stdin) != NULL) {
    buf[strlen(buf)-1] = 0; //remove newline

    //init.
    for (i='a'; i<='z'; i++) {
      code_map[i] = 0;
    }

    //get crypted words
    i = 0; //i is the number of words in crypted sentence
    pch = buf;
    while (*pch) {
      sscanf(pch, "%s", crypted_words[i]);
      pch += strlen(crypted_words[i]) + 1; //1 is for space
      i++;
    }

    pch = buf;
    /*printf("word count = %d\n", i);*/
    if (find_words(crypted_words, 0, i, code_map)) {
      //code_map contains valid mapping for decryption
      while (*pch) {
        printf("%c", *pch == ' ' ? ' ' : code_map[*pch]);
        pch++;
      }
    } else {
      while (*pch) {
        printf("%c", *pch == ' ' ? ' ' : '*');
        pch++;
      }
    }
    printf("\n");
  }

  return 0;
}


int print_code_map(char code_map[]) {
  char c;
  for (c='a'; c<='z'; c++) {
    if (code_map[c])
      printf("%c -> %c, ", c, code_map[c]);
  }
  printf("\n");
}

int find_words(char crypted_words[45][MAX_LEN+1], int word_index, int word_count, char code_map[]) {
  int i,j;
  char *cand; //candidate original word for current crypted word
  char *current_word; //current crypted word begin indicated by word_index
  int len; //current crypted word length
  char backup_code_map[122+1] = {0,}; //backup codemap

  if (word_count == word_index) {
    return 1;
  }

  current_word = crypted_words[word_index];
  len = strlen(current_word);

  /*for (j=0; j<word_index; j++) printf("  ");*/
  /*printf("word = %s\n", current_word);*/

  memcpy(backup_code_map, code_map, sizeof(char)*(122+1));
  for (i=0; i<len_dict_count[len]; i++) { //iterate candidates
    cand = len_dict[len][i];
    /*for (j=0; j<word_index; j++) printf("  ");*/
    /*printf("cand = %s\n", cand);*/

    /*for (j=0; j<word_index; j++) printf("  ");*/
    /*print_code_map(code_map);*/

    if (update_code_map(code_map, current_word, cand, len)) {
      if (find_words(crypted_words, word_index+1, word_count, code_map))
        return 1;
      else
        memcpy(code_map, backup_code_map, sizeof(char)*(122+1));
    } else {
      memcpy(code_map, backup_code_map, sizeof(char)*(122+1));
    }
  }

  return 0;
}

void revert_code_map(char code_map[], char *crypted, int len) {
  int i;
  for (i=0; i<len; i++)
    code_map[crypted[i]] = 0;
}

int update_code_map(char code_map[], char *crypted, char *original, int len) {
  int i,j;
  for (i=0; i<len; i++) {
    if (code_map[crypted[i]] && code_map[crypted[i]] != original[i]) 
      return 0; //cannot update code map. inconsistency found
    else
      code_map[crypted[i]] = original[i];
  }

  //duplicate member check
  for (i='a'; i<='y'; i++) {
    for (j=i+1; j<='z'; j++) {
      if (i == j) continue;
      if (code_map[i] && code_map[j] && code_map[i] == code_map[j]) {
        /*printf("code_map[%c] = code_map[%c] = %c\n", i, j, code_map[i]);*/
        return 0;
      }
    }
  }
  return 1;
}
