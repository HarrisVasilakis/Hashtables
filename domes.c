#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"domes.h"

int Hashfunction(int i){
	int a=11;
	int b=23;
	long int p=  16769023;
	return (a*i+b)%p;
}

int Hashfunctionstring(char* i){
	int a=33;
	int b=5381;
	long int p=16769023;
	for(int j=0;j<strlen(i);j++){
		b = ((b*a) + i[j])%p;
	}
	return b;
}

walletbucket* insertwallet(walletbucket* walletIDHashtable,char *userID,int bucketSize,int hashtablesize){
	int i,h,j,k;
	walletbucket *temp=NULL;
	i=1;
	j=(bucketSize-4)/sizeof(walletIDlist);
	h=Hashfunctionstring(userID );
	h=h%hashtablesize;
	if(walletIDHashtable[h].wbt==NULL){
		walletIDHashtable[h].wbt=(walletIDlist*)malloc(j*sizeof(walletIDlist));
		walletIDHashtable[h].wbt[0].walletID=(char*)malloc(strlen(userID)*sizeof(char));
		strcpy(walletIDHashtable[h].wbt[0].walletID,userID);
		walletIDHashtable[h].wbt[0].walletptr=(wallet*)malloc(sizeof(wallet));
		walletIDHashtable[h].wbt[0].walletptr->ypoloipo=0;
	}
	else{
		do{
			if(temp==NULL){
				temp=&walletIDHashtable[h];
			}
			else{
				temp=temp->next;
			}
			for(k=0;k<j;k++){
				if(temp->wbt[k].walletID==NULL){
					temp->wbt[k].walletID=(char*)malloc(strlen(userID)*sizeof(char)); 
					strcpy(temp->wbt[k].walletID,userID);
					temp->wbt[k].walletptr=(wallet*)malloc(sizeof(wallet));
					temp->wbt[k].walletptr->ypoloipo=0;
					return walletIDHashtable;
				}
				else if(strcmp(temp->wbt[k].walletID,userID)==0){
					perror("Username already exists");
					return walletIDHashtable;
				}
			}
		}while(temp->next!=NULL);
		
		temp->next=(walletbucket *)malloc(sizeof(walletbucket));
		temp->next->wbt=(walletIDlist*)malloc(j*sizeof(walletIDlist));
		temp->next->wbt[0].walletID=(char*)malloc(strlen(userID)*sizeof(char));
		strcpy(temp->next->wbt[0].walletID,userID);
		temp->next->wbt[0].walletptr=(wallet*)malloc(sizeof(wallet));
		temp->next->wbt[0].walletptr->ypoloipo=0;
	}
	return walletIDHashtable;
}

walletbucket* insertbitcoin(walletbucket* walletIDHashtable,bitcoinbucket *bitcoinIDHashtable,char *userID,int bitcoinID,int bucketSize, int hashtablesize){
	int i,h,j,k;
	walletbucket *temp=NULL;
	bitcoinbalance *temp2=NULL;
	h=Hashfunctionstring(userID );
	h=h%hashtablesize;
	j=(bucketSize-4)/sizeof(walletIDlist);
	do{
		if(temp==NULL){
			temp=&walletIDHashtable[h];
		}
		else{
			temp=temp->next;
		}
		for(k=0;k<j;k++){
			if(strcmp(temp->wbt[k].walletID,userID)==0){
				if(temp->wbt[k].walletptr->bitcoins==NULL){
					temp->wbt[k].walletptr->bitcoins=(bitcoinbalance*)malloc(sizeof(bitcoinbalance));
					temp->wbt[k].walletptr->bitcoins->bitcoinID=(bitcoinlist*)malloc(sizeof(bitcoinlist));
					temp->wbt[k].walletptr->bitcoins->bitcoinID=findbitcoinID(bitcoinIDHashtable,bitcoinID,bucketSize,bitcoinIDHashtablesize);
				}
				else{
					do{	
						if(temp2==NULL){				
							temp2=temp->wbt[k].walletptr->bitcoins;
						}
						else{
							temp2=temp2->next;
						}
						if(temp2->bitcoinID->bitcoinID==bitcoinID){
							return walletIDHashtable;
						}
					}while(temp2->next!=NULL);
					temp2->next=(bitcoinbalance*)malloc(sizeof(bitcoinbalance));
					temp2->next->bitcoinID=findbitcoinID(bitcoinIDHashtable,bitcoinID,bucketSize,bitcoinIDHashtablesize);
				}
				return walletIDHashtable;
			}
		}
	}while(temp->next!=NULL);
		
	return walletIDHashtable;
}

walletbucket* increaseypoloipo(walletbucket* walletIDHashtable,char *userID,int bits,int bucketSize, int hashtablesize){
	int i,h,j,k;
	walletbucket *temp=NULL;
	bitcoinbalance *temp2=NULL;
	h=Hashfunctionstring(userID );
	h=h%hashtablesize;
	j=(bucketSize-4)/sizeof(walletIDlist);
	do{
		if(temp==NULL){
			temp=&walletIDHashtable[h];
		}
		else{
			temp=temp->next;
		}
		for(k=0;k<j;k++){
			if(temp->wbt[k].walletID!=NULL){
				if(strcmp(temp->wbt[k].walletID,userID)==0){
					temp->wbt[k].walletptr->ypoloipo+=bits;
					return walletIDHashtable;	
				}
			}
		}
	}while(temp->next!=NULL);
		
	return walletIDHashtable;
}

bitcoinbucket *newbitcoinID(bitcoinbucket *bitcoinIDHashtable,walletbucket* walletIDHashtable,char* userID,int bitcoinID,int bitCoinValue,int bucketSize, int hashtablesize){
	int i,j,h,k;
	bitcoinbucket *temp=NULL;
	h=Hashfunction(bitcoinID);
	h=h%hashtablesize;
	j=(bucketSize-4)/sizeof(bitcoinlist);
	if(bitcoinIDHashtable[h].bbt==NULL){
		bitcoinIDHashtable[h].bbt=(bitcoinlist*)malloc(j*sizeof(bitcoinlist));
		bitcoinIDHashtable[h].bbt[0].bitcoinID=bitcoinID;
		bitcoinIDHashtable[h].bbt[0].bitcoinptr=(bitcoin*)malloc(sizeof(bitcoin));
		bitcoinIDHashtable[h].bbt[0].bitcoinptr->ypoloipo=bitCoinValue;
		bitcoinIDHashtable[h].bbt[0].bitcoinptr->userID=finduserID(walletIDHashtable,userID,bucketSize,hashtablesize);
	}
	else{
		do{
			if(temp==NULL){
				temp=&bitcoinIDHashtable[h];
			}
			else{
				temp=temp->next;
			}
			for(k=0;k<j;k++){
				if(temp->bbt[k].bitcoinptr==NULL){
					temp->bbt[k].bitcoinID=bitcoinID;
					temp->bbt[k].bitcoinptr=(bitcoin*)malloc(sizeof(bitcoin));
					temp->bbt[k].bitcoinptr->ypoloipo=bitCoinValue;
					temp->bbt[k].bitcoinptr->userID=finduserID(walletIDHashtable,userID,bucketSize,hashtablesize);
					return bitcoinIDHashtable;
				}
				else if(temp->bbt[k].bitcoinID==bitcoinID){
					perror("bitcoin already exists");
					exit;
				}
			}
		}while(temp->next!=NULL);
		temp->next=(bitcoinbucket*)malloc(sizeof(bitcoinbucket));
		temp->next->bbt=(bitcoinlist*)malloc(j*sizeof(bitcoinlist));
		temp->next->bbt[0].bitcoinID=bitcoinID;
		temp->next->bbt[0].bitcoinptr=(bitcoin*)malloc(sizeof(bitcoin));
		temp->next->bbt[0].bitcoinptr->ypoloipo=bitCoinValue;
		temp->next->bbt[0].bitcoinptr->userID=finduserID(walletIDHashtable,userID,bucketSize,hashtablesize);
	}
	return bitcoinIDHashtable;
}

walletIDlist* finduserID(walletbucket* walletIDHashtable,char* userID,int bucketSize, int hashtablesize){
	int i,h,j,k;
	walletbucket *temp=NULL;
	i=0;
	j=(bucketSize-4)/sizeof(walletIDlist);
	h=Hashfunctionstring(userID);
	h=h%hashtablesize;
	if(	walletIDHashtable[h].wbt==NULL){
		printf("userID does not exist\n");
		return NULL;
	}
	do{
		if(temp==NULL){
			temp=&walletIDHashtable[h];
		}
		else{
			temp=temp->next;
		}
		for(k=0;k<j;k++){
			if(temp->wbt[k].walletID!=NULL){
				if(strcmp(temp->wbt[k].walletID,userID)==0){
					return &temp->wbt[k];
				}
			}
		}
	}while(temp->next!=NULL);
	printf("userID does not exist\n");
	return NULL;
}

bitcoinlist* findbitcoinID(bitcoinbucket* bitcoinIDHashtable,int bitcoinID,int bucketSize, int hashtablesize){
	int i,h,j,k;
	bitcoinbucket *temp=NULL;
	i=0;
	j=(bucketSize-4)/sizeof(bitcoinlist);
	h=Hashfunction(bitcoinID);
	h=h%hashtablesize;
	if(	bitcoinIDHashtable[h].bbt==NULL){
		printf("bitcoinID does not exist\n");
		return NULL;
	}
	do{
		if(temp==NULL){
			temp=&bitcoinIDHashtable[h];
		}
		else{
			temp=temp->next;
		}
		for(k=0;k<j;k++){
			if(temp->bbt[k].bitcoinptr!=NULL){
				if(temp->bbt[k].bitcoinID==bitcoinID){
					return &temp->bbt[k];
				}
			}
		}
	}while(temp->next!=NULL);
	printf("bitcoinID does not exist\n");
	return NULL;
}

void* readwalletHashtable(walletbucket* walletIDHashtable,int bucketSize, int hashtablesize){
	int i,h,j,k;
	walletbucket *temp=NULL;
	bitcoinbalance *temp2;//=(bitcoinbalance*)malloc(sizeof(bitcoinbalance));
	temp2=NULL;
	j=(bucketSize-4)/sizeof(walletIDlist);
	for(h=0;h<hashtablesize;h++){
		if(walletIDHashtable[h].wbt!=NULL){
			do{	
				if(temp==NULL){
					temp=&walletIDHashtable[h];
				}
				else{
					temp=temp->next;
				}
				for(k=0;k<j;k++){
					if(temp->wbt[k].walletptr!=NULL){
						printf("*UserID:%s %d %d balance:%d  bitcoins:",temp->wbt[k].walletID,h,k,temp->wbt[k].walletptr->ypoloipo);
						if(temp->wbt[k].walletptr->bitcoins!=NULL){
							do{
								if(temp2==NULL){				
									temp2=temp->wbt[k].walletptr->bitcoins;
								}
								else{
									temp2=temp2->next;
								}
								printf("%d ",temp2->bitcoinID->bitcoinID);
							}while(temp2->next!=NULL);	
						}
						printf("\n");
					}
					temp2=NULL;
				}
			}while(temp->next!=NULL);
		}
		temp=NULL;
	}
}

void* readbitcoinHashtable(bitcoinbucket* bitcoinIDHashtable,int bucketSize, int hashtablesize){
	int i,h,j,k;
	bitcoinbucket *temp=NULL;
	i=0;
	j=(bucketSize-4)/sizeof(bitcoinlist);
	printf("Printing bitcoins: ");
	for(h=0;h<hashtablesize;h++){
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
						printf("%d ",temp->bbt[k].bitcoinID);
					}
				}
			}while(temp->next!=NULL);
		}
		temp=NULL;
	}
	printf("\n");
}

walletbucket* deletebitcoin(walletbucket* walletIDHashtable,char *userID,int bitcoinID,int bucketSize, int hashtablesize){
	int i,h,j,k;
	walletbucket *temp=NULL;
	bitcoinbalance *temp2=NULL;
	bitcoinbalance *temp3=NULL;
	h=Hashfunctionstring(userID );
	h=h%hashtablesize;
	j=(bucketSize-4)/sizeof(walletIDlist);
	do{
		if(temp==NULL){
			temp=&walletIDHashtable[h];
		}
		else{
			temp=temp->next;
		}
		for(k=0;k<j;k++){
			if(strcmp(temp->wbt[k].walletID,userID)==0){
				if(temp->wbt[k].walletptr->bitcoins==NULL){
					printf("He doesn't have any bitcoins\n");
					return walletIDHashtable;
				}
				else{
					do{	
						if(temp2==NULL){				
							temp2=temp->wbt[k].walletptr->bitcoins;
						}
						else{
							temp3=temp2;
							temp2=temp2->next;
						}
						if(temp2->bitcoinID->bitcoinID==bitcoinID){
							if(temp3==NULL){
								walletIDHashtable[h].wbt[k].walletptr->bitcoins=NULL;
							}
							else{
								temp3=temp2->next;
							}
							free(temp2);
							return walletIDHashtable;
						}
					}while(temp2->next!=NULL);
					printf("Bitcoin doesn't exist\n");
					return walletIDHashtable;
				}
				return walletIDHashtable;
			}
		}
	}while(temp->next!=NULL);
		
	return walletIDHashtable;
}



