#include <stdio.h>
#include <string.h>
#include <assert.h>

#define MAX_FRAG_LEN 256
#define MAX_FILE_NUM 144
#define MAX_FRAG_COUNT MAX_FILE_NUM*2

int perm(int arr[], int n, int index, int l1, int l2, int candidates[2][MAX_FRAG_LEN+1], int cand_index, int frags[MAX_FRAG_LEN+1][MAX_FILE_NUM*2][MAX_FRAG_LEN]);
int frags[MAX_FRAG_LEN+1][MAX_FILE_NUM*2][MAX_FRAG_LEN] = {0,}; //frags[i][j] = 길이가 i인 fragment의 j번째 fragment

int main() {
  int t; //test cases
  int i,j,k; //loop var
  char buf[BUFSIZ];
  char *pch; //tracking pointer of buf
  int frag_count[MAX_FRAG_LEN+1] = {0,}; //frag_count[i] = 길이가 i인 fragment의 개수
  int frag_len; //length of current frag
  int min_frag_count; //minimum frag count 
  int min_frag_count_len; //length of fragments which have min frag count
  int arr[MAX_FRAG_COUNT]; //순열을 만들어내기 위한 어레이 
  int file_len; //한 파일의 길이
  int total_frag_cnt; //전체 조각 개수
  int total_file_cnt; //전체 파일 개수
  int sum_frag_len; //전체 조각길이의 합
  int candidates[2][MAX_FRAG_LEN+1]; //조각 두개가 합쳐진 배열. 두개 붙이는 순서에 따라 인덱스가 바뀌어야.

  //순열 배열 초기화
  for (i=0; i<MAX_FRAG_LEN; i++)
    arr[i] = i;

  scanf("%d", &t);
  fgets(buf, BUFSIZ-1, stdin); //consume new line char

  while (t--) {
    fgets(buf, BUFSIZ-1, stdin); //consume empty line

    //init
    total_frag_cnt = 0;
    total_file_cnt = 0;
    sum_frag_len = 0;

    //input fragments
    while(fgets(buf, BUFSIZ-1, stdin) != NULL) {
      if (buf[0] == '\n') break; //new line. test case end
      frag_len = strlen(buf);
      if (buf[frag_len - 1] == '\n') { //remove newline char
        buf[frag_len - 1] = 0;
        frag_len--;
      }

      for (i=0; i<frag_len; i++)
        frags[frag_len][frag_count[frag_len]][i] = buf[i] - '0';

      frag_count[frag_len]++;
      total_frag_cnt++;
      sum_frag_len += frag_len;
    }

    assert(total_frag_cnt % 2 == 0);
    total_file_cnt = total_frag_cnt/2;
    assert(sum_frag_len % total_file_cnt == 0);
    file_len = sum_frag_len / total_file_cnt;

    printf("sum_frag_len = %d, total_frag_cnt = %d, total_file_cnt = %d, file_len = %d\n", sum_frag_len, total_frag_cnt, total_file_cnt, file_len);

    //find min frag count which should not be 0
    min_frag_count = 2*MAX_FILE_NUM;
    for (i=1; i<=MAX_FRAG_LEN; i++) {
      if (frag_count[i] > 0 && min_frag_count > frag_count[i]) {
        min_frag_count = frag_count[i];
        min_frag_count_len = i;
      }
    }
    printf("min_frag_count = %d, min_frag_count_len = %d\n", min_frag_count, min_frag_count_len);

    // [0, min_frag_count_len)로 만들수 있는 순열을 가지고 비교를 한다.
    perm(arr, min_frag_count_len, 0, min_frag_count_len, file_len - min_frag_count_len, candidates, 0, frags);
  }

  return 0;
}

void swap_arr(int arr[], int i, int j) {
  int t = arr[j];
  arr[j] = arr[i];
  arr[i] = t;
}

int is_equal(int target[], int frag1[], int l1, int frag2[], int l2) {
  //frag1와 frag2로 target을 만들어낼 수 있으면 1을 리턴
  //case 1: frag1을 앞에 frag2를 뒤에
  int i;//loop var
  int flag = 1; //스트링 매치에 성공했음을 나타내는 플래그. 1 = 성공
  for (i=0; i<l1; i++) {
    if (target[i] != frag1[i]) {
      flag = 0;
      break;
    }
  }
  if (flag) { //flag = 0이라는 건 frag1을 앞에 놓은거에서 이미 틀렸단 소리니까 검증할 필요가 없음
    for (i=0; i<l2; i++) {
      if (target[i+l1] != frag2[i]) {
        flag = 0;
        break;
      }
    }
  }

  //case 2: frag2, frag1
  if (!flag) { //flag가 0이라는건 1-1에서 실패했음을 뜻함
    flag = 1;
    for (i=0; i<l2; i++) {
      if (target[i] != frag2[i]) {
        flag = 0;
        break;
      }
    }
    if (flag) {
      for (i=0; i<l1; i++) {
        if (target[l2+i] != frag1[i]) {
          flag = 0;
          break;
        }
      }
    }
  }

  return flag;
}

int perm(int arr[], int n, int index, int l1, int l2, int candidates[2][MAX_FRAG_LEN+1], int cand_index, int frags[MAX_FRAG_LEN+1][MAX_FILE_NUM*2][MAX_FRAG_LEN]) {
  int i,j;
  int flag = 1; //기준스트링과 만들어낸 스트링이 일치하는지를 보기 위한 플래그
  if (index == n) {
    //정답 출력 
    for (i=0; i<l1+l2; i++)
      printf("%d", candidates[cand_index][i]);
    printf("\n");
    return 1;
  }

  for (i=index; i<n; i++) {
    if (index == 0) { //아직 맞춰본 조각이 없는 때
      //check : 길이l1짜리의 arr[i]번째 조각과 길이 l2의 0번째 조각을 가지고 string을 만들어보자
      for (j=0; j<l1; j++)
        candidates[0][j] = frags[l1][arr[i]][j];
      for (j=0; j<l2; j++)
        candidates[0][j+l1] = frags[l2][0][j];

      for (j=0; j<l2; j++)
        candidates[1][j] = frags[l2][0][j];
      for (j=0; j<l1; j++)
        candidates[1][j+l2] = frags[l1][arr[i]][j];

    } else { 
      //이미 기준스트링인 cand와 cand2가 있는 상태

      //길이 l1짜리 arr[i]번째 조각과 길이 l2짜리 index번째 조각을 가지고 string을 만들었을때
      //기준 스트링 중 하나라도 일치하는가? 

      //case 1: cand와 일치하는지 보자
      flag = is_equal(candidates[0], frags[l1][arr[i]], l1, frags[l2][index], l2);
      if (flag) 
        cand_index = 0;

      if (!flag) {
      //case 2: cand2와 일치하는지
        if (is_equal(candidates[1], frags[l1][arr[i]], l1, frags[l2][index], l2)) {
          cand_index = 1;
          flag = 1;
        }
      }
    }

    if (flag) {
      swap_arr(arr, i, index);
      if (perm(arr, n, index+1, l1, l2, candidates, cand_index, frags))
        return 1;
      swap_arr(arr, i, index);
    }
  }

  return 0;
}
