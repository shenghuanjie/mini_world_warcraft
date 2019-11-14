#include <iostream>
#include <iomanip>
#define none 0
#define red 1
#define blue 2
using namespace std;
int originallife;//本部的生命源
int originalhealth[8];//dragon 、ninja、iceman、lion、wolf 的生命力
int originalattack[8];//dragon 、ninja、iceman、lion、wolf 的攻击力
int numberofcity;//城市数目
int lionhealthbefore;//狮子战斗前的生命值
int hour;//小时
int minute;//分钟
int originaltime;//总时长
int pasttime;//已经经过的时长
char whichside[10][10]={"","red","blue"};
class monster
{	public:
	monster(char * inputname, int inputhealth, int inputattack, int inputnumber, int inputcolor, int inputposition):health(inputhealth),attack(inputattack),number(inputnumber),color(inputcolor),position(inputposition)
	{//怪物基类的构造函数，依次初始化名字，生命值，攻击力，序号，所属阵营，所处位置
		name =new char[10];
		strcpy(name,inputname);
		cout <<setfill('0')<<setw(3)<<hour<< ':'<<setfill('0')<<setw(2)<<minute;
		printf(" %s %s %d born\n", whichside[color], name, number);
	}
	char *name;//名字
	int number;//编号
	int health;//生命值
	int attack;//攻击力
	int position;//所在城市编号，为红方大本营，numberofcity+1为蓝方大本营
	int color;//所属阵营
	virtual void move()//控制移动的函数
	{
		if(color==red)//红色右移
			position++;
		else//蓝色左移
			position--;
		cout <<setfill('0')<<setw(3)<<hour<< ':'<<setfill('0')<<setw(2)<<minute;
		if(position==numberofcity+1||position==0)//到达总部
		{
			printf(" %s %s %d reached %s headquarter with %d elements and force %d\n",whichside[color], name, number, whichside[3-color], health, attack);
			delete this;
		}
		else//到达城市
		printf(" %s %s %d marched to city %d with %d elements and force %d\n",whichside[color], name, number, position, health, attack);
	}
	virtual bool hurt(int attacking)
	{//受伤的函数
		health-=attacking;
		if(health>0)//没死
			return false;
		else
		{//死了
			cout <<setfill('0')<<setw(3)<<hour<< ':'<<setfill('0')<<setw(2)<<minute;
			printf(" %s %s %d was killed in city %d\n", whichside[color], name, number, position);
			return true;
		}
	}
	virtual void fight(monster *x)
	{//攻击函数
		cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
		printf(" %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",whichside[color], name, number, whichside[x->color], x->name, x->number, position, health, attack);
		if(!x->hurt(attack))//没死则反击
			x->fightback(this);
		else
		{//死了，如果是Lion，吸血
			if(!strcmp(x->name,"lion"))
				health+=lionhealthbefore;
		}
	}
	virtual void fightback(monster *x)
	{//反击函数
		cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2) <<minute;
		printf(" %s %s %d fought back against %s %s %d in city %d\n",whichside[color], name, number, whichside[x->color], x->name, x->number, position, health, attack);
		if(x->hurt(attack/2) &&!strcmp(x->name,"lion"))//死了，如果是Lion，吸血
			health+=lionhealthbefore;
	}
};
class dragon:public monster//龙
{	public:
	dragon(char * inputname, int inputhealth, int inputattack, int inputnumber, int inputcolor, int inputposition):monster(inputname,inputhealth, inputattack,inputnumber,inputcolor,inputposition)
	{}
	virtual void fight(monster *x)
	{
		monster::fight(x);
		if(health>0)//咆哮
		{
			cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2) <<minute;
			printf(" %s %s %d yelled in city %d\n",whichside[color], name, number, position);
		}
	}

};
class ninja:public monster//忍者
{
		public:
	ninja(char * inputname, int inputhealth, int inputattack, int inputnumber, int inputcolor, int inputposition):monster(inputname,inputhealth, inputattack,inputnumber,inputcolor,inputposition)
	{}
	virtual void fightback(monster *x)//我忍了
	{
	}
};
class iceman:public monster//冰人
{
	public:
	iceman(char * inputname, int inputhealth, int inputattack, int inputnumber, int inputcolor, int inputposition):monster(inputname,inputhealth, inputattack,inputnumber,inputcolor,inputposition)
	{step=0;}
	int step;
	virtual void move()//两步自残
	{
		step++;
		if(step==2)
		{
			step=0;
			attack+= 20;
			if(health>9)
				health-= 9;
			else
				health=1;
		}
		monster::move();
	}
};
class lion:public monster//狮子
{
		public:
	lion(char * inputname, int inputhealth, int inputattack, int inputnumber, int inputcolor, int inputposition):monster(inputname,inputhealth, inputattack,inputnumber,inputcolor,inputposition)
	{}
	virtual bool hurt(int attacking)//损己利人
	{
		 lionhealthbefore=health;//记录生命值以备吸取
		return monster::hurt(attacking);
	}
};
class wolf:public monster//狼
{	public:
	wolf(char * inputname, int inputhealth, int inputattack, int inputnumber, int inputcolor, int inputposition):monster(inputname,inputhealth, inputattack,inputnumber,inputcolor,inputposition)
	{numberkilled=0;}
	int numberkilled;
	virtual void fight(monster *x)
	{
		cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
		printf(" %s %s %d attacked %s %s %d in city %d with %d elements and force %d\n",whichside[color], name, number, whichside[x->color], x->name, x->number, position, health, attack);
		if(!x->hurt(attack))
			x->fightback(this);
		else
		{
			numberkilled++;
			if(numberkilled==2)//杀人如麻
			{
				health*=2;
				attack*=2;
				numberkilled=0;
			} 
			if(!strcmp(x->name,"lion"))//吸狮子血
				health+=lionhealthbefore;
			//delete x;
		}
	}
};
struct city
{
	int life;//生命源
	int whowinbefore;//记录是谁在之前在这个城市取得的胜利，为红，为蓝
	int flag;//记录现在的旗帜颜色，为无，为红，为蓝
	monster * warrior[3];
};
void setcity(city &mycity)//初始化城市的函数，传入一个引用
{
	mycity.life=0;
	mycity.whowinbefore=none;
	mycity.flag=none;
	mycity.warrior[0]=NULL;
	mycity.warrior[1]=NULL;
	mycity.warrior[2]=NULL;
}
class center
{	public:
	int life;//存储的生命源
	int number;//下一个产生的怪物的编号
	int enemy;//大本营中敌人数目
	int color;//颜色
	monster *newmonster;//产生怪物的指针
	center(int inputlife,int inputcolor):life(inputlife),color(inputcolor)
	{
		enemy=0;
		number=1;
	}
	bool endgame()
	{
		if(enemy==2)
		{
			cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
			printf(" %s headquarter was taken\n", whichside[color]);
			return true;
		}
		return false;
	}
	monster * create()//生成怪物的函数，未生成返回NULL
	{
		if(color==red)
		{
			switch(number%5)
			{
			    case 1:
					if(originalhealth[2]<=life)
					{newmonster = new iceman("iceman", originalhealth[2], originalattack[2], number, red, 0);life-=originalhealth[2];number++;
					return newmonster;}
					break;
				case 2:
					if(originalhealth[3]<=life){newmonster = new lion("lion", originalhealth[3], originalattack[3], number, red, 0);life-=originalhealth[3];number++;
					return newmonster;}
					break;
				case 3:
					if(originalhealth[4]<=life){newmonster = new wolf("wolf", originalhealth[4], originalattack[4], number, red, 0);life-=originalhealth[4];number++;
					return newmonster;}
					break;
				case 4:
					if(originalhealth[1]<=life){newmonster = new ninja("ninja", originalhealth[1], originalattack[1], number, red, 0);life-=originalhealth[1];number++;
					return newmonster;}
					break;
				case 0:
					if(originalhealth[0]<=life){newmonster = new dragon("dragon", originalhealth[0], originalattack[0], number, red, 0);life-=originalhealth[0];number++;
					return newmonster;}
					break;
				default: 
				return NULL;
			}
		}
		else
		{
			switch(number%5)
			{
			    case 1:
					if(originalhealth[3]<=life){newmonster = new lion("lion", originalhealth[3], originalattack[3], number, blue, numberofcity+1);life-=originalhealth[3];number++;
					return newmonster;}
					break;
				case 2:
					if(originalhealth[0]<=life){newmonster = new dragon("dragon", originalhealth[0], originalattack[0], number, blue, numberofcity+1);life-=originalhealth[0];number++;
					return newmonster;}
					break;
				case 3:
					if(originalhealth[1]<=life){newmonster = new ninja("ninja", originalhealth[1], originalattack[1], number, blue, numberofcity+1);life-=originalhealth[1];number++;
					return newmonster;}
					break;
				case 4:
					if(originalhealth[2]<=life){newmonster = new iceman("iceman", originalhealth[2], originalattack[2], number, blue, numberofcity+1);life-=originalhealth[2];number++;
					return newmonster;}
					break;
				case 0:
					if(originalhealth[4]<=life){newmonster = new wolf("wolf", originalhealth[4], originalattack[4], number, blue, numberofcity+1);life-=originalhealth[4];number++;
					return newmonster;}
					break;
				default: 
				return NULL;
			}
		}
		return NULL;//生命源不足，返回NULL
	}
};
int timefly()//处理时间的函数，返回值为~5，表示现在处于哪个阶段
{
	if(pasttime>=originaltime)
		return -1;
	pasttime+=10;
	minute+=10;
	if(minute>=60)
	{
		hour+=1;
		minute-=60;
	}
	return minute/10;
}


int main()
{
	int t,tt;//t总测试组数，tt目前进行到第几组
	int tempblue,tempred;//缓存双方因为战胜对手而获得的生命源
	int i;//用于循环的变量
	bool stop;//用于结束本组数据的变量
	city cities[30];//生成城市的组数
	cin>>t;
	for(tt=1;tt<=t;tt++)
	{
		for(i=0;i<30;i++)//初始化城市
		setcity(cities[i]);
		cin>>originallife>>numberofcity>>originaltime;//输入
		for(i=0;i<5;i++)cin>>originalhealth[i];
		for(i=0;i<5;i++)cin>>originalattack[i];
		minute=-10;pasttime=-10;hour=0;stop=false;//初始化
		center redcenter(originallife,red),bluecenter(originallife,blue);
		cout << "Case:" <<tt<<endl;//第几组
		while(!stop)
		{
			switch(timefly())
			{
			    case -1://时间到
				   stop=true;
			    break;
			    case 0://总部生成怪物
					cities[0].warrior[red]=redcenter.create();
					cities[numberofcity+1].warrior[blue]=bluecenter.create();
				break;
			    case 1://怪物前进
					if(cities[1].warrior[blue])//蓝方可能到达红方大本营的怪物
					{
						cities[1].warrior[blue]->move();
						redcenter.enemy++;
					}
					if(redcenter.endgame())stop=true;//是否继续游戏
					for(i=1;i<=numberofcity;i++)//群体移动
					{
						if(cities[i-1].warrior[red])
						cities[i-1].warrior[red]->move();
						if(cities[i+1].warrior[blue])
						cities[i+1].warrior[blue]->move();
					}
					if(cities[numberofcity].warrior[red])//红方可能到达蓝方大本营的怪物
					{
						cities[numberofcity].warrior[red]->move();
						bluecenter.enemy++;
					}
					if(bluecenter.endgame())stop=true;//是否继续游戏
					for(i=0;i<=numberofcity;i++)//城市中存储的怪物的指针要移位
						cities[i].warrior[blue]=cities[i+1].warrior[blue];
					for(i=numberofcity+1;i>0;i--)
						cities[i].warrior[red]=cities[i-1].warrior[red];
				break;
				case 2://城市生成生命源
					for(i=1;i<=numberofcity;i++)
						cities[i].life+=10;
				break;
				case 3://怪物独揽生命源
					for(i=1;i<=numberofcity;i++)
					{
						if(cities[i].warrior[red]&&!cities[i].warrior[blue])
						{	//红方独揽
							redcenter.life+=cities[i].life;
							cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
							printf(" red %s %d earned %d elements for his headquarter\n",cities[i].warrior[red]->name, cities[i].warrior[red]->number,cities[i].life);
							cities[i].life=0;
						}
						if(!cities[i].warrior[red]&&cities[i].warrior[blue])
						{   //蓝方独揽
							bluecenter.life+=cities[i].life;
							cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
							printf(" blue %s %d earned %d elements for his headquarter\n",cities[i].warrior[blue]->name, cities[i].warrior[blue]->number,cities[i].life);
							cities[i].life=0;
						}
					}
				break;
				case 4://怪物厮杀
					tempblue=0;
					for(i=1;i<=numberofcity;i++)
					{
						if(cities[i].warrior[red]&&cities[i].warrior[blue])//假如一个城市里有两个人
						{
						   if(cities[i].flag==red||(cities[i].flag!=blue&&i%2==1))//红方先出招
						   {
								cities[i].warrior[red]->fight(cities[i].warrior[blue]);
								if(cities[i].warrior[red]->health>0&&cities[i].warrior[blue]->health<=0)//红方胜利
							{
								cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
								printf(" red %s %d earned %d elements for his headquarter\n",cities[i].warrior[red]->name, cities[i].warrior[red]->number, cities[i].life);
									if(cities[i].whowinbefore==red)//两连胜，升旗
								{
										if(cities[i].flag==none)
									{
										cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
										printf(" red flag raised in city %d\n", i);
									}
										cities[i].flag=red;
								}
								else//这局红赢了
									cities[i].whowinbefore=red;
								}
							else if(cities[i].warrior[red]->health<=0&&cities[i].warrior[blue]->health>0)//蓝方反击把红方K死了
							{
								if(bluecenter.life>=8)//奖励蓝方英雄生命源
								{
									cities[i].warrior[blue]->health+=8;
									bluecenter.life-=8;
								}
								cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
								printf(" blue %s %d earned %d elements for his headquarter\n",cities[i].warrior[blue]->name, cities[i].warrior[blue]->number, cities[i].life);
								tempblue+=cities[i].life;//锁存战斗胜利获得的生命源
								cities[i].life=0;
								if(cities[i].whowinbefore==blue)//是否两连胜
								{
									if(cities[i].flag==none)//升旗
									{
										cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
										printf(" blue flag raised in city %d\n", i);
									}
									cities[i].flag=blue;
								}
								else
									cities[i].whowinbefore=blue;
							}
							else cities[i].whowinbefore=none;
						   }
						else if(cities[i].flag==blue||(cities[i].flag!=red&&i%2==0))
						{//蓝方率先暴动
							cities[i].warrior[blue]->fight(cities[i].warrior[red]);
							if(cities[i].warrior[red]->health>0&&cities[i].warrior[blue]->health<=0)
							{//红方果断反击，蓝方当场毙命
								cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
								printf(" red %s %d earned %d elements for his headquarter\n",cities[i].warrior[red]->name, cities[i].warrior[red]->number, cities[i].life);
								if(cities[i].whowinbefore==red)//是否连胜
								{
									if(cities[i].flag==none)//升旗
									{
										cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
										printf(" red flag raised in city %d\n",i);
									}
										cities[i].flag=red;
								}
								else
									cities[i].whowinbefore=red;
							}
							else if(cities[i].warrior[red]->health<=0&&cities[i].warrior[blue]->health>0)
							{//蓝方竟然被红方反击致死了
								if(bluecenter.life>=8)
								{//奖励
									cities[i].warrior[blue]->health+=8;
									bluecenter.life-=8;
								}
								cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
								printf(" blue %s %d earned %d elements for his headquarter\n",cities[i].warrior[blue]->name, cities[i].warrior[blue]->number, cities[i].life);
								tempblue+=cities[i].life;//锁存战斗获得的生命源
								cities[i].life=0;
								if(cities[i].whowinbefore==blue)//是否连胜
								{
									if(cities[i].flag==none)//升旗
									{
										cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
										printf(" blue flag raised in city %d\n", i);
									}
									cities[i].flag=blue;
								}
								else
									cities[i].whowinbefore=blue;
							}
							else cities[i].whowinbefore=none;
						   }
						}
					}
					tempred=0;
					for(i=numberofcity;i>0;i--)//奖励红方
					{
						if(cities[i].warrior[red]&&cities[i].warrior[blue])
						{
						if(cities[i].warrior[red]->health>0&&cities[i].warrior[blue]->health<=0)//红方在本次战斗中胜利
						{
							if(redcenter.life>=8)
							{
								redcenter.life-=8;
								cities[i].warrior[red]->health+=8;
							}
							tempred+=cities[i].life;
							cities[i].life=0;
							delete cities[i].warrior[blue];//移走蓝方尸体
							cities[i].warrior[blue]=NULL;
						}
						else if(cities[i].warrior[red]->health<=0&&cities[i].warrior[blue]->health>0)//蓝方在本次战斗中胜利
						{
							delete cities[i].warrior[red];//移走红方尸体
							cities[i].warrior[red]=NULL;
						}

						}
					}
					redcenter.life+=tempred;//生命源送回总部
					bluecenter.life+=tempblue;
				break;
				case 5://总部“时终”总结，报告剩余生命源
					cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
					printf(" %d elements in red headquarter\n", redcenter.life);
					cout <<setfill('0')<<setw(3)<<hour<<':'<<setfill('0')<<setw(2)<<minute;
					printf(" %d elements in blue headquarter\n", bluecenter.life);
				break;
			}
		}
	}
	return 0;
}
