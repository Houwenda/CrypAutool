#ifndef CIPHER_H
#define CIPHER_H
#include <regex>
#include <QString>

class cipher
{
private: QString cipher_type;
private: int weights = 0;//权重
public: void add_weights(int num);
public: int get_weights(void);
public: void set_cipher_type(QString ch);
public:
    cipher();
};

#endif // CIPHER_H
