/*
 * makrau.h
 *
 *  Created on: 21-04-2013
 *       Autor: Miros�aw Karda�
 */

#ifndef MAKRAU_H_
#define MAKRAU_H_

// je�li makra tego typu wyst�puj� w kilku plikach mo�na wyprowadzi� je do jednego np makrau.h
// a nast�pnie importowa� w ka�dym potrzebnym pliku nag��wkowym
// tak aby by�y tylko w jednym miejscu programu

// Makra upraszczaj�ce dost�p do port�w
// *** PORT
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** PIN
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)


#endif /* MAKRAU_H_ */
