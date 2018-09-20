#include "cipher.h"

cipher::cipher()
{

}

void cipher::add_weights(int num){
    weights += num;
    //counter += num;
}
int cipher::get_weights(void){
    return weights;
}
void cipher::set_cipher_type(QString ch){
    cipher_type =  ch;
}
