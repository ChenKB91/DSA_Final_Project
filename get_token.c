#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int amount = 0;
char** get_token(char* s) {
    int len = strlen(s);
    int last = -1; // last position of [0-9a-zA-Z] alphabet "-1" means the previous alphabet is a non [0-9a-zA-Z] alphabet
    char** ret = malloc(len*sizeof(char*)/2);
    for(int i = 0; i <= len; i++) {
        if(isdigit(s[i]) || isalpha(s[i])) {
            if(last == -1)
                last = i;
        }
        else if(last != -1) {
            ret[amount] = malloc((i-last+1)*sizeof(char));
            strncpy(ret[amount], s+last, i-last);
            ret[amount][i-last] = '\0';
            last = -1;
            amount++;
        }
    }
    ret = realloc(ret,amount*sizeof(char*));
    return ret;
}

int main(){
    char s[]= "On 23 May 2021, during a scheduled trip, the Stresa-Alpino-Mottarone Cable Car aerial tramway crashed to the ground after the traction or haulage cable snapped about 100 meters (300 ft) from the summit of the mountain Mottarone, which is near Lake Maggiore in northern Italy. Fourteen people were killed as a result";
    char** p = get_token(s);
    for(int i=0;i<amount;i++){
        printf("%s\n",p[i]);
    }
}
