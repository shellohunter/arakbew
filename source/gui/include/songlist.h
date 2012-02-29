#ifndef SONGLIST_HPP
#define SONGLIST_HPP

#include <string>


typedef enum
{
    E_MALE,
    E_FEMALE,
    E_BAND,
    E_UNKNOWN,
}eGender;


class Song
{
public:
    int index; /* mandatory */
    string name; /* mandatory */
    string url; /* mandatory */
    int category;
    int artistIndex;
    string artistName;
    string albumName;

    void toString();
    bool operator==(const Song & song);    
};


class Artist
{
public:

    int index; /* mandatory */
    string name; /* mandatory */
    eGender gender;
    string imgurl;
    
    void toString();
    bool operator==(const Artist & song);    
};


#endif /* SONGLIST_HPP */