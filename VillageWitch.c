#include<stdio.h>
#include<stdlib.h>
#include<time.h>


#define M 20
#define C 30

typedef struct{
	int level;
	int damage;
	int health;
}final;

typedef struct{
	int money;
	int max_health;
	int health;
	int damage;
	int elixir;
	int level;
	int weapon_level;
}character;

typedef struct{
	int pot;
	int gold;
	int x;
	int y;
}bag;

typedef struct{
	int level;
	int damage;
	int health;
	int gold;
	int x;
	int y;
}monsters;

void update(int map[10][10],int map_control[10][10],monsters *zombie,bag *chess,character *player,final *boss);
void fight(monsters *zombie,character *player,int i,int *dead);
void prepare_map(int map[10][10],int map_control[10][10],monsters *zombie,bag *chess);
void displayMap(int map[10][10],int map_control[10][10],int day);
void upgrade(character *player);
void shop(character *player);
void boss_fight(final *boss,character *player,int *dead);

int main(){
	
	int map_control[10][10]={0};
	int map[10][10]={0};
	int choice;
	int i;
	srand(time(NULL));
	
	final *boss=NULL;
	character *player=NULL;
	monsters *zombie=NULL;
	bag *chess=	NULL;
	
	player=(character *)malloc(sizeof(character));
	zombie=(monsters *)malloc(M*sizeof(monsters));
	chess=(bag *)malloc(C*sizeof(bag));
	boss=(final *)malloc(3*sizeof(final));
	
	if(zombie==NULL || chess==NULL || player==NULL)
		exit(1);
	
	printf("				VILLAGE WITCH\n");
	printf("			  Developed by Engin MEMIS\n\n");
	player->health=100;
	player->money=200;
	player->damage=25;
	player->elixir=0;
	player->max_health=100;
	player->level=1;
	player->weapon_level=1;
	
	for(i=0;i<3;i++){
		boss[i].level=i+1;
		boss[i].health=(i+1)*1000;
		boss[i].damage=(i+1)*100;
	}
	
	update(map,map_control,zombie,chess,player,boss);
	
	return 0;
}



void update(int map[10][10],int map_control[10][10],monsters *zombie,bag *chess,character *player,final *boss){
	int turn;
	int day=1;
	int x=1,y=1;
	int i,j;
	int dead=0;
	int choice;
	while(player->health>0 && dead!=2){        //DEAD CONTROL
		choice=menu(day);
		switch(choice){
			case 1:
				player->health=player->max_health;
				prepare_map(map,map_control,zombie,chess);
				turn=5;
				
				while(turn!=0 && dead==0){				// TURN CONTROL
					displayMap(map,map_control,day);
					printf("Your Health: %d\nYour Avarage Damage: %d\nYour Elixir: %d\nYour Gold: %d\n\n",player->health,player->damage,player->elixir,player->money);
					printf("Your remaining turn:%d\n",turn);
					do{
						printf("Please enter the coordinates(Exp:2 4):");
						scanf("%d %d",&x,&y);
					}while(x<1 || x>10 || y<1 || y>10 || map_control[x-1][y-1]==1);
						
					if(map[x-1][y-1]=='C'){
						for(i=0;i<C;i++){
							if(chess[i].x==x-1 && chess[i].y==y-1){
								printf("\nYou found items\n%d Gold\n%d Elixir\n",chess[i].gold,chess[i].pot);
								player->money+=chess[i].gold;
								player->elixir+=chess[i].pot;
								i=C+1;
								sleep(3);
							}
						}
					}
					else if(map[x-1][y-1]=='M'){
						for(i=0;i<M;i++){
							if(zombie[i].x==x-1 && zombie[i].y==y-1){
								fight(zombie,player,i,&dead);
								i=M+1;
							}
						}
					}
					else if(map[x-1][y-1]=='B'){
						boss_fight(boss,player,&dead);
					}
					else{
						printf("\nYou did not find anything!");
						sleep(3);
					}
					
					if(dead==0){
						map_control[x-1][y-1]=1;
						turn--;	
					}
					else if(dead==1){
						system("cls");
						printf("YOU DIED\n");
						printf("YOU SURVIVED %d DAYS ",day);
					}
					else if(dead==2){
						system("cls");
						printf("				VILLAGE WITCH\n");
						printf("			  Developed by Engin MEMIS\n\n");
						printf("CONGRATULATIONS\n");
						printf("You Survived %d days",day);
					}
					
				}
				day++;
				break;
			case 2:
				upgrade(player);
				break;
			case 3:
				shop(player);
				break;
		}
	}
			
		
		
}

void fight(monsters *zombie,character *player,int i,int *dead){
	
	char control;
	int turn;
	int choice;
	int damage;
	system("cls");
	printf("				VILLAGE WITCH\n");
	printf("			  Developed by Engin MEMIS\n\n");
	printf("			YOU FOUND THE ZOMBIE\n\n");
	printf("Zombie's Health: %d\nZombie's Level: %d\nZombie's Avarage Damage: %d\n\n",zombie[i].health,zombie[i].level,zombie[i].damage);
	printf("Your Health: %d\nYour Avarage Damage: %d\nYour Elixir: %d\nYour Gold: %d\n\n",player->health,player->damage,player->elixir,player->money);
	do{
		printf("Do you want to leave the fight(-10 Gold)?(Y/N):");
		scanf(" %c",&control);	
	}while(control!='Y' && control!='y' && control!='N' && control !='n');
	
	if((control=='Y' || control=='y') && player->money>=10){
		player->money-=10;
	}
	else{
		turn=0;
		while(player->health>0 && zombie[i].health>0){
			system("cls");
			printf("				VILLAGE WITCH\n");
			printf("			  Developed by Engin MEMIS\n\n");
			printf("Zombie's Health: %d\nZombie's Level: %d\nZombie's Avarage Damage: %d\n\n",zombie[i].health,zombie[i].level,zombie[i].damage);
			printf("Your Health: %d\nYour Avarage Damage: %d\nYour Elixir: %d\nYour Gold: %d\n\n",player->health,player->damage,player->elixir,player->money);
			if(turn%2==0){
				printf("Your Turn\n");
				printf("1-Attack\n2-Use Elixir(Max Health)\nChoice:");
				scanf("%d",&choice);
				if(choice==2 && player->elixir<=0){
					printf("Not enough elixir!!!\n");
					choice=1;
				}
				
				switch(choice){
					case 1:
						damage= player->damage+(-5+rand()%11);
						printf("Damage: %d",damage);
						zombie[i].health-=damage;
						sleep(2);
						break;
					case 2:
						player->health=player->max_health;
						player->elixir--;
						sleep(2);
						break;
				}
			}
			else{
				printf("Zombie's Turn\n");
				damage=zombie[i].damage+(-5+rand()%11);
				printf("Damage: %d",damage);
				player->health-=damage;
				sleep(2);
			}
			turn++;
		}
		
		if(player->health<=0){
			*dead=1;
		}
		else{
			printf("\n\nYou killed the zombie!\n");
			printf("You Earned %d Gold",zombie[i].gold);
			player->money+=zombie[i].gold;
			sleep(2);
		}
		
	}
	
}

void boss_fight(final *boss,character *player,int *dead){
	int i=0,turn,choice,damage;
	turn=0;
	while(player->health>0 && i<3){
		system("cls");
		printf("				VILLAGE WITCH\n");
		printf("			  Developed by Engin MEMIS\n\n");
		printf("	CASTLE\n\n");
		printf("Boss' Health: %d\nBoss' Level: %d/3\nBoss' Avarage Damage: %d\n\n",boss[i].health,boss[i].level,boss[i].damage);
		printf("Your Health: %d\nYour Avarage Damage: %d\nYour Elixir: %d\nYour Gold: %d\n\n",player->health,player->damage,player->elixir,player->money);
		if(turn%2==0){
			printf("Your Turn\n");
			printf("1-Attack\n2-Use Elixir(Max Health)\nChoice:");
			scanf("%d",&choice);
			if(choice==2 && player->elixir<=0){
				printf("Not enough elixir!!!\n");
				choice=1;
			}
				
			switch(choice){
				case 1:
					damage= player->damage+(-5+rand()%11);
					printf("Damage: %d",damage);
					boss[i].health-=damage;
					sleep(2);
					break;
				case 2:
					player->health=player->max_health;
					player->elixir--;
					sleep(2);
					break;
			}
		}
		else{
			printf("Boss' Turn\n");
			damage=boss[i].damage+(-5+rand()%11);
			printf("Damage: %d",damage);
			player->health-=damage;
			sleep(2);
		}
		if(boss[i].health<=0){
			i++;
			turn=0;
		}
		else{
			turn++;
		}
		
	}
	if(player->health<=0){
			*dead=1;
	}
	else{
		*dead=2;
	}
}

void shop(character *player){
	
	int choice;
	int count;
	do{
		system("cls");
		printf("				VILLAGE WITCH\n");
		printf("			  Developed by Engin MEMIS\n\n");
		printf("Your Health: %d\nYour Avarage Damage: %d\nYour Elixir: %d\nYour Gold: %d\n\n",player->health,player->damage,player->elixir,player->money);
		printf("1-Buy Elixir (Cost:100 Gold)\n");
		printf("2-Sell Elixir (+50 Gold) (You have %d elixir)\n",player->elixir);
		printf("3-Exit\n\n");
		do{
			printf("Your Choice:");
			scanf("%d",&choice);
		}while(choice!=1 && choice!=2 && choice!=3);
		
		switch(choice){
			case 1:
				do{
					printf("How many do you want to buy:");
					scanf("%d",&count);
				}while(count*100>player->money);
				
				player->money-=100*count;
				player->elixir+=count;
				break;
			case 2:
				if(player->elixir<=0){
				printf("Not Enough Elixir!");
				sleep(2);
				}
				else{
					do{
						printf("How many do you want to sell:");
						scanf("%d",&count);	
					}while(count>player->elixir);	
					
					player->money+=50*count;
					player->elixir-=count;	
				}		
			
				break;
		}
	}while(choice!=3);
	
}

void upgrade(character *player){
	
	static int cost_weapon=100;
	static int cost_health=200;
	int choice;
	int count;
	
	do{
		system("cls");
		printf("				VILLAGE WITCH\n");
		printf("			  Developed by Engin MEMIS\n\n");
		printf("Your Health: %d\nYour Avarage Damage: %d\nYour Elixir: %d\nYour Gold: %d\n\n",player->health,player->damage,player->elixir,player->money);
		printf("1-Upgrade Weapon (+25 Damage) (Cost:%d)\n",cost_weapon);
		printf("2-Upgrade Max Health (+50 Health) (Cost:%d)\n",cost_health);
		printf("3-Exit\n\n");
		do{
			printf("Enter your choice:");
			scanf("%d",&choice);
		}while(choice!=1 && choice!=2 && choice!=3);
		
		switch(choice){
			case 1:
				if(cost_weapon<=player->money){
					player->damage+=25;
					player->money-=cost_weapon;
					player->weapon_level++;
					cost_weapon*=player->weapon_level;
				}
				else{
					printf("Not Enough Money!");
					sleep(2);
				}
				
				break;
			case 2:
				if(cost_health<=player->money){
					player->max_health+=50;
					player->money-=cost_health;
					player->health=player->max_health;
					player->level++;
					cost_health*=player->level;
				}
				else{
					printf("Not Enough Money!");
					sleep(2);
				}
				break;
		}
	}while(choice!=3);
	
}

void prepare_map(int map[10][10],int map_control[10][10],monsters *zombie,bag *chess){
	
	int i,j;
	int x,y;
	
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			map[i][j]=0;
			map_control[i][j]=0;
		}
	}
	
	for(i=0;i<M;i++){
		zombie[i].level=i+1;
		zombie[i].damage=(i*5)+1+rand()%5;
		zombie[i].gold=((i*50)+1+rand()%5)/2;
		zombie[i].health=zombie[i].level*20;
		
		do{
			x=rand()%10;
			y=rand()%10;
		}while(map[x][y]!=0);
		map[x][y]='M';
		zombie[i].x=x;
		zombie[i].y=y;
	}
	
	for(i=0;i<C;i++){
		chess[i].gold=1+rand()%30;
		chess[i].pot=rand()%3;
		
		do{
			x=rand()%10;
			y=rand()%10;
		}while(map[x][y]!=0);
		map[x][y]='C';	
		chess[i].x=x;
		chess[i].y=y;
	}
	
	do{
		x=rand()%10;
		y=rand()%10;
	}while(map[x][y]!=0);
	map[x][y]='B';
}


int menu(int day){
	
	int choice;
	
	do{
		system("cls");
		printf("				VILLAGE WITCH\n");
		printf("		   	  Developed by Engin MEMIS\n\n");
		if(day!=1){
			printf("			DAY FINISHED!\n");
		}
		printf("1-Map\n2-Upgrade\n3-Shop\n");
		scanf("%d",&choice);
	}while(choice!=1 && choice!=2 && choice!=3);
	
	return choice;
}


void displayMap(int map[10][10],int map_control[10][10],int day){
	
	int i,j;
	int x=1,y=1;
	system("cls");
	printf("				VILLAGE WITCH\n");
	printf("			  Developed by Engin MEMIS\n\n");
	printf("		DAY %d\n\n",day);
	printf("   ");
	for(i=0;i<10;i++){
		printf("%3d",i+1);
	}
	printf("\n\n");
	for(i=0;i<10;i++){
		printf("%3d",i+1);
		for(j=0;j<10;j++){
			if(map_control[i][j]==0){
				if(map[i][j]=='B'){
					printf("%3c",'B');
				}
				else{
					printf("%3c",242);
				}
				
			}
			else{
				if(map[i][j]=='M'){
					printf("%3c ",0);
				}
				else if(map[i][j]=='C'){
					printf("%3c ",0);
				}
				else{
					printf("%3c ",0);
				}
				
			}
			
		}
		printf("\n\n");
	}
		
}

