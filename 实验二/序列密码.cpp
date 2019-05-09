#include<stdlib.h>
#include<stdio.h>

const int length = 100;

int pulse(int lfsr[]){
    int in,out;
    in = lfsr[0]^lfsr[3];
    out = lfsr[0];
    for(int i=0; i<4; i++){
        lfsr[i] = lfsr[i+1];
    }
    lfsr[4]=in;
    return out;
}

int main(){
    // 秘钥
    int lfsr[5] = {0, 1, 1, 0, 1};
    int result[length];
    printf("key:\n");
    for(int i=0; i<length; i++){
        result[i] = pulse(lfsr);
        printf("%d ", result[i]);
    }
    printf("\n");
    // 明文
    char str[5]={'h','e','l','l','o'};
    int m[sizeof(str)*8]={0};
    for(int i=0; i<sizeof(str); i++){
        int temp=str[i];
        for(int j=7; j>=0; j--){
            m[8*i+j]=temp%2;
            temp/=2;
        }
    }
    printf("message:\n");
    for(int i=0; i<8*sizeof(str); i++){
        printf("%d ", m[i]);
    }
    printf("\n");
    for (int i = 0; i < sizeof(str); i++)
    {
        printf("%c ", str[i]);
    }
    printf("\n");
    // 加密
    int c[sizeof(str)*8]={0};
    for(int i=0; i<sizeof(str)*8; i++){
        c[i]=m[i]^result[i];
    }
    printf("cipher:\n");
    for (int i = 0; i < 8 * sizeof(str); i++)
    {
        printf("%d ", c[i]);
    }
    printf("\n");
    // 解密
    int d[sizeof(str)*8]={0};
    char dm[sizeof(str)]={0};
    for(int i=0; i<sizeof(str)*8; i++){
        d[i]=c[i]^result[i];
    }
    printf("decrypt:\n");
    for(int i=0; i<sizeof(str)*8; i++){
        printf("%d ", d[i]);
    }
    printf("\n");
    for(int i=0; i<sizeof(str); i++){
        int rank=1;
        for(int j=7; j>=0; j--){
            dm[i]+=rank*d[i*8+j];
            rank*=2;
        }
    }
    for(int i=0; i<sizeof(str); i++){
        printf("%c ", dm[i]);
        if(str[i]!=dm[i]){
            printf("第%d个字母解密出错\n", i+1);
            exit(0);
        }
    }
    printf("\nsuccess!\n");

    // 输出到文件
    FILE *fp;
    if ((fp = fopen("compare.txt", "w")) == NULL)
    {
        printf("Cannot open the file,strike any key to exit!\n");
        getchar();
        exit(0);
    }
    for (int i = 0; i < sizeof(str); i++)
    {
        fputc(str[i], fp);
        fputc('.', fp);
        fputc(dm[i], fp);
        fputc('\t', fp);
    }
    fputc('\n', fp);
    fclose(fp);
    return 0;
}