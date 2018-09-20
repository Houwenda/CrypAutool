#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWebEngineWidgets>
extern QString ceasar_cipher(QString cipher,int k);
extern int base64_decode(const char * base64, unsigned char * dedata);
extern QString decrypt12(QString input);
extern QString decrypt11(QString text);
extern QString deescapeURL(const char* URL);
extern int base32_decode(const char * base32, unsigned char * dedata);
extern int base16_decode(const char * base16, unsigned char * dedata);
extern QString decrypt3(QString input);
extern QString encrypt5(char* str);
extern QString zhalan(char *cipher);
extern QString md5BruteForce(QString input);
QString sha1BruteForce(QString input);
QString sha224BruteForce(QString input);
QString sha256BruteForce(QString input);
QString sha384BruteForce(QString input);
QString sha512BruteForce(QString input);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(this->width(),this->height());
    setWindowTitle("CrypAutool");
    view.setParent(ui->widget);
    view.setFixedHeight(800);
    view.setFixedWidth(700);
    view.setUrl(QUrl("file:///C:/Users/houwd/Desktop/programming/Qt5/classic_cryptography/testEcharts.html"));
    view.show();    
    ui->lineEdit->setFont(QFont("微软雅黑",14));
    ui->pushButton->setFont(QFont("微软雅黑",14,QFont::Bold));
    ui->pushButton->setStyleSheet("color:green");

    //ui->textEdit->setFocusPolicy(Qt::NoFocus);
    //ui->textEdit->setTextBackgroundColor(QColor(Qt::black));
    ui->textEdit->setFont(QFont("微软雅黑",10));
    ui->textEdit_2->setFont(QFont("微软雅黑",10));
    //ui->textEdit->append(QString("text"));
    //ui->textEdit->append(QString("test"));
    ui->textEdit->document()->setMaximumBlockCount(50);//最多显示10行
    ui->textEdit_2->document()->setMaximumBlockCount(500);
    //设定定时器，每秒触发一次事件，调整TextEdit光标位置
    /*
    QTimer *CurrentTime = new QTimer(this);
    CurrentTime->start(100000);
    connect(CurrentTime,&QTimer::timeout,[=](){
            //保持编辑器光标在最后一行
            QTextCursor cursor=ui->textEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->textEdit->setTextCursor(cursor);
            //ui->textEdit->append(QString("......"));
    });
    */
    //连接数据库
    //QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("MyDataBase.db");
    }

    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        qDebug()<<"open database succeeded"<<endl;
    }
    //database.close();

    //drawCharts函数测试
    //drawCharts(0," ",0,0);
}

MainWindow::~MainWindow()
{
    QFile file("C:\\Users\\houwd\\Desktop\\programming\\Qt5\\classic_cryptography\\raw_json.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"raw json reading failed";
        return ;
    }
    QTextStream in(&file);
    QString raw = in.readAll();
    file.close();
    QFile file1("C:\\Users\\houwd\\Desktop\\programming\\Qt5\\classic_cryptography\\testEcharts.json");
    if(!file1.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"json initializing failed"<<endl;
        return ;
    }
    QTextStream out(&file1);
    out<<raw;
    file1.close();
    delete ui;
}


//textedit输出
void MainWindow::printText(QString str,int k){
    if(k==1){
        ui->textEdit->append(str);
    }
    else if(k==2){
        ui->textEdit_2->append(str);
    }
}

//写入文件
bool MainWindow::writeQuipquip(QString str){
    ui->textEdit_2->append(str);
    QFile file("quipquip.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug()<<"Open failed"<< endl;
        return false;
    }
    QTextStream out(&file);
    str = str + "\r\n";
    out<<str;

    file.close();
    //qDebug()<<"file writing succeeded"<<endl;

    return true;
}
//接收quipquip消息
void MainWindow::replyFinished(QNetworkReply *reply)
{
    qDebug()<<"data received"<<endl;
    // 获取响应信息
    QByteArray bytes = reply->readAll();
    //qDebug()<<bytes<<endl;
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(bytes, &jsonError);
    if (jsonError.error != QJsonParseError::NoError) {
        qDebug() << QStringLiteral("解析Json失败");
        return;
    }
    QString source = QString(bytes);
    //qDebug()<<source<<endl;
    for(int i=0;i<source.length();i++){
        if(source[i]=='p'&&source[i+1]=='l'&&source[i+2]=='a'&&source[i+3]=='i'&&source[i+4]=='n'&&source[i+5]=='t'&&source[i+6]=='e'&&source[i+7]=='x'&&source[i+8]=='t'){
            i += 12;
            int j = i;
            while(source[j]!='"'){
                j++;
            }
            QString plaintext = source.mid(i,j-i);
            //qDebug()<<plaintext<<endl;
            if(writeQuipquip(plaintext)){
                //qDebug()<<"file writing succeeded"<<endl;
            }
            else{
                qDebug()<<"file writing failed"<<endl;
            }
            i = j;
            continue;
        }
    }
    qDebug()<<"plaintext writing succeeded"<<endl;
    //ui->textEdit->append(QString("quipquip analysing finished. . . . . ."));
    printText(QString("quipquip analysing finished. . . . . ."),1);
/*
    // 解析Json
    if (doucment.isObject()) {
        QJsonObject obj = doucment.object();
        QJsonValue value;
        if (obj.contains("data")) {
            value = obj.take("data");
            if (value.isString()||true) {
                QString data = value.toString();
                qDebug() << data;
            }
        }
    }
*/

}

//发送请求quipquip
bool MainWindow::requestQuipquip(void){

    qDebug()<<"clicked"<<endl;
    QString baseUrl = "https://6n9n93nlr5.execute-api.us-east-1.amazonaws.com/prod/dict";
    QUrl url(baseUrl);

    QString ciphertext = ui->lineEdit->text();

    ui->textEdit->append(QString("quipquip analysing starts. . . . . ."));

    QJsonObject json;
    json.insert("time",1);
    json.insert("shardidx",0);
    json.insert("shards",1);
    json.insert("clues","");
    json.insert("ciphertext",ciphertext);

    QJsonDocument document;
    document.setObject(json);
    QByteArray dataArray = document.toJson(QJsonDocument::Compact);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    request.setRawHeader("Origin","https://quipquip.com");
    request.setRawHeader("Referer","https://quipquip.com");
    request.setUrl(url);

    //QNetworkAccessManager manager();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply *)));

    manager->post(request,dataArray);
    qDebug()<<"data posted"<<endl;

    return true;
}

//接收brainfuck消息
void MainWindow::replyFinished1(QNetworkReply *reply)
{
    qDebug()<<"data received"<<endl;
    // 获取响应信息
    QByteArray bytes = reply->readAll();
    //qDebug()<<bytes<<endl;

    int i;
    for(i=0;i<bytes.length();i++){
        if(bytes[i]=='r'&&bytes[i+1]=='o'&&bytes[i+2]=='w'&&bytes[i+3]=='s') break;
    }
    int j;
    for(j=0;j<bytes.length()-i-9;j++){
        if(bytes[i+j+9]=='<'&&bytes[i+j+10]=='/'&&bytes[i+j+11]=='t') break;
    }
    QString result = bytes.mid(i+10,j-1);
    //qDebug()<<result;
    printText(result,2);

}

//发送请求brainfuck
void MainWindow::requestBrainfuck(QString input){

    QString baseUrl = "http://tool.bugku.com/brainfuck/";
    QUrl url(baseUrl);

    QString cipher = "input=";
    QString tmp_str;
    for(int i=0;i<input.length();i++){
        if(input[i]=='>') tmp_str.append("%3E");
        else if(input[i]=='<') tmp_str.append("%3C");
        else if(input[i]=='+') tmp_str.append("%2B");
        else if(input[i]==',') tmp_str.append("%2C");
        else if(input[i]=='[') tmp_str.append("%5B");
        else if(input[i]==']') tmp_str.append("%5d");
        else if(input[i]==' ') tmp_str.append("+");
        else tmp_str.append(input[i]);
    }
    cipher.append(tmp_str);
    cipher.append("&do=Brainfuck+to+Text");
    QByteArray postData;
    postData.append(cipher);

    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    //request.setRawHeader("Origin","https://quipquip.com");
    request.setRawHeader("Referer","http://tool.bugku.com/brainfuck/");
    request.setUrl(url);

    //QNetworkAccessManager manager();
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->proxyFactory()->setUseSystemConfiguration(true);
    connect(manager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished1(QNetworkReply *)));

    manager->post(request,postData);
    qDebug()<<"data posted"<<endl;

}

//画图表
void MainWindow::drawCharts(int result_count,QString ans,int type1,int type2){
    QFile file("C:\\Users\\houwd\\Desktop\\programming\\Qt5\\classic_cryptography\\testEcharts.json");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug()<<"json open failed"<<endl;
        return ;
    }
    QTextStream in(&file);
    QString text;
    text = in.readAll();
    //qDebug()<<text;
    int i;
    int j;
    QString categories;
    QString nodes;
    QString links;
    for(i=0;i<text.length();i++){
        if(text[i]=='n'&&text[i+1]=='o'&&text[i+2]=='d'&&text[i+3]=='e'&&text[i+4]=='s'){
            categories = text.mid(0,i-1);
            //qDebug()<<categories;
            for(j=0;j<text.length()-i;j++){
                if(text[i+j]==']'){
                    break;
                }
            }
            nodes = text.mid(i-1,j-2);
            //qDebug()<<nodes;
            //links = text.mid(i+j+3,text.length()-i-j-7);
            int k;
            for(k=0;k<text.length()-i-j-1;k++){
                if(text[i+j+1+k]==']'){
                    break;
                }
            }
            links = text.mid(i+j+5,k-7);
            //qDebug()<<links;
            break;
        }
    }
    file.close();
    //text = categories + nodes + "],\n" + links + "\n  ]\n}";

/*测试提取完整性
    QFile file1("outTest.txt");
    if(!file1.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug()<<"out file creating failed"<<endl;
        return ;
    }
    QTextStream out(&file1);
    out<<categories+nodes+"],\n"+links+"]\n}";
    file1.close();
*/
    //设置nodes
    if(type2<10){
        nodes = nodes + ",\n    {\n    \"name\": \""+ans+"\",\n    \"value\": 1,\n    \"category\": " + QChar(type2+'0') + "\n    }\n";
    }
    else{
        QString tmp = QChar(type2/10+'0');
        tmp += QChar((type2-(type2/10)*10)+'0');
        nodes = nodes +",\n    {\n    \"name\": \""+ans+"\",\n    \"value\": 1,\n    \"category\": " + tmp + "\n    }\n";
    }
    //设置links
    //qDebug()<<"start setting links";
    int count = 0;//节点总数
    for(i=0;i<nodes.length();i++){
        if(nodes[i]=='n'&&nodes[i+1]=='a'&&nodes[i+2]=='m'&&nodes[i+3]=='e') count++;
    }
    int target_num = count - 1;//目标节点位置
    int count2 = 0;
    for(i=nodes.length();i>0;i--){
        if(nodes[i]=='g'&&nodes[i+1]=='o'&&nodes[i+2]=='r'&&nodes[i+3]=='y'){
            count2++;
            if(nodes[i+8]=='\n'){
                //qDebug()<<nodes[i+7].toLatin1()-48;
                if(nodes[i+7].toLatin1()-48==type1&&count2>result_count+1) break;
            }
            else if(nodes[i+9]=='\n'){
                int category = nodes[i+8].toLatin1()-48;
                category += (nodes[i+7].toLatin1()-48)*10;
                //qDebug()<<category;
                if(category==type1&&count2>result_count+1) break;
            }
        }
    }
    //qDebug()<<"type1"<<type1;
    //qDebug()<<"location"<<i;
    //qDebug()<<"count"<<count;
    //qDebug()<<"count2:"<<count2;
    int source_num = count - count2;//起始节点位置
    //qDebug()<<"source:"<<source_num;
    //qDebug()<<"target:"<<target_num<<endl;

    links = links + ",\n   {\n    \"source\": "+QString::number(source_num,10);
    links = links + ",\n    \"target\": "+QString::number(target_num,10);
    links = links + "\n   }\n ";
//写入
    //QFile file1("outTest.txt");
    if(!file.open(QIODevice::ReadWrite | QIODevice::Text)){
        qDebug()<<"out file creating failed"<<endl;
        return ;
    }
    QTextStream out(&file);
    out<<categories+nodes+"  ],\n  "+links+" ]\n}";
    file.close();

}

//特征识别
void MainWindow::staticsAnalysis(QString input,int recurse_count, int type1){

    if(recurse_count==5) return;
    else{}
    int result_count = 0;//记录每运行一遍函数得到的结果数，用于解决画图表时同节点干扰问题
    QString result;
    //输入检测
    for(int i=0;i<input.length();i++){
        if(input[i]>=' '&&input[i]<='~'){}
        else{
            printText(QString("输入解析错误"),1);
            return ;
        }
    }
    //长度检测
    //sha256
    if(input.length()==64){
        printText("sha256",1);
        if(recurse_count==0){
            result = sha256BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"sha256",type1,8);
        result_count++;
    }
    //md5(32)
    else if(input.length()==32){
        printText("md5",1);
        if(recurse_count==0){
            result = md5BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"md5",type1,5);
        result_count++;
    }
    //md5(16)
    else if(input.length()==16){
        printText("md5",1);
        if(recurse_count==0){
            result = md5BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"md5",type1,5);
        result_count++;
    }
    //sha1
    else if(input.length()==40){
        printText("sha1",1);
        if(recurse_count==0){
            result = sha1BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"sha1",type1,6);
        result_count++;
    }
    //sha244
    else if(input.length()==56){
        printText("sha244",1);
        if(recurse_count==0){
            result = sha224BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"sha244",type1,7);
        result_count++;
    }
    //sha384
    else if(input.length()==96){
        printText("sha384",1);
        if(recurse_count==0){
            result = sha384BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"sha384",type1,9);
        result_count++;
    }
    //sha512
    else if(input.length()==128){
        printText("sha512",1);
        if(recurse_count==0){
            result = sha512BruteForce(input);
            //qDebug()<<result;
            printText(result,2);
        }
        drawCharts(result_count,"sha512",type1,10);
        result_count++;
    }
    else{
        printText("非md5",1);
        printText("非sha",1);
    }


    //无固定长度 字符种类检测
    //base64
    bool flag = 0;
    for(int i=0;i<input.length()-2;i++){
        if((input[i]>='a'&&input[i]<='z')||(input[i]>='A'&&input[i]<='Z')||(input[i]>='0'&&input[i]<='9')||input[i]=='+'||input[i]=='/'){}
        else{
            flag = 1;
            break;
        }
    }
    if(input[input.length()-2]=='='&&input[input.length()-1]=='='&&flag==0){
        printText("base64",1);\
        char* base64 = input.toLatin1().data();
        char dedata[1024];
        base64_decode(base64,(unsigned char*)dedata);
        printText(dedata,2);
        QFile file("result.txt");
        QString str = QString(QLatin1String(dedata));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"base64",type1,2);
        //drawCharts(result_count,"base64",0,2);//测试使用
        result_count++;
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1,2);
    }
    else if(((input[input.length()-2]>='a'&&input[input.length()-2]<='z')||(input[input.length()-2]>='A'&&input[input.length()-2]<='Z')||(input[input.length()-2]>='0'&&input[input.length()-2]<='9'))&&input[input.length()-1]=='='&&flag==0){
        printText("base64",1);
        char* base64 = input.toLatin1().data();
        char dedata[1024];
        base64_decode(base64,(unsigned char*)dedata);
        printText(dedata,2);
        QFile file("result.txt");
        QString str = QString(QLatin1String(dedata));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"base64",type1,2);
        //drawCharts(result_count,"base64",0,2);//测试使用
        result_count++;
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1,2);
    }
    else if(((input[input.length()-2]>='a'&&input[input.length()-2]<='z')||(input[input.length()-2]>='A'&&input[input.length()-2]<='Z')||(input[input.length()-2]>='0'&&input[input.length()-2]<='9'))&&((input[input.length()-1]>='a'&&input[input.length()-1]<='z')||(input[input.length()-1]>='A'&&input[input.length()-1]<='Z')||(input[input.length()-1]>='0'&&input[input.length()-1]<='9'))&&flag==0){
        printText("base64",1);
        char* base64 = input.toLatin1().data();
        char dedata[1024];
        base64_decode(base64,(unsigned char*)dedata);
        printText(dedata,2);
        QFile file("result.txt");
        QString str = QString(QLatin1String(dedata));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"base64",type1,2);
        //drawCharts(result_count,"base64",0,2);//测试使用
        result_count++;
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1,2);
    }
    else{
        printText("非base64",1);
    }

    //base32
    int i;
    for(i=0;i<input.length();i++){
        if((input[i]>='A'&&input[i]<='Z')||(input[i]>='2'&&input[i]<='7')){}
        else{
            break;
        }
    }
    //结尾只可能有一三四六个=
    if(i==input.length()-6||i==input.length()-4||i==input.length()-3||i==input.length()-1){
        flag = 0;
        for(;i<input.length();i++){
            if(input[i]!='='){
                flag = 1;
                break;
            }
        }
        //qDebug()<<"flag"<<flag;
        if(flag==0){
            printText("base32",1);
            char* base32 = input.toLatin1().data();
            char dedata[1024];
            base32_decode(base32,(unsigned char*)dedata);
            printText(dedata,2);
            QFile file("result.txt");
            QString str = QString(QLatin1String(dedata));
            if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
                qDebug()<<"Open failed"<< endl;
            }
            QTextStream out(&file);
            str = str + "\r\n";
            out<<str;
            file.close();
            drawCharts(result_count,"base32",type1,3);
            result_count++;
            staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1,3);
        }
        else{
            printText("非base32",1);
        }
    }
    else if(i==input.length()){
        printText("base32",1);
        char* base32 = input.toLatin1().data();
        char dedata[1024];
        base32_decode(base32,(unsigned char*)dedata);
        printText(dedata,2);
        QFile file("result.txt");
        QString str = QString(QLatin1String(dedata));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"base32",type1,3);
        result_count++;
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1,3);
    }
    else{
        printText("非base32",1);
    }

    //base16
    flag = 0;
    for(int i=0;i<input.length();i++){
        if((input[i]>='0'&&input[i]<='9')||(input[i]>='A'&&input[i]<='F')){}
        else{
            flag = 1;
            break;
        }
    }
    if(flag==0){
        printText("base16",1);
        char* base16 = input.toLatin1().data();
        char dedata[1024];
        base16_decode(base16,(unsigned char*)dedata);
        printText(dedata,2);
        QFile file("result.txt");
        QString str = QString(QLatin1String(dedata));
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"base16",type1,4);
        result_count++;
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1,4);
    }
    else{
        printText("非base16",1);
    }

    //unicode
    int count = 0;
    for(int i=0;i<input.length();i++){
        if((input[i]=='\\'&&(input[i+1]=='u'||input[i+1]=='U'))||(input[i]=='&'&&input[i+1]=='#')){
            count++;
        }
    }
    if(count>=input.length()/8.0){
        printText("unicode",1);
        result = decrypt12(input);
        QFile file("result.txt");
        QString str = result;
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"unicode",type1,12);
        result_count++;
        staticsAnalysis(result,recurse_count+1,12);
    }
    else{
        printText("非Unicode",1);
    }
    //qDebug()<<result;

    //atbash
    flag = 0;
    for(int i=0;i<input.length();i++){
        if((input[i]>='a'&&input[i]<='z')||(input[i]>='A'&&input[i]<='Z')){
            flag = 1;
            break;
        }
    }
    if(flag==1){
        printText("atbash",1);
        result = decrypt3(input);
        printText(result,2);
        drawCharts(result_count,"atbash",type1,15);
        result_count++;
        staticsAnalysis(result,recurse_count+1,15);
    }
    else{
        printText("非atbash",1);
    }

    //jsfuck
    flag = 0;
    for(int i=0;i<input.length();i++){
        if(input[i]=='('||input[i]==')'||input[i]=='+'||input[i]=='['||input[i]==']'||input[i]=='!'){}
        else{
            flag = 1;
            break;
        }
    }
    if(flag==0){
        printText("jsfuck",1);
        drawCharts(result_count,"Jsfuck",type1,16);
        result_count++;
    }
    else{
        printText("非jsfuck",1);
    }

    //brainfuck
    flag = 0;
    for(int i=0;i<input.length();i++){
        if(input[i]=='>'||input[i]=='<'||input[i]=='+'||input[i]=='-'||input[i]=='.'||input[i]==','||input[i]=='['||input[i]==']'||input[i]==' '){}
        else{
            flag = 1;
            break;
        }
    }
    if(flag==0){
        printText("brainfuck",1);
        requestBrainfuck(input);
        drawCharts(result_count,"Brainfuck",type1,17);
        result_count++;
        result = "";
        QString res = ui->textEdit_2->toPlainText();
        for(int i=res.length()-1;i>=0;i--){
            if(res[i-1]!='\n') result = res[i] + result;
        }
        //qDebug()<<result;
        staticsAnalysis(result,recurse_count+1,17);
    }
    else{
        printText("非brainfuck",1);
    }

    //urlencode
    flag = 0;
    for(int i=0;i<input.length();i++){
        if(input[i]=='%'&&((input[i+1]>='A'&&input[i+1]<='Z')||(input[i+1]>='0'&&input[i+1]<='9'))&&((input[i+2]>='A'&&input[i+2]<='Z')||(input[i+2]>='0'&&input[i+2]<='9'))){
            flag = 1;
        }
    }
    if(flag==1){
        printText("Urlencode",1);
        char* url = input.toLatin1().data();
        result = deescapeURL(url);
        printText(result,2);
        QFile file("result.txt");
        QString str = result;
        if(!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
            qDebug()<<"Open failed"<< endl;
        }
        QTextStream out(&file);
        str = str + "\r\n";
        out<<str;
        file.close();
        drawCharts(result_count,"urlencode",type1,11);
        result_count++;
        staticsAnalysis(result,recurse_count+1,11);
    }
    else{
        printText("非Urlencode",1);
    }
    //qDebug()<<result;

    //vigenere
    //fence cipher
    printText("fence",1);
    result = zhalan(input.toLatin1().data());
    //qDebug()<<result;
    QFile file("fence.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text| QIODevice::Append)){
        qDebug()<<"Open failed"<< endl;
    }
    QTextStream out(&file);
    out<<result;
    file.close();
    drawCharts(result_count,"fence",type1,19);
    result_count++;


    //polybius
    flag = 0;
    for(int i=0;i<input.length();i++){
        if(input[i]>='1'&&input[i]<='5'||input[i]==' '){}
        else{
            flag = 1;
            break;
        }
    }
    if(flag==0){
        char* in = input.toLatin1().data();
        result = encrypt5(in);
        printText("polybius",1);
        printText(result,2);
        drawCharts(result_count,"polybius",type1,18);
        result_count++;
    }
    else{
        printText("非polybius",1);
    }

    //ceasar
    flag = 0;
    for(int i=0;i<input.length();i++){
        if((input[i]>='a'&&input[i]<='z')||(input[i]>='A'&&input[i]<='Z')){
            flag = 1;
            break;
        }
    }
    if(flag==1){
        printText("Ceasar cipher",1);
        for(int i=1;i<26;i++){
            result = ceasar_cipher(input,i);
            printText(result,2);
            //qDebug()<<result;
        }
        drawCharts(result_count,"ceasar",type1,1);
        result_count++;
    }
    else{
        printText("非ceasar",1);
    }

    //XXencode 字母数字+-
    flag = 0;
    for(int i=0;i<input.length();i++){
        if((input[i]>='A'&&input[i]<='Z')||(input[i]>='a'&&input[i]<='z')||(input[i]>='0'&&input[i]<-'9')||input[i]=='+'||input[i]=='-'){}
        else{
            flag = 1;
            break;
        }
    }
    if(flag==1){
        printText("非XXencode",1);
    }
    else{
        printText("尝试XXencode",1);
        drawCharts(result_count,"XXencode",type1,13);
        result_count++;
    }

    //UUencode 可打印字符
    printText("尝试UUencode",1);
    drawCharts(result_count,"UUencode",type1,14);
    result_count++;
}

//按下按钮
void MainWindow::on_pushButton_clicked()
{
    QFile file("C:\\Users\\houwd\\Desktop\\programming\\Qt5\\classic_cryptography\\raw_json.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"raw json reading failed";
        return ;
    }
    QTextStream in(&file);
    QString raw = in.readAll();
    file.close();
    QFile file1("C:\\Users\\houwd\\Desktop\\programming\\Qt5\\classic_cryptography\\testEcharts.json");
    if(!file1.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug()<<"json initializing failed"<<endl;
        return ;
    }
    QTextStream out(&file1);
    out<<raw;
    file1.close();
    //清空输出
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    //忽略空输入
    if(ui->lineEdit->text().length()==0) return;
    //quipquip
    if(requestQuipquip())   qDebug()<<"quipquip analysis"<<endl;
    //特征检测
    QString input = ui->lineEdit->text();
    staticsAnalysis(input,0,0);

}
