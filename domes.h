#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define bitcoinIDHashtablesize 10
#define walletIDHashtablesize 10

typedef struct bitcoinlist bitcoinlist;

typedef struct bitcoinbalance{
	struct bitcoinlist* bitcoinID;
	struct bitcoinbalance* next;
}bitcoinbalance;

typedef struct wallet{
	int ypoloipo;  
	bitcoinbalance *bitcoins; 
}wallet;

typedef struct walletIDlist{
	char *walletID;
	wallet *walletptr;
}walletIDlist;

typedef struct walletbucket{
	walletIDlist* wbt;                      //wallet bucket table
	struct walletbucket* next;
}walletbucket;

typedef struct{
	char* transactionID;
	walletIDlist* senderID;
	walletIDlist* receiverID;
	int transactionsum;
	char* date;
	char* time;
}transaction;

typedef struct transactionlist{
	transaction* transactionptr;
	struct transactionlist* next;
}transactionlist;							

typedef struct{                     //userID table inside bucket
	walletIDlist* userID;
	transactionlist *transactionlistptr;
}userIDlist;

typedef struct bitcoin{                       //tree node
	walletIDlist* userID;
	int ypoloipo;
	struct bitcoin* right;
	struct bitcoin* left;
	transaction* bitcointransactionptr;
}bitcoin;

struct bitcoinlist{
	int bitcoinID;
	bitcoin *bitcoinptr;
};

typedef struct bitcoinbucket{
	bitcoinlist *bbt;                          //bitcoin bucket table
	struct bitcoinbucket *next;
}bitcoinbucket;

typedef struct bucket{
	userIDlist* bt;
	struct bucket* next;
}bucket;

int Hashfunction(int);
int Hashfunctionstring(char* );
walletbucket* insertwallet(walletbucket* ,char *,int,int);
walletbucket* insertbitcoin(walletbucket*,bitcoinbucket *,char *,int,int,int);
walletbucket* increaseypoloipo(walletbucket*,char *,int,int,int);
bitcoinbucket *newbitcoinID(bitcoinbucket *,walletbucket*,char* ,int ,int ,int, int );
walletIDlist* finduserID(walletbucket* ,char* ,int , int);
void* readwalletHashtable(walletbucket* ,int , int );
bitcoinlist* findbitcoinID(bitcoinbucket* ,int ,int , int );
void* readbitcoinHashtable(bitcoinbucket* ,int , int);
walletbucket* deletebitcoin(walletbucket*,char *,int,int, int);
int datetranslate(char*);
int timetranslate(char*);
bucket* inserttransaction(bucket*,walletbucket**,char *,char*,char*,char*,char*,char*,int,int);
transaction* findtransaction(bucket*,char*,char*,int,int);
bucket* receivertransaction(bucket*,bucket*,walletbucket**,char*,char*,char *,int,int);
bitcoin* findbitcoinnode(bitcoin*,char*);
bitcoinbucket* dividebitcoins(bitcoinbucket*,bucket*,walletbucket**,char*,char*,int,int);
int findtransactions(bucket*,char*,char*,char*,char*,char*,int,int);
int findnumberoftransactions(bitcoin*);
int findunspentsum(bitcoin*);
void readalltransactions(bitcoin*);
