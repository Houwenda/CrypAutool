#include <cmath>
#include <QDebug>
#include <QString>

//char转ascii
QString char_to_ascii(char x){
    int n = x;
    //cout<<n;
    char m[10];
    for(int i=0;i<10;i++) m[i] = 0;
    int i = 0;
    while(n/10!=0){
        n /= 10;
        i++;
    }
    n = x;
    while(i>=0){
        m[i] = n - (n/10)*10 + '0';
        n /= 10;
        i--;
    }
    QString str = QString(QLatin1String(m));
    return str;
}
//凯撒
QString ceasar_cipher(QString input,int k)
{
    char text[1024];
    int a[26];
    int A[26];
    int m;
    for(int i=97;i<123;i++){
        a[i-97] = i;
        A[i-97] = i-32;
    }
    int len = input.length();
    char* cipher = input.toLatin1().data();
    for(int i=0;i<len;i++){
        int t = cipher[i];
        if(t>96&&t<123){
            m = (cipher[i] - k - 71) % 26;
            text[i] = a[m];
        }
        else if(t>64&&t<91){
            m = (cipher[i] - k - 39) % 26;
            text[i] = A[m];
        }
        else{
            text[i] = ' ';
        }
    }

    return QString(QLatin1String(text));
}

//单表置换秘钥
QString transkey(QString input)
{
    char* key = input.toLatin1().data();
    char table[27];
    char tem[100] = { 0 };
    char table1[30] = { 0 };
    for (int i = 97; i < 123; i++)  table1[i - 97] = i;
    int len1 = input.length();
    strcpy(tem, key);
    strcpy(tem + len1, table1);//拼接
    int k = 1; int flag;
    //判断是否为大写
    if (isupper(tem[0]))    tem[0] += 32;
    table[0] = tem[0];
    for (int i = 1; i < strlen(tem) && k < 26; i++){
        if (!isalpha(tem[i]))   continue;
        if (isupper(tem[i]))    tem[i] += 32;
        flag = 0;
        for (int j = 0; j < k; j++){
            if (table[j] == tem[i]){
                flag = 1;
                break;
            }
        }
        if (!flag)	table[k++] = tem[i];
    }
    return QString(QLatin1String(table));
}

//单表置换（重新制表）
QString ttable(QString input)
{
    char* table = input.toLatin1().data();
    char ip[30] = { 0 };
    for (int i = 97; i < 122; i++) {
        ip[table[i - 97] - 'a'] = i;
    }
    memcpy(table, ip, 26);
    return QString(QLatin1String(table));
}

//单表置换解密
QString decrypt2(QString input1, QString input2)
{
    char* cipher = input1.toLatin1().data();
    char* key = input2.toLatin1().data();
    char text[1024];
    char table[27] = { 0 };
    transkey(key);
    ttable(table);
    int len = strlen(cipher);
    for (int i = 0; i < len; i++) {
        if (!isalpha(cipher[i]))    text[i] = cipher[i];
        else if (islower(cipher[i]))	text[i] = table[cipher[i] - 97];
        else	text[i] = table[cipher[i] - 65] - 32;
    }
    return QString(QLatin1String(text));
}

//埃特巴什码 解密
QString decrypt3(QString input)
{
    char* cipher = input.toLatin1().data();
    char text[1024];
    int len = strlen(cipher);
    for (int i = 0; i < len; i++)
    {
        int t = cipher[i];
        if (t >= 65 && t <= 77)//A~M
        {
            t = t + 25 - (t - 65) * 2;
        }
        else if (t >= 78 && t <= 90)//N~Z
        {
            t = t - 25 + (90 - t) * 2;
        }
        else if (t >= 97 && t <= 109)//a~m
        {
            t = t + 25 - (t - 97) * 2;
        }
        else//n~z
        {
            t = t - 25 + (122 - t) * 2;
        }
    }
    return QString(QLatin1String(text));
}

//仿射密码 解密
int rec(int k, int m) {
    int i = 1;
    int j = 0;
    while (1) {
        for (j = 0; k*j < m*i + 1; j++) {}
        if (k*j == m * i + 1)
            break;
        else
            i++;
    }
    return j;
}
QString decrypt7(QString input, int k1, int k2)
{
    char* cipher= input.toLatin1().data();
    char text[1024];
    int len = strlen(cipher);
    int  e = 0;
    int k3 = rec(k1, 26);
    for (int i = 0; i < len; i++) {
        if (!isalpha(cipher[i])) {
            text[i] = cipher[i];
        }
        else if (islower(cipher[i])) {
            e = cipher[i] - 'a';
            text[i] = (k3*(e - k2 + 26)) % 26 + 'a';
        }
        else {
            e = cipher[i] - 'A';
            text[i] = ((e - k2 + 26)*k3) % 26 + 'A';
        }
    }
    return QString(text);
}

//维吉尼亚密码  解密
QString decrypt8(QString input1, QString input2) {
    char* cipher = input1.toLatin1().data();
    char* key = input2.toLatin1().data();
    char text[1024];
    int len = strlen(cipher);
    int lenkey = strlen(key);
    for (int j = 0; j < lenkey; j++)
        if (isupper(key[j]))
            key[j] += 32;
    for (int i = 0; i < len; i++) {
        if (!isalpha(cipher[i]))
            text[i] = cipher[i];
        else if (isupper(cipher[i]))
            text[i] = (26 + cipher[i] - 'A' - (key[i%lenkey] - 'a')) % 26 + 'A';
        else
            text[i] = (26 + cipher[i] - 'a' - (key[i%lenkey] - 'a')) % 26 + 'a';
    }
    return QString(QLatin1String(text));
}

//Base64 解密
int num_strchr(const char *str, char c) //
{
    const char *pindex = strchr(str, c);
    if (NULL == pindex){
        return -1;
    }
    return pindex - str;
}

int base64_decode(const char * base64, unsigned char * dedata)
{
    int i = 0, j=0;
    int trans[4] = {0,0,0,0};
    for (;base64[i]!='\0';i+=4){

        trans[0] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", base64[i]);
        trans[1] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", base64[i+1]);

        dedata[j++] = ((trans[0] << 2) & 0xfc) | ((trans[1]>>4) & 0x03);

        if (base64[i+2] == '='){
            continue;
        }
        else{
            trans[2] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", base64[i + 2]);
        }

        dedata[j++] = ((trans[1] << 4) & 0xf0) | ((trans[2] >> 2) & 0x0f);

        if (base64[i + 3] == '='){
            continue;
        }
        else{
            trans[3] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/", base64[i + 3]);
        }


        dedata[j++] = ((trans[2] << 6) & 0xc0) | (trans[3] & 0x3f);
    }

    dedata[j] = '\0';

    return 0;
}
//base32
int base32_decode(const char * base32, unsigned char * dedata)
{
    int i = 0, j=0;
    int trans[8] = {0,0,0,0,0,0,0,0};
    for (;base32[i]!='\0';i+=8){

        trans[0] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i]);
        trans[1] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i+1]);

        dedata[j++] = ((trans[0] << 3) & 0xf8 ) | ((trans[1] >> 2) & 0x07 );
        //dedata[j++] = ((trans[0] << 3) & 0xf8) | ((trans[1]>>2) & 0x07);

        if (base32[i+2] == '='){
            continue;
        }
        else{
            trans[2] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i + 2]);
        }

        //dedata[j++] = ((trans[1] << 4) & 0xf0) | ((trans[2] >> 2) & 0x0f);

        if (base32[i + 3] == '='){
            continue;
        }
        else{
            trans[3] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i + 3]);
        }

        dedata[j++] = ((trans[1] << 6) & 0xc0 ) | ((trans[2] << 1) & 0x3e ) | ((trans[3] >> 4) & 0x01 );

        if (base32[i + 4] == '='){
            continue;
        }
        else{
            trans[4] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i + 4]);
        }

        dedata[j++] = ((trans[3] << 4) & 0xf0 ) | ((trans[4] >> 1) & 0x0f );

        if (base32[i + 5] == '='){
            continue;
        }
        else{
            trans[5] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i + 5]);
        }

        if (base32[i + 6] == '='){
            continue;
        }
        else{
            trans[6] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i + 6]);
        }

        dedata[j++] = ((trans[4] << 7) & 0x80 ) | ((trans[5] << 2) & 0x7c ) | ((trans[6] >> 3) & 0x03 );

        if (base32[i + 7] == '='){
            continue;
        }
        else{
            trans[7] = num_strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ234567", base32[i + 7]);
        }

        dedata[j++] = ((trans[6] << 5) & 0xe0 ) | (trans[7] & 0x1f);
        //dedata[j++] = ((trans[2] << 6) & 0xc0) | (trans[3] & 0x3f);
    }

    dedata[j] = '\0';

    return 0;
}
//base16
int base16_decode(const char * base16, unsigned char * dedata)
{
    int i = 0, j=0;
    int trans[2] = {0,0};
    for (;base16[i]!='\0';i+=2){

        trans[0] = num_strchr("0123456789ABCDEF", base16[i]);
        trans[1] = num_strchr("0123456789ABCDEF", base16[i+1]);

        dedata[j++] = (trans[0] << 4) | (trans[1]);

        //dedata[j++] = ((trans[2] << 6) & 0xc0) | (trans[3] & 0x3f);
    }

    dedata[j] = '\0';

    return 0;
}

//URL编码 解密
char dec2hexChar(short int n) {
 if ( 0 <= n && n <= 9 ) {
  return char( short('0') + n );
 } else if ( 10 <= n && n <= 15 ) {
  return char( short('A') + n - 10 );
 } else {
  return char(0);
 }
}

short int hexChar2dec(char c) {
 if ( '0'<=c && c<='9' ) {
  return short(c-'0');
 } else if ( 'a'<=c && c<='f' ) {
  return ( short(c-'a') + 10 );
 } else if ( 'A'<=c && c<='F' ) {
  return ( short(c-'A') + 10 );
 } else {
  return -1;
 }
}

QString deescapeURL(const char* URL) {
 QString result = "";
 for ( unsigned int i=0; i<strlen(URL); i++ ) {
  char c = URL[i];
  if ( c != '%' ) {
   result += c;
  } else {
   char c1 = URL[++i];
   char c0 = URL[++i];
   int num = 0;
   num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
   result += char(num);
  }
 }
 return result;
}

//Unicode编码 解密(&#x [Hex]：)
QString decrypt12(QString cipher)
{
    char* input = cipher.toLatin1().data();
    QString result;
    int i=0;
    for(int i=0;i<strlen(input);i++){
        if(input[i]=='&'&&input[i+1]=='#'&&input[i+2]!='x'){
            int n = 0;
            n += (input[i+3]-'0')*100;
            n += (input[i+4]-'0')*10;
            n += (input[i+5]-'0');
            result.append(QChar(n));
        }
        else if(input[i]=='&'&&input[i+1]=='#'&&input[i+2]=='x'){
            int n = 0;
            n += (input[i+5]-'0')*16;
            if(input[i+6]>'0'&&input[i+6]<='9'){
                n += input[i+6] - '0';
            }
            else if(input[i+6]=='A'||input[i+6]=='a'){
                n += 10;
            }
            else if(input[i+6]=='B'||input[i+6]=='b'){
                n += 11;
            }
            else if(input[i+6]=='C'||input[i+6]=='c'){
                n += 12;
            }
            else if(input[i+6]=='D'||input[i+6]=='d'){
                n += 13;
            }
            else if(input[i+6]=='E'||input[i+6]=='e'){
                n += 14;
            }
            else if(input[i+6]=='F'||input[i+6]=='f'){
                n += 15;
            }
            result.append(QChar(n));
        }
        else if(input[i]=='\\'&&(input[i+1]=='U'||input[i+1]=='u')&&input[i+2]!='+'){
            int n = 0;
            n += (input[i+4]-'0')*16;
            if(input[i+5]>'0'&&input[i+5]<='9'){
                n += input[i+5] - '0';
            }
            else if(input[i+5]=='A'||input[i+5]=='a'){
                n += 10;
            }
            else if(input[i+5]=='B'||input[i+5]=='b'){
                n += 11;
            }
            else if(input[i+5]=='C'||input[i+5]=='c'){
                n += 12;
            }
            else if(input[i+5]=='D'||input[i+5]=='d'){
                n += 13;
            }
            else if(input[i+5]=='E'||input[i+5]=='e'){
                n += 14;
            }
            else if(input[i+5]=='F'||input[i+5]=='f'){
                n += 15;
            }
            result.append(QChar(n));
        }
        else if(input[i]=='\\'&&(input[i+1]=='U'||input[i+1]=='u')&&input[i+2]=='+'){
            int n = 0;
            n += (input[i+5]-'0')*16;
            if(input[i+6]>'0'&&input[i+6]<='9'){
                n += input[i+6] - '0';
            }
            else if(input[i+6]=='A'||input[i+6]=='a'){
                n += 10;
            }
            else if(input[i+6]=='B'||input[i+6]=='b'){
                n += 11;
            }
            else if(input[i+6]=='C'||input[i+6]=='c'){
                n += 12;
            }
            else if(input[i+6]=='D'||input[i+6]=='d'){
                n += 13;
            }
            else if(input[i+6]=='E'||input[i+6]=='e'){
                n += 14;
            }
            else if(input[i+6]=='F'||input[i+6]=='f'){
                n += 15;
            }
            result.append(QChar(n));
        }
    }


    return QString(cipher);
}

//md5爆破
QString md5BruteForce(QString input){
    if(input.length()==32){
        //截取16位md5
        input = input.mid(9,16);
        qDebug()<<"32位md5"<<input<<endl;
    }
    else{
        qDebug()<<"16位md5"<<input<<endl;
    }
    return "";
}
