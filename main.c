#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"domes.h"

////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]){
	char *bitCoinBalancesFile,*transactionsFile,userID[51],receiverID[51],transactionID[11],*px,date[11],date2nd[11],ttime[11],ttime2[11],money[6],transaction[160];
	int bitCoinValue,senderHashtableNumOfEntries,receiverHashtableNumOfEntries,bucketSize,i,j,k,date1,date2,time1,time2,lasttime,lastdate,neg,h,l,u,maxtransactionID;
	bucket *senderHashtable,*receiverHashtable;
	walletbucket *walletIDHashtable;
	bitcoinbucket *bitcoinIDHashtable;
	bitcoinbucket *tempbitcoinIDHashtable=NULL;
	walletIDlist* walletyp;
	FILE* fp;
	time_t curtime;
	if(argc!=13){
		perror("wrong input");
		return 0;
	}
	neg=-1;
	lasttime=0000;
	lastdate=00000000;
	maxtransactionID=0;
	for(i=1;i<argc;i++){
		if(strcmp(argv[i],"-a")==0){
			bitCoinBalancesFile=argv[i+1];
		}
		else if(strcmp(argv[i],"-t")==0){
			transactionsFile=argv[i+1];
		}
		else if(strcmp(argv[i],"-v")==0){
			bitCoinValue=atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-h1")==0){
			senderHashtableNumOfEntries=atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-h2")==0){
			receiverHashtableNumOfEntries=atoi(argv[i+1]);
		}
		else if(strcmp(argv[i],"-b")==0){
			bucketSize=atoi(argv[i+1]);
		}
	}
	printf("bitCoinBalancesFile:%s  ,transactionsFile:%s ,bitCoinValue:%d ,senderHashtableNumOfEntries:%d  ,receiverHashtableNumOfEntries:%d ,bucketSize:%d \n",bitCoinBalancesFile ,transactionsFile,bitCoinValue,senderHashtableNumOfEntries,receiverHashtableNumOfEntries,bucketSize);
	
	senderHashtable=(bucket*)malloc(senderHashtableNumOfEntries *sizeof( bucket));
	receiverHashtable=(bucket*)malloc(receiverHashtableNumOfEntries * sizeof(bucket));
	walletIDHashtable=(walletbucket *)malloc(walletIDHashtablesize*sizeof(walletbucket));
	bitcoinIDHashtable=(bitcoinbucket*)malloc(bitcoinIDHashtablesize*sizeof(bitcoinbucket));
	fp = fopen(bitCoinBalancesFile,"r");
	while(fscanf(fp,"%s",userID)>0){   
		i=0;
		
		
		if(atoi(userID)){             //userID must have at least one latin character
			if(px==NULL){
				continue;
			}
			else{
				bitcoinIDHashtable=newbitcoinID(bitcoinIDHashtable,walletIDHashtable,px,atoi(userID),bitCoinValue,bucketSize,bitcoinIDHashtablesize);                        //inser bitcoins inside wallet
				walletIDHashtable=insertbitcoin(walletIDHashtable,bitcoinIDHashtable,px,atoi(userID),bucketSize,walletIDHashtablesize);
				walletIDHashtable=increaseypoloipo(walletIDHashtable,px,bitCoinValue,bucketSize,walletIDHashtablesize);
			}
		}
		else{
			while(userID[i]!='\0'){
				i=i+1;
			}
			walletIDHashtable=insertwallet(walletIDHashtable,userID,bucketSize,walletIDHashtablesize);  //make wallet
			px=(char*)malloc(i*sizeof(char));
			strcpy(px,userID);
		
			//printf("------------%d \n",walletIDHashtable[8].wbt[0].walletptr->bitcoins->bitcoinID->bitcoinID);
		}
		
	}
	
	fp = fopen(transactionsFile,"r");
	while(fscanf(fp,"%s",transactionID)>0){ 
		fscanf(fp,"%s",userID);
		fscanf(fp,"%s",receiverID);
		fscanf(fp,"%s",money);
		fscanf(fp,"%s",date);
		fscanf(fp,"%s",ttime);
		date1=datetranslate(date);
		time1=timetranslate(ttime);
		
		if(date1>lastdate){                 //check if date is new date
			lastdate=date1;
			lasttime=time1;                                  //insert transaction to both hashtables
			senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
			receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
			walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
			walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
			bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);  //make payment
			if(atoi(transactionID)>maxtransactionID){
				maxtransactionID=atoi(transactionID);
			}
		}
		else{
			if(date1==lastdate && time1>lasttime){       //check time
				lastdate=date1;
				lasttime=time1;
				senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
				receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
				walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
				walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
				bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);
				if(atoi(transactionID)>maxtransactionID){
					maxtransactionID=atoi(transactionID);
				}
			}
			else{
				printf("Cannot make transaction, because the time of the transaction is in the past\n");
			}
		}
	}
	while(fgets(transaction,160,stdin)){        //start reading inputs
		px=strtok(transaction," ");
		i=0;
		if(strcmp(px,"/requestTransaction")==0){
			maxtransactionID+=1;
			sprintf(transactionID,"%d",maxtransactionID);
			while(px!=NULL){
				px=strtok(NULL," ");
				if(px==NULL)break;	
				i++;
				if(i==1){
					strcpy(userID,px);
				}
				else if(i==2){
					strcpy(receiverID,px);
				}
				else if(i==3){
					strcpy(money,px);
				}
				else if(i==4){
					strcpy(date,px);
				}
				else if(i==5){
					strcpy(ttime,px);
				}				
			}
			if(i==3){
				time(&curtime);
				px=strtok(ctime(&curtime)," ");
				px=strtok(NULL," ");
				px=strtok(NULL," ");
				px=strtok(NULL," ");
				strcpy(ttime,px);
				printf("time:%s\n",ttime);
				strcpy(date,"20-03-2019");             //I don't think this is important
			}		
			date1=datetranslate(date);
			time1=timetranslate(ttime);
			px=strtok(userID,"\n");
			strcpy(userID,px);
			px=strtok(receiverID,"\n");           //having difficulties with reading from files
			strcpy(receiverID,px);
			if(date1>lastdate){
				lastdate=date1;
				lasttime=time1;
				senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
				receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
				walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
				walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
				bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);
				if(atoi(transactionID)>maxtransactionID){
					maxtransactionID=atoi(transactionID);
				}
			}
			else{
				if(date1==lastdate && time1>lasttime){
					lastdate=date1;
					lasttime=time1;
					senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
					receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
					walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
					walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
					bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);
					if(atoi(transactionID)>maxtransactionID){
						maxtransactionID=atoi(transactionID);
					}
				}
				else{
					printf("Cannot make transaction, because the time of the transaction is in the past\n");
				}
			}
		}
		else if(strcmp(px,"/requestTransactions")==0){
			maxtransactionID+=1;
			sprintf(transactionID,"%d",maxtransactionID);
			while(px!=NULL){
				px=strtok(NULL," ");
				if(px==NULL)break;	
				i++;
				if(i==1){
					strcpy(userID,px);
				}
				else if(i==2){
					strcpy(receiverID,px);
				}
				else if(i==3){
					strcpy(money,px);
				}
				else if(i==4){
					strcpy(date,px);
				}
				else if(i==5){
					strcpy(ttime,px);
				}				
			}
			if(i==1){                           //read from input file
				fp = fopen(transactionsFile,"r");
				while(fscanf(fp,"%s",transactionID)>0){ 
					fscanf(fp,"%s",userID);
					fscanf(fp,"%s",receiverID);
					fscanf(fp,"%s",money);
					fscanf(fp,"%s",date);
					fscanf(fp,"%s",ttime);
					date1=datetranslate(date);
					time1=timetranslate(ttime);
					px=strtok(ttime,";");
					strcpy(ttime,px);
					if(date1>lastdate){                 //check if date is new date
						lastdate=date1;
						lasttime=time1;                                  //insert transaction to both hashtables
						senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
						receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
						walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
						walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
						bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);  //make payment
						if(atoi(transactionID)>maxtransactionID){
							maxtransactionID=atoi(transactionID);
						}
					}
					else{
						if(date1==lastdate && time1>lasttime){       //check time
							lastdate=date1;
							lasttime=time1;
							senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
							receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
							walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
							walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
							bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);
							if(atoi(transactionID)>maxtransactionID){
								maxtransactionID=atoi(transactionID);
							}
						}
						else{
							printf("Cannot make transaction, because the time of the transaction is in the past\n");
						}
					}
				}
			}
			else{                      //read from command line
				while(1){  //read from stdin until I press another command
					if(i==3){
						time(&curtime);
						px=strtok(ctime(&curtime)," ");
						px=strtok(NULL," ");
						px=strtok(NULL," ");
						px=strtok(NULL," ");
						strcpy(ttime,px);
						printf("time:%s\n",ttime);
						strcpy(date,"20-03-2019");             //I don't think this is important
						px=strtok(money,";");
						strcpy(money,px);
					}	
					else if(ttime!=NULL){
						px=strtok(ttime,";");
						strcpy(ttime,px);
					}	
					date1=datetranslate(date);
					time1=timetranslate(ttime);
					px=strtok(userID,"\n");
					strcpy(userID,px);
					px=strtok(receiverID,"\n");
					strcpy(receiverID,px);
					if(date1>lastdate){
						lastdate=date1;
						lasttime=time1;
						senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
						receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
						walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
						walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
						bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);
						if(atoi(transactionID)>maxtransactionID){
							maxtransactionID=atoi(transactionID);
						}
					}
					else{
						if(date1==lastdate && time1>lasttime){
							lastdate=date1;
							lasttime=time1;
							senderHashtable=inserttransaction(senderHashtable,&walletIDHashtable,transactionID,userID,receiverID,money,date,ttime,bucketSize,senderHashtableNumOfEntries);
							receiverHashtable=receivertransaction(receiverHashtable,senderHashtable,&walletIDHashtable,receiverID,userID,transactionID,bucketSize,receiverHashtableNumOfEntries);
							walletIDHashtable=increaseypoloipo(walletIDHashtable,userID,atoi(money)*neg,bucketSize,walletIDHashtablesize);
							walletIDHashtable=increaseypoloipo(walletIDHashtable,receiverID,atoi(money),bucketSize,walletIDHashtablesize);
							bitcoinIDHashtable=dividebitcoins(bitcoinIDHashtable,senderHashtable,&walletIDHashtable,transactionID,userID,bucketSize,senderHashtableNumOfEntries);
							if(atoi(transactionID)>maxtransactionID){
								maxtransactionID=atoi(transactionID);
							}
						}
						else{
							printf("Cannot make transaction, because the time of the transaction is in the past\n");
						}
					}
					fgets(transaction,160,stdin);
					i=0;
					px=strtok(transaction," ");        //end if you type another command
					if(strcmp(px,"/requestTransaction")==0 || strcmp(px,"/findEarnings")==0 || strcmp(px,"/findPayments")==0 || strcmp(px,"/walletStatus")==0 || strcmp(px,"/bitCoinStatus")==0 || strcmp(px,"/traceCoin")==0 || strcmp(px,"/exit")==0 )break; 
					strcpy(userID,px);
					maxtransactionID+=1;
					sprintf(transactionID,"%d",maxtransactionID);
					while(px!=NULL){
						px=strtok(NULL," ");
						if(px==NULL)break;	
						i++;
						if(i==1){
							strcpy(receiverID,px);
						}
						else if(i==2){
							strcpy(money,px);
						}
						else if(i==3){
							strcpy(date,px);
						}
						else if(i==4){
							strcpy(ttime,px);
						}
					}
					if(i<2)break;
					i++;
				}
			}
		}
		if(strcmp(px,"/findEarnings")==0){
			i=0;
			while(px!=NULL){
				px=strtok(NULL," ");
				if(px==NULL)break;	
				i++;
				if(i==1){
					strcpy(userID,px);
				}
				else if(i==2){
					strcpy(date,px);
				}
				else if(i==3){
					strcpy(ttime,px);
				}
				else if(i==4){
					strcpy(date2nd,px);
				}
				else if(i==5){
					strcpy(ttime2,px);
				}
			}
			if(i<1)break;
			px=strtok(userID,"\n");
			strcpy(userID,px);
			if(date==NULL){
				strcpy(date,"00-00-0000");
				strcpy(date2nd,"99-99-9999");
				strcpy(ttime,"00:00");
				strcpy(ttime2,"99:99");
			}
			else if(ttime==NULL){
				continue;
			}
			else if(date2nd==NULL){
				strcpy(date2nd,ttime);
				strcpy(ttime,"00:00");
				strcpy(ttime2,"99:99");
			}
			printf("total earnings:%d\n",findtransactions(receiverHashtable,userID,date,ttime,date2nd,ttime2,bucketSize,receiverHashtableNumOfEntries));
		}
		if(strcmp(px,"/findPayments")==0){
			i=0;
			while(px!=NULL){
				px=strtok(NULL," ");
				if(px==NULL)break;	
				i++;
				if(i==1){
					strcpy(userID,px);
				}
				else if(i==2){
					strcpy(date,px);
				}
				else if(i==3){
					strcpy(ttime,px);
				}
				else if(i==4){
					strcpy(date2nd,px);
				}
				else if(i==5){
					strcpy(ttime2,px);
				}
			}
			if(i<1)break;
			px=strtok(userID,"\n");
			strcpy(userID,px);
			if(date==NULL){
				strcpy(date,"00-00-0000");
				strcpy(date2nd,"99-99-9999");
				strcpy(ttime,"00:00");
				strcpy(ttime2,"99:99");
			}
			else if(ttime==NULL){
				continue;
			}
			else if(date2nd==NULL){
				strcpy(date2nd,ttime);
				strcpy(ttime,"00:00");
				strcpy(ttime2,"99:99");
			}
			printf("total payments:%d\n",findtransactions(senderHashtable,userID,date,ttime,date2nd,ttime2,bucketSize,senderHashtableNumOfEntries));
		}
		if(strcmp(px,"/walletStatus")==0){
			px=strtok(NULL," ");
			strcpy(userID,px);
			px=strtok(userID,"\n");
			strcpy(userID,px);
			walletyp=finduserID(walletIDHashtable,userID,bucketSize,walletIDHashtablesize);
			printf("%s has %d in his wallet\n",userID,walletyp->walletptr->ypoloipo);
		}
		if(strcmp(px,"/bitCoinStatus")==0){
			px=strtok(NULL," ");
			strcpy(userID,px);
			px=strtok(userID,"\n");
			strcpy(userID,px);
			h=Hashfunction(atoi(userID));
			h=h%bitcoinIDHashtablesize;
			j=(bucketSize-4)/sizeof(bitcoinlist);
			tempbitcoinIDHashtable=NULL;
			if(bitcoinIDHashtable[h].bbt!=NULL){
			do{
				if(tempbitcoinIDHashtable==NULL){
					tempbitcoinIDHashtable=&bitcoinIDHashtable[h];
				}
				else{
					tempbitcoinIDHashtable=tempbitcoinIDHashtable->next;
				}
				for(k=0;k<j;k++){
					if(tempbitcoinIDHashtable->bbt[k].bitcoinptr!=NULL){
						if(tempbitcoinIDHashtable->bbt[k].bitcoinID==atoi(userID)){
							l=findnumberoftransactions(tempbitcoinIDHashtable->bbt[k].bitcoinptr);
							u=findunspentsum(tempbitcoinIDHashtable->bbt[k].bitcoinptr);
							printf("%d %d %d\n",atoi(userID),l,u);
							break;
						}
					}
				}
			}while(tempbitcoinIDHashtable->next!=NULL);
			}
		}
		if(strcmp(px,"/traceCoin")==0){
			px=strtok(NULL," ");
			strcpy(userID,px);
			px=strtok(userID,"\n");
			strcpy(userID,px);
			h=Hashfunction(atoi(userID));
			h=h%bitcoinIDHashtablesize;
			j=(bucketSize-4)/sizeof(bitcoinlist);
			tempbitcoinIDHashtable=NULL;
			if(bitcoinIDHashtable[h].bbt!=NULL){
			do{
				if(tempbitcoinIDHashtable==NULL){
					tempbitcoinIDHashtable=&bitcoinIDHashtable[h];
				}
				else{
					tempbitcoinIDHashtable=tempbitcoinIDHashtable->next;
				}
				for(k=0;k<j;k++){
					if(tempbitcoinIDHashtable->bbt[k].bitcoinptr!=NULL){        //find coin in hashtable
						if(tempbitcoinIDHashtable->bbt[k].bitcoinID==atoi(userID)){
							readalltransactions(tempbitcoinIDHashtable->bbt[k].bitcoinptr);
							break;
						}
					}
				}
			}while(tempbitcoinIDHashtable->next!=NULL);
			}
		}
		if(strcmp(px,"/exit\n")==0){
			
			break;
		}
	}
	//readwalletHashtable(walletIDHashtable,bucketSize,walletIDHashtablesize);   
	//readbitcoinHashtable(bitcoinIDHashtable,bucketSize,bitcoinIDHashtablesize);
	
}
