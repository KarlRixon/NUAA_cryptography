#include<stdlib.h>
#include<stdio.h>
#include<time.h>

const int k1=2,k2=3; // 注意k2值应小于26且与26互质，如果大于程序会出错
const int length=10000;

/* 原理：
   c===(k1+k2*m)mod26
   m===k2^-1(c-k1)mod26
*/

// 解二元一次不定方程（求模运算逆元）
void ext_gcd(int a, int b, int &d, int &x, int &y)
{
	if (b == 0)
	{
		d = a;
		x = 1;
		y = 0;
		return;
	}
	ext_gcd(b, a % b, d, y, x);
	y -= a / b * x;
	return;
}

// a-97 z-122
// A-65 Z-90

int main(){
	// printf("%c\n", 90);
	char m[length];
	char c[length];
	char d[length];
	float possibilityM[26]={0};
	float possibilityC[26]={0};
	int sortM[26]={0};
	int sortC[26]={0};
	srand(time(NULL));
	rand();
	// for(int i=0; i<10; i++){
	// 	printf("%c ", rand()%26+65);
	// }
	// 生成明文
	for(int i=0; i<length; i++){
		int random=rand()%26;
		possibilityM[random]+=1;
		m[i]=random+65;
	}
	// 生成密文
	for(int i=0; i<length; i++){
		int random=((m[i]-65)*k2+k1)%26;
		possibilityC[random]+=1;
		c[i]=random+65;
	}
	// 打印明文和密文
	// for(int i=0; i<length; i++){
	// 	printf("%c ", m[i]);
	// }
	// printf("\n");
	// for(int i=0; i<length; i++){
	// 	printf("%c ", c[i]);
	// }
	// printf("\n");
	// 统计字母出现频率
	for(int i=0; i<26; i++){
		// printf("%f,%f  ", possibilityC[i], possibilityM[i]);
		possibilityM[i]/=(float)length;
		possibilityC[i]/=(float)length;
	}

	// 解密
	int x, y, dd = 0;
	int a, b;
	int k_1;		// 要求的模逆k2^-1
	a = 26;
	b = k2;
	ext_gcd(26, k2, dd, x, y);
	k_1 = -((x + b) % b*26-1)/b+26;		// 重要
	if(dd!=1){
		printf("k2模逆计算出错\n");
		exit(0);
	}
	// printf("%d\n", k_1);
	for(int i=0; i<length; i++){
		d[i] = ((k_1 * (c[i] - 65 - k1)) + 26) % 26 + 65;		// 重要
		// printf("%c.%c.%c ", m[i], c[i], d[i]);
		if (m[i] != d[i])
		{
			printf("第%d个字母解密出错\n", i+1);
			break;
		}
	}
	printf("success!\n");
	// system("pause");

	// 频率攻击
	for(int i=0; i<26; i++){
		for(int j=0; j<26; j++){
			if (possibilityM[i]==possibilityC[j]){
				printf("%c(%f)->%c(%f)\t", i+65, possibilityM[i], j+65, possibilityC[j]);
				//printf("%c->%c\t", i+65, j+65);
			}
		}
	}

	// 输出明文，密文，解密，攻击结果
	FILE *fp;
	if ((fp = fopen("message.txt", "w")) == NULL)
	{
		printf("Cannot open the file,strike any key to exit!\n");
		getchar();
		exit(0);
	}
	for(int i=0; i<length; i++){
		fputc(m[i], fp);
	}
	fputc('\n', fp);
	fclose(fp);

	if ((fp = fopen("cipher.txt", "w")) == NULL)
	{
		printf("Cannot open the file,strike any key to exit!\n");
		getchar();
		exit(0);
	}
	for (int i = 0; i < length; i++)
	{
		fputc(c[i], fp);
	}
	fputc('\n', fp);
	fclose(fp);

	if ((fp = fopen("decrypt.txt", "w")) == NULL)
	{
		printf("Cannot open the file,strike any key to exit!\n");
		getchar();
		exit(0);
	}
	for (int i = 0; i < length; i++)
	{
		fputc(d[i], fp);
	}
	fputc('\n', fp);
	fclose(fp);

	if ((fp = fopen("compare.txt", "w")) == NULL)
	{
		printf("Cannot open the file,strike any key to exit!\n");
		getchar();
		exit(0);
	}
	for (int i = 0; i < length; i++)
	{
		fputc(m[i], fp);
		fputc('.', fp);
		fputc(c[i], fp);
		fputc('.', fp);
		fputc(d[i], fp);
		fputc('\t', fp);
	}
	fputc('\n', fp);
	fclose(fp);
}