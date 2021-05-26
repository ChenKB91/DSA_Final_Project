#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
int len;
char** get_token(char* s)
{
    int i=0;
    int j=0;
    int token=0;
    char** ret = malloc( (strlen(s)/2)*sizeof(char*) );
    char* buf = malloc( 50*sizeof(char) );
    while(s[i] != '\0'){
        if(isdigit(s[i])||isalpha(s[i])){
            buf[j] = s[i];
            j+=1;
        }else if( j!=0 ){
          j=0;
          buf=realloc(buf,(strlen(buf)+1)*sizeof(char));
          ret[token] = buf;
          token+=1;
          buf=malloc(50*sizeof(char));
        }
        i+=1;
    }
    len=token;
    ret = realloc(ret,token*sizeof(char*));
    return ret;
}

int main(){
    char s[]= "On 23 May 2021, during a scheduled trip, the Stresa-Alpino-Mottarone Cable Car aerial tramway crashed to the ground after the traction or haulage cable snapped about 100 meters (300 ft) from the summit of the mountain Mottarone, which is near Lake Maggiore in northern Italy. Fourteen people were killed as a result";
    char** p = get_token(s);
    for(int i=0;i<len;i++){
        printf("%s\n",p[i]);
    }
}
