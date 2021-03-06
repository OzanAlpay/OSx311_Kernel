#include "isr.h"
#include "monitor.h"
#include "shell.h"
#include "timer.h"

extern void mboot(); //Assembly Fonksiyonunu kullanabilmek için [reboot yaparken]
static void helpOption()
{
	monitor_write("Available commands: clear ,help, settime, add, sub, reboot\n");
}
static void rebootOption()
{
	u32int i;
	monitor_write("rebooting...");
	while(i<600000)
    i++;
    //bunu yapma sebebimiz thread stop falan bile yok daha kernelde sleepte yok getchar falanda yok yani mecburiyetten
	mboot();
}
static void errorSetTime()
{
	monitor_write("You should use settime with three parameters like : settime hh mm ss\n");
}
static void error(char *c)
{
	monitor_put(39); // Bunlar '--> karakteri string concatenate de olmadıgı için böyle yapmak en mantıklısı
  //Aslında common.c de var bi strcat , ama çalışmadı bu yüzden bu şekilde yazdık
	monitor_write(c);
	monitor_put(39); // bakınız iki üst satır
	monitor_write(" is an invalid command.\n");
	monitor_write("Type 'help' for a list of available commands.\n");
}
static void errorHandler(char *c) {
  if(c[1]=='\t')
  {
    if(c[0]=='c')
    {
      monitor_clear();
      monitor_command_prompt();
    }
    else if(c[0]=='h')
    {
      monitor_write("help\n");
      helpOption();
    }
    else if(c[0]=='r')
    {
       rebootOption();
    }
    else{
      error(c);
    }
  }
  else {
    error(c);
  }
}
static void addOption(char *c) { add 25 45
  monitor_write("Sum is : ");
  monitor_write_dec( (10*(c[4]-48)) + (c[5]-48) + (10*(c[7]-48)) + (c[8]-48) );
  monitor_write("\n");
}
static void subOption(char *c) {
  monitor_write("Sub is : ");
  monitor_write_dec( (10*(c[4]-48)) + (c[5]-48) - ((10*(c[7]-48))  + (c[8]-48)));
  monitor_write("\n");
}
static void setTimeOption(char *c)
{
	u8int h,m,s;
	h=(c[1]-48)*10+(c[2]-48);
	m=(c[4]-48)*10+(c[5]-48);
	s=(c[7]-48)*10+(c[8]-48);
	if(((c[0]!=' ') || (c[3]!=' ')) || (c[6]!=' '))
	{
		errorSetTime();
	}
	else if(h<24 && ( m<60 && s<60)) {

		setTime(h,m,s);
    c='\0';
  }
	else {
		errorSetTime();
  }
}

void checkCommand(char *c)
{
	u8int i=0;
	u8int c1[7];
	u8int c2[9];
  u8int c3[3] = "";

  c3[0] = c[0];
  c3[1] = c[1];
  c3[2] = c[2];
  if(strcmp(c,"clear")==0) {
      monitor_clear();
      monitor_command_prompt();
  }
  else if(strcmp(c3,"add")==0) {
    addOption(c);
  }
  else if(strcmp(c3,"sub")==0) {
    subOption(c);
  }
	else if(strcmp(c,"help")==0) {
		helpOption();
  }
	else if(strcmp(c,"reboot")==0) {
		rebootOption();
  }
	else
	{
		for(i=0;i<7;i++)
		{
			c1[i]=c[i];
			c2[i]=c[i+7];
		}
		c1[i]=0;
		c2[7]=c[14];
		c2[8]=c[15];
		if(strcmp(c1,"settime")==0) {
			setTimeOption(c2);
    }
		else {
			errorHandler(c);
    }
	}
}
