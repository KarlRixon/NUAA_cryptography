#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char *words = "I LOVE NANJING UNIVERSITY OF AERONAUTICS AND ASTRONAUTICS";
int  enc[100];
int  encCount=0;
char dec[100];
int  decCount=0;
#define p 563               // private
#define q 281               // private
// static const int  p=563;
// static const int  q=281;
// #define p (int )563
// #define q (int )281
const int  n = p*q;                // p*q  public
int  e = 0;                  // 1<e<phi  public
int  d = 0;                  // ed=1 mod n  private
const int  phi = (p-1)*(q-1);      // (p-1)*(q-1)  private
const int  mr = 10;                // miller-rabin 循环轮数

// 扩展Euclid算法
int  mod_inv(int  t, int  m){
    int  a = t, b = m;
	int  x = 0, y = 1, x0 = 1, y0 = 0, q0, temp;
	while(b != 0) {
		q0 = a / b;
		temp = a % b;
		a = b;
		b = temp;
		temp = x; x = x0 - q0 * x; x0 = temp;
		temp = y; y = y0 - q0 * y; y0 = temp;
	}
	if(x0 < 0) x0 += m;
	return x0;
}

// Miller-Rabin 素性测试
int  MillRab(int  m){
    srand((unsigned)time(NULL));
    int  a = rand() % (m - 3) + 2;

    int  s = 0, t = m - 1;
    while (t % 2 == 0) {
        t /= 2; s++;
    }
    int  x = 1;
    for (int  i = 0; i < t; i++) {
        x = x * a % m;
    }
    if (x == 1 || x == m - 1) {
        return 1;
    }
    for (int  i = 0; i < s; i++) {
        x = x * x % m;
        if (x == m - 1) {
            return 1;
        }
    }
    return 0;
}

int  isPrime(int  t){
    for (int  i = 0; i < mr; i++) {
        if (!MillRab(t)) {
            return 0;
        }
    }
    return 1;
}

int  pow_mod(int  t, int  k, int  m){
    int  r=1, base=t;
    while(k != 0){
        if(k & 1)
            r = (r * base) % m;
        base = (base * base) % m;
        k>>=1;
    }
    return r;
}

int  gcd(int  a,int  b)  {  
    int  r;  
    while(b>0)  
    {  
         r=a%b;  
         a=b;  
         b=r;  
    }  
    return a;  
}  

void gen_Key(){
    srand((unsigned)time(NULL));
    e=rand() % (phi-2) + 2;
    while(gcd(e, phi)!=1){
        //printf("###\n");
        e = rand() % (n - 2) + 2;
    }
    e=3;
    d = mod_inv(e, phi);
}

// 两个字符合并加密防止统计字符频率
void RSAEncrypt(){
    int  i=0,sum=0,temp=0;
    for(i=0; words[i]!='\0'; i++){
        decCount++;
        if(words[i]==' ') temp=0;
        else temp=words[i]-'A'+1;
        if((i%2)==0){
            sum=temp*100;
        }else{
            encCount++;
            sum+=temp;
            enc[encCount-1]=pow_mod(sum, e, n);
            if(enc[encCount-1]<0) enc[encCount-1]+=n;
        }
    }
    if ((decCount % 2)==0)
    {
        encCount++;
        enc[encCount] = pow_mod(words[i] * 100, e, n);
    }
}

void RSADecrypt(){
    for(int  i=0; i<encCount; i++){
        int  temp=pow_mod(enc[i], d, n);
        dec[i*2]=temp/100;
        dec[i*2+1]=temp%100;
        if(dec[i*2]==0){
            dec[i*2]=' ';
        }else if(dec[i*2]<0){
            dec[i*2]=dec[i*2]+n;
            dec[i*2]=dec[i*2]-1+'A';
        }
        else{
            dec[i*2]=dec[i*2]-1+'A';
        }
        if(dec[i*2+1]==0){
            dec[i*2+1]=' ';
        }else if(dec[i*2]<0){
            dec[i*2+1]=dec[i*2+1]+n;
            dec[i*2+1]=dec[i*2+1]-1+'A';
        }
        else{
            dec[i*2+1]=dec[i*2+1]-1+'A';
        }
    }
}

int  main(){
    int  ans=0;

    // 模逆运算
    int  test=11;
    ans=mod_inv(test,p);
    printf("%d^(-1) mod %d = %d\n", test, p, ans);
    printf("%d*%d mod %d = %d\n\n", test, ans, p, test*ans%p);

    // 素性测试
    printf("is %d a prime? %d\n", q, isPrime(q));
    printf("is %d a prime? %d\n\n", p, isPrime(p));

    // 幂模运算
    ans = pow_mod(7, 563, 561);
    printf("7^563 mod 561 = %d\t(should be 343)\n\n", ans);

    // 加密解密
    // printf("%d\n", pow_mod(900, 1131, 158203));
    gen_Key();
    printf("p: %d\nq: %d\nn: %d\nphi: %d\ne(public key): %d\nd(private key): %d\n", p, q, n, phi, e, d);
    printf("%d*%d mod %d = %d\n", e, d, phi, e*d%phi);
    printf("message is: %s\n", words);
    RSAEncrypt();
    printf("encrypt:\n");
    for(int  i=0; i<encCount; i++){
        printf("%08x\t", enc[i]);
    }
    printf("\n");
    RSADecrypt();
    printf("decrypt: %s\n", words);
    for(int  i=0; i<decCount; i++){
        printf("%d\t", words[i]);
    }
}