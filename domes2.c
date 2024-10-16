#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"domes.h"

bucket* inserttransaction(bucket* Hashtable,walletbucket** walletIDHashtable,char *argtransactionID,char* argsender,char* argreceiver,char* argsum,char* argdate,char* argtime,int bucketSize,int hashtablesize){
	int i,h,j,k;
	bucket *temp=NULL;
	transactionlist* temp2=NULL;
	i=1;
	walletIDlist* temp3=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
	if(temp3==NULL){
		printf("user does not exist\n");
		return Hashtable;
	}
	if(temp3->walletptr->ypoloipo<atoi(argsum)){
		printf("sender doesn't have the money for the transaction\n");
		return Hashtable;
	}	
	j=(bucketSize-4)/sizeof(userIDlist);  //find how many potition the table inside the bucket has
	h=Hashfunctionstring(argsender );     //find position in hashtable
	h=h%hashtablesize;
	if(Hashtable[h].bt==NULL){
		Hashtable[h].bt=(userIDlist*)malloc(j*sizeof(userIDlist));     //make new bucket
		Hashtable[h].bt[0].userID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
		Hashtable[h].bt[0].transactionlistptr=(transactionlist*)malloc(sizeof(transactionlist));
		Hashtable[h].bt[0].transactionlistptr->transactionptr=(transaction*)malloc(sizeof(transaction));
		Hashtable[h].bt[0].transactionlistptr->transactionptr->transactionID=(char*)malloc(strlen(argtransactionID)*sizeof(char));
		strcpy(Hashtable[h].bt[0].transactionlistptr->transactionptr->transactionID,argtransactionID);
		Hashtable[h].bt[0].transactionlistptr->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
		Hashtable[h].bt[0].transactionlistptr->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
		if(Hashtable[h].bt[0].transactionlistptr->transactionptr->senderID==NULL){
			*walletIDHashtable=insertwallet(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
			Hashtable[h].bt[0].transactionlistptr->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
		}
		if(Hashtable[h].bt[0].transactionlistptr->transactionptr->receiverID==NULL){      //make new wallet
			*walletIDHashtable=insertwallet(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
			Hashtable[h].bt[0].transactionlistptr->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
		}
		Hashtable[h].bt[0].transactionlistptr->transactionptr->transactionsum=atoi(argsum);
		Hashtable[h].bt[0].transactionlistptr->transactionptr->date=(char*)malloc(strlen(argdate)*sizeof(char));
		strcpy(Hashtable[h].bt[0].transactionlistptr->transactionptr->date,argdate);
		Hashtable[h].bt[0].transactionlistptr->transactionptr->time=(char*)malloc(strlen(argtime)*sizeof(char));
		strcpy(Hashtable[h].bt[0].transactionlistptr->transactionptr->time,argtime);
	}
	else{
		do{
			if(temp==NULL){
				temp=&Hashtable[h];
			}
			else{
				temp=temp->next;
			}
			for(k=0;k<j;k++){  //search for empty position in bucket
				if(temp->bt[k].userID==NULL){
					temp->bt[k].userID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
					temp->bt[k].transactionlistptr=(transactionlist*)malloc(sizeof(transactionlist));
					temp->bt[k].transactionlistptr->transactionptr=(transaction*)malloc(sizeof(transaction));
					temp->bt[0].transactionlistptr->transactionptr->transactionID=(char*)malloc(strlen(argtransactionID)*sizeof(char));
					strcpy(temp->bt[0].transactionlistptr->transactionptr->transactionID,argtransactionID);
					temp->bt[k].transactionlistptr->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
					temp->bt[k].transactionlistptr->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
					if(temp->bt[k].transactionlistptr->transactionptr->senderID==NULL){
						*walletIDHashtable=insertwallet(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
						temp->bt[k].transactionlistptr->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
					}
					if(temp->bt[k].transactionlistptr->transactionptr->receiverID==NULL){
						*walletIDHashtable=insertwallet(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
						temp->bt[k].transactionlistptr->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
					}
					temp->bt[k].transactionlistptr->transactionptr->transactionsum=atoi(argsum);
					temp->bt[k].transactionlistptr->transactionptr->date=(char*)malloc(strlen(argdate)*sizeof(char));
					strcpy(temp->bt[k].transactionlistptr->transactionptr->date,argdate);
					temp->bt[k].transactionlistptr->transactionptr->time=(char*)malloc(strlen(argtime)*sizeof(char));
					strcpy(temp->bt[k].transactionlistptr->transactionptr->time,argtime);
					return Hashtable;
				}
				else if(strcmp(temp->bt[k].userID->walletID,argsender)==0){  //if user already has made transactions
					do{	
						if(temp2==NULL){				
							temp2=temp->bt[k].transactionlistptr;
						}
						else{
							temp2=temp2->next;
						}
						if(strcmp(temp2->transactionptr->transactionID,argtransactionID)==0){
							printf("TransactionId already exists\n");
							return Hashtable;
						}
					}while(temp2->next!=NULL);
					temp2->next=(transactionlist*)malloc(sizeof(transactionlist));  //insert new transaction
					temp2->next->transactionptr=(transaction*)malloc(sizeof(transaction));
					temp2->next->transactionptr->transactionID=(char*)malloc(strlen(argtransactionID)*sizeof(char));
					strcpy(temp2->next->transactionptr->transactionID,argtransactionID);
					temp2->next->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
					temp2->next->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
					if(temp2->next->transactionptr->senderID==NULL){
						*walletIDHashtable=insertwallet(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
						temp2->next->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
					}
					if(temp2->next->transactionptr->receiverID==NULL){
						*walletIDHashtable=insertwallet(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
						temp2->next->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
					}
					temp2->next->transactionptr->transactionsum=atoi(argsum);
					temp2->next->transactionptr->date=(char*)malloc(strlen(argdate)*sizeof(char));
					strcpy(temp2->next->transactionptr->date,argdate);
					temp2->next->transactionptr->time=(char*)malloc(strlen(argtime)*sizeof(char));
					strcpy(temp2->next->transactionptr->time,argtime);
					return Hashtable;
				}
			}
		}while(temp->next!=NULL);  //bucket overflows
		temp->next=(bucket*)malloc(sizeof(bucket));    //so make new bucket
		temp->next->bt=(userIDlist*)malloc(j*sizeof(userIDlist));
		temp->next->bt[0].userID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
		temp->next->bt[0].transactionlistptr=(transactionlist*)malloc(sizeof(transactionlist));
		temp->next->bt[0].transactionlistptr->transactionptr=(transaction*)malloc(sizeof(transaction));
		temp->next->bt[0].transactionlistptr->transactionptr->transactionID=(char*)malloc(strlen(argtransactionID)*sizeof(char));
		strcpy(temp->next->bt[0].transactionlistptr->transactionptr->transactionID,argtransactionID);
		temp->next->bt[0].transactionlistptr->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
		temp->next->bt[0].transactionlistptr->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
		if(temp->next->bt[0].transactionlistptr->transactionptr->senderID==NULL){
			*walletIDHashtable=insertwallet(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
			temp->next->bt[0].transactionlistptr->transactionptr->senderID=finduserID(*walletIDHashtable,argsender,bucketSize,walletIDHashtablesize);
		}
		if(temp->next->bt[0].transactionlistptr->transactionptr->receiverID==NULL){
			*walletIDHashtable=insertwallet(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
			temp->next->bt[0].transactionlistptr->transactionptr->receiverID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
		}
		temp->next->bt[0].transactionlistptr->transactionptr->transactionsum=atoi(argsum);
		temp->next->bt[0].transactionlistptr->transactionptr->date=(char*)malloc(strlen(argdate)*sizeof(char));
		strcpy(temp->next->bt[0].transactionlistptr->transactionptr->date,argdate);
		temp->next->bt[0].transactionlistptr->transactionptr->time=(char*)malloc(strlen(argtime)*sizeof(char));
		strcpy(temp->next->bt[0].transactionlistptr->transactionptr->time,argtime);
	}
	return Hashtable;
}

int datetranslate(char* date){  //translate date to int
	int i=0;
	i=(date[1]-48)+10*(date[0]-48)+100*(date[4]-48)+1000*(date[3]-48)+10000*(date[9]-48)+100000*(date[8]-48)+1000000*(date[7]-48)+10000000*(date[6]-48);
	return i;
}

int timetranslate(char* time){
	int i=0;
	i=(time[1]-48)+10*(time[0]-48)+100*(time[4]-48)+1000*(time[3]-48);
	return i;
}


transaction* findtransaction(bucket* Hashtable,char* transactionID,char* userID,int bucketSize,int hashtablesize){
	int i,h,j,k;
	bucket *temp=NULL;
	transactionlist *temp2;
	temp2=NULL;
	j=(bucketSize-4)/sizeof(userIDlist);
	h=Hashfunctionstring(userID);
	h=h%hashtablesize;
	if(Hashtable[h].bt!=NULL){
		do{	
			if(temp==NULL){
				temp=&Hashtable[h];
			}
			else{
			temp=temp->next;
			}
				for(k=0;k<j;k++){
				if(temp->bt[k].transactionlistptr!=NULL){
					if(strcmp(temp->bt[k].userID->walletID,userID)==0){
						if(temp->bt[k].transactionlistptr->transactionptr!=NULL){
							do{
								if(temp2==NULL){				
									temp2=temp->bt[k].transactionlistptr;
								}
								else{
									temp2=temp2->next;
								}
								if(strcmp(temp2->transactionptr->transactionID,transactionID)==0){
									return temp2->transactionptr;
								}
								//printf("*TransactionID:%s senderID:%s receiverID:%s transactionsum:%d  date:%s time:%s.",temp2->transactionptr->transactionID,temp2->transactionptr->senderID->walletID,temp2->transactionptr->receiverID->walletID,temp2->transactionptr->transactionsum,temp2->transactionptr->date,temp2->transactionptr->time);
							}while(temp2->next!=NULL);	
						}
					}	
				}
				temp2=NULL;
			}
		}while(temp->next!=NULL);
		temp=NULL;
	}
	printf("Did not find transaction\n");
	return NULL;
}

bucket* receivertransaction(bucket* Hashtable,bucket* senderHashtable,walletbucket** walletIDHashtable,char* argreceiver,char* senderID,char *argtransactionID,int bucketSize,int hashtablesize){  //same as sender but now search for pointer to transaction
	int i,h,j,k;
	bucket *temp=NULL;
	transactionlist* temp2=NULL;
	i=1;
	transaction* temp3=findtransaction(senderHashtable,argtransactionID,senderID,bucketSize,hashtablesize);
	if(temp3==NULL){
		return Hashtable;
	}	
	j=(bucketSize-4)/sizeof(userIDlist);
	h=Hashfunctionstring(argreceiver);
	h=h%hashtablesize;
	if(Hashtable[h].bt==NULL){
		Hashtable[h].bt=(userIDlist*)malloc(j*sizeof(userIDlist));
		Hashtable[h].bt[0].userID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
		Hashtable[h].bt[0].transactionlistptr=(transactionlist*)malloc(sizeof(transactionlist));
		Hashtable[h].bt[0].transactionlistptr->transactionptr=findtransaction(senderHashtable,argtransactionID,senderID,bucketSize,hashtablesize);
	}
	else{
		do{
			if(temp==NULL){
				temp=&Hashtable[h];
			}
			else{
				temp=temp->next;
			}
			for(k=0;k<j;k++){
				if(temp->bt[k].userID==NULL){
					temp->bt[k].userID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
					temp->bt[k].transactionlistptr=(transactionlist*)malloc(sizeof(transactionlist));
					temp->bt[k].transactionlistptr->transactionptr=findtransaction(senderHashtable,argtransactionID,senderID,bucketSize,hashtablesize);
					return Hashtable;
				}
				else if(strcmp(temp->bt[k].userID->walletID,argreceiver)==0){
					do{	
						if(temp2==NULL){				
							temp2=temp->bt[k].transactionlistptr;
						}
						else{
							temp2=temp2->next;
						}
						if(strcmp(temp2->transactionptr->transactionID,argtransactionID)==0){
							printf("TransactionId already exists\n");
							return Hashtable;
						}
					}while(temp2->next!=NULL);
					temp2->next=(transactionlist*)malloc(sizeof(transactionlist));
					temp2->next->transactionptr=findtransaction(senderHashtable,argtransactionID,senderID,bucketSize,hashtablesize);
					return Hashtable;
				}
			}
		}while(temp->next!=NULL);
		temp->next=(bucket*)malloc(sizeof(bucket));
		temp->next->bt=(userIDlist*)malloc(j*sizeof(userIDlist));
		temp->next->bt[0].userID=finduserID(*walletIDHashtable,argreceiver,bucketSize,walletIDHashtablesize);
		temp->next->bt[0].transactionlistptr=(transactionlist*)malloc(sizeof(transactionlist));
		temp->next->bt[0].transactionlistptr->transactionptr=findtransaction(senderHashtable,argtransactionID,senderID,bucketSize,hashtablesize);
	}
	return Hashtable;
}

bitcoinbucket* dividebitcoins(bitcoinbucket* bitcoinIDHashtable,bucket* Hashtable,walletbucket** walletIDHashtable,char* transactionID,char* userID,int bucketSize,int hashtablesize){   //make the transaction in bitcoin tree
	int i,h,j,k,money;
	bitcoinbucket *temp=NULL;
	bitcoin* temp2=NULL;
	transaction* temp3=NULL;
	bitcoinbalance* temp4=NULL;
	i=1;
	temp3=findtransaction(Hashtable,transactionID,userID,bucketSize,hashtablesize);
	if(temp3==NULL){
		return bitcoinIDHashtable;
	}
	money=temp3->transactionsum;
	j=(bucketSize-4)/sizeof(bitcoinlist);
	temp4=temp3->senderID->walletptr->bitcoins;
	while(money>0){	
		if(temp4==NULL){
			printf("something went wrong\n");
			break;
		}
		h=Hashfunction(temp4->bitcoinID->bitcoinID);
		h=h%hashtablesize;
		if(bitcoinIDHashtable[h].bbt!=NULL){
			do{
				if(temp==NULL){
					temp=&bitcoinIDHashtable[h];
				}
				else{
					temp=temp->next;
				}
				for(k=0;k<j;k++){
					if(temp->bbt[k].bitcoinptr!=NULL){
						if(temp->bbt[k].bitcoinID==temp4->bitcoinID->bitcoinID){
							i=0;
							while(1){							
								temp2=findbitcoinnode(temp->bbt[k].bitcoinptr,userID);
								if(temp2==NULL || money<=0){
									break;
								}
								temp2->bitcointransactionptr=findtransaction(Hashtable,transactionID,userID,bucketSize,hashtablesize);
								money=money-temp2->ypoloipo;
								if(temp2->ypoloipo>=temp3->transactionsum){
									temp2->left=(bitcoin*)malloc(sizeof(bitcoin));
									temp2->left->userID=temp3->receiverID;
									temp2->left->ypoloipo=temp3->transactionsum;
									temp2->right=(bitcoin*)malloc(sizeof(bitcoin));
									temp2->right->userID=temp3->receiverID;
									temp2->right->ypoloipo=temp2->ypoloipo-temp3->transactionsum;
								}
								else{
									temp2->left=(bitcoin*)malloc(sizeof(bitcoin));
									temp2->left->userID=temp3->receiverID;
									temp2->left->ypoloipo=temp2->ypoloipo;
									temp2->right=(bitcoin*)malloc(sizeof(bitcoin));
									temp2->right->userID=temp3->receiverID;
									temp2->right->ypoloipo=0;
									*walletIDHashtable=deletebitcoin(*walletIDHashtable,temp3->senderID->walletID,temp->bbt[k].bitcoinID,bucketSize,walletIDHashtablesize);
								}
								*walletIDHashtable=insertbitcoin(*walletIDHashtable,bitcoinIDHashtable,temp3->receiverID->walletID,temp->bbt[k].bitcoinID,bucketSize,walletIDHashtablesize);
							}
						}
					}
				}
			}while(temp->next!=NULL && i);
			i=1;
		}
		temp=NULL;
		temp4=temp4->next;
	}
	return bitcoinIDHashtable;
}

bitcoin* findbitcoinnode(bitcoin* bitcoinnode,char* userID){
	bitcoin* temp=NULL;
	if(bitcoinnode->left==NULL){
		if(strcmp(bitcoinnode->userID->walletID,userID)==0 && bitcoinnode->ypoloipo>0){
			return bitcoinnode;
		}
		else{
			return NULL;
		}
	}
	else{
		temp=findbitcoinnode(bitcoinnode->left,userID);
	}
	if(temp==NULL){
		if(bitcoinnode->right==NULL){
			if(strcmp(bitcoinnode->userID->walletID,userID)==0 && bitcoinnode->ypoloipo>0){
				return bitcoinnode;
			}
			else{
				return NULL;
			}
		}	
	}
	else{
		temp=findbitcoinnode(bitcoinnode->right,userID);
	}
	return temp;
}

int findtransactions(bucket* Hashtable,char* userID,char* date,char* ttime,char* date2,char* ttime2,int bucketSize,int hashtablesize){
	int i,h,j,k;
	bucket *temp=NULL;
	transactionlist *temp2;
	temp2=NULL;
	j=(bucketSize-4)/sizeof(userIDlist);
	h=Hashfunctionstring(userID);
	h=h%hashtablesize;
	i=0;printf("%d\n",h);
	if(Hashtable[h].bt!=NULL){
		do{	
			if(temp==NULL){
				temp=&Hashtable[h];
			}
			else{
				temp=temp->next;
			}
				for(k=0;k<j;k++){
				if(temp->bt[k].transactionlistptr!=NULL){
					if(strcmp(temp->bt[k].userID->walletID,userID)==0){
						if(temp->bt[k].transactionlistptr->transactionptr!=NULL){
							do{
								if(temp2==NULL){				
									temp2=temp->bt[k].transactionlistptr;
								}
								else{
									temp2=temp2->next;
								}
									i=i+temp2->transactionptr->transactionsum;
								//printf("*TransactionID:%s senderID:%s receiverID:%s transactionsum:%d  date:%s time:%s.",temp2->transactionptr->transactionID,temp2->transactionptr->senderID->walletID,temp2->transactionptr->receiverID->walletID,temp2->transactionptr->transactionsum,temp2->transactionptr->date,temp2->transactionptr->time);
							}while(temp2->next!=NULL);	
						}
					}	
				}
				temp2=NULL;
			}
			if(i>0)break;
		}while(temp->next!=NULL);
		temp=NULL;
	}
	return i;
}

int findnumberoftransactions(bitcoin* bitcoinnode){
	int temp=0;
	if(bitcoinnode->left==NULL){
		return temp;
	}
	else{
		temp=findnumberoftransactions(bitcoinnode->left);
	}
	if(bitcoinnode->right==NULL){
		return temp;
	}	
	else{
		temp=findnumberoftransactions(bitcoinnode->right);
	}
	return temp+1;
}

int findunspentsum(bitcoin* bitcoinnode){
	int temp=0;
	if(bitcoinnode->right==NULL){
		return bitcoinnode->ypoloipo;
	}	
	else{
		temp=findnumberoftransactions(bitcoinnode->right);
	}
	return temp;
}

void readalltransactions(bitcoin* bitcoinnode){
	int temp=0;
	if(bitcoinnode->left==NULL){
		if(bitcoinnode->bitcointransactionptr!=NULL)
		printf("*TransactionID:%s senderID:%s receiverID:%s transactionsum:%d  date:%s time:%s.\n",bitcoinnode->bitcointransactionptr->transactionID,bitcoinnode->bitcointransactionptr->senderID->walletID,bitcoinnode->bitcointransactionptr->receiverID->walletID,bitcoinnode->bitcointransactionptr->transactionsum,bitcoinnode->bitcointransactionptr->date,bitcoinnode->bitcointransactionptr->time);
	}
	else{
		temp=findnumberoftransactions(bitcoinnode->left);
	}
	if(bitcoinnode->right==NULL){
		if(bitcoinnode->bitcointransactionptr!=NULL)
		printf("*TransactionID:%s senderID:%s receiverID:%s transactionsum:%d  date:%s time:%s.\n",bitcoinnode->bitcointransactionptr->transactionID,bitcoinnode->bitcointransactionptr->senderID->walletID,bitcoinnode->bitcointransactionptr->receiverID->walletID,bitcoinnode->bitcointransactionptr->transactionsum,bitcoinnode->bitcointransactionptr->date,bitcoinnode->bitcointransactionptr->time);
	}	
	else{
		temp=findnumberoftransactions(bitcoinnode->right);
	}
	if(bitcoinnode->bitcointransactionptr!=NULL)
	printf("*TransactionID:%s senderID:%s receiverID:%s transactionsum:%d  date:%s time:%s.\n",bitcoinnode->bitcointransactionptr->transactionID,bitcoinnode->bitcointransactionptr->senderID->walletID,bitcoinnode->bitcointransactionptr->receiverID->walletID,bitcoinnode->bitcointransactionptr->transactionsum,bitcoinnode->bitcointransactionptr->date,bitcoinnode->bitcointransactionptr->time);
}
