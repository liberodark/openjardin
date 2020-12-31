#ifndef UTIL_H
#define UTIL_H
#include <QString>

class util
{
public:
    util();
    static QString apos(const QString texte);   //remplacement des apostrophes pour requetes sql
    static QString afficheZeros(QString texte); // remplissage des zéros en fin de chaine
    static QString getLocale();
};

#endif // UTILITAIRES_H
