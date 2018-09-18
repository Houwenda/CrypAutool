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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view.setParent(ui->widget);
    view.setFixedHeight(800);
    view.setFixedWidth(700);
    view.setUrl(QUrl("file:///C:/Users/houwd/Desktop/programming/Qt5/classic_cryptography/graph-webkit-dep.html"));
    view.show();    
    ui->lineEdit->setFont(QFont("微软雅黑",14));
    ui->pushButton->setFont(QFont("微软雅黑",14,QFont::Bold));
    ui->pushButton->setStyleSheet("color:green");

    //ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setTextBackgroundColor(QColor(Qt::black));
    ui->textEdit->setFont(QFont("微软雅黑",10));
    ui->textEdit_2->setFont(QFont("微软雅黑",10));
    //ui->textEdit->append(QString("text"));
    //ui->textEdit->append(QString("test"));
    ui->textEdit->document()->setMaximumBlockCount(10);//最多显示10行
    ui->textEdit_2->document()->setMaximumBlockCount(200);
    //设定定时器，每秒触发一次事件，调整TextEdit光标位置
    QTimer *CurrentTime = new QTimer(this);
    CurrentTime->start(1000);
    connect(CurrentTime,&QTimer::timeout,[=](){
            //保持编辑器光标在最后一行
            QTextCursor cursor=ui->textEdit->textCursor();
            cursor.movePosition(QTextCursor::End);
            ui->textEdit->setTextCursor(cursor);
            //ui->textEdit->append(QString("......"));
    });
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
}

MainWindow::~MainWindow()
{
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
//槽函数 接收服务器消息
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

void MainWindow::drawCharts(int recurse_count,int type){

}


//特征识别
void MainWindow::staticsAnalysis(QString input,int recurse_count){

    if(recurse_count==3) return;
    else{}

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
    }
    //md5(32)
    else if(input.length()==32){
        printText("md5",1);
    }
    //md5(16)
    else if(input.length()==16){
        printText("md5",1);
    }
    //sha1
    else if(input.length()==40){
        printText("sha1",1);
    }
    //sha244
    else if(input.length()==56){
        printText("sha244",1);
    }
    //sha384
    else if(input.length()==96){
        printText("sha384",1);
    }
    //sha512
    else if(input.length()==128){
        printText("sha512",1);
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
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1);
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
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1);
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
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1);
    }
    else{
        printText("非base64",1);
    }

    //base32
    int i;
    for(i=0;i<input.length()-1;i++){
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
            staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1);
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
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1);
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
        staticsAnalysis(QString(QLatin1String(dedata)),recurse_count+1);
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
    if(count>=input.length()/7){
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
        staticsAnalysis(result,recurse_count+1);
    }
    else{
        printText("非Unicode",1);
    }
    //qDebug()<<result;

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
    }
    else{
        printText("非jsfuck",1);
    }

    //brainfuck
    flag = 0;
    for(int i=0;i<input.length();i++){
        if(input[i]=='>'||input[i]=='<'||input[i]=='+'||input[i]=='-'||input[i]=='.'||input[i]==','||input[i]=='['||input[i]==']'){}
        else{
            flag = 1;
            break;
        }
    }
    if(flag==0){
        printText("brainfuck",1);
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
        staticsAnalysis(result,recurse_count+1);
    }
    else{
        printText("非Urlencode",1);
    }
    //qDebug()<<result;

    //vigenere


    //ceasar
    printText("Ceasar cipher",1);
    for(int i=1;i<26;i++){
        result = ceasar_cipher(input,i);
        printText(result,2);
        //qDebug()<<result;
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
    }

    //UUencode 可打印字符
    printText("尝试UUencode",1);


}

//按下按钮
void MainWindow::on_pushButton_clicked()
{
    //忽略空输入
    if(ui->lineEdit->text().length()==0) return;
    //清空输出
    ui->textEdit->clear();
    ui->textEdit_2->clear();
    //quipquip
    //if(requestQuipquip())   qDebug()<<"quipquip analysis"<<endl;
    //特征检测
    QString input = ui->lineEdit->text();
    staticsAnalysis(input,0);

}
