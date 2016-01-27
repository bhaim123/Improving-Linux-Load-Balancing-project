#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/cpumask.h>
#include <../kernel/sched/sched.h>
#include <linux/time.h>
#include <linux/unistd.h>
#include <linux/timer.h>

#define MAX_SAMPLES 1000
#define CORENUM 16
#define SLEEP_TIME 3

static struct timer_list my_timer;
void get_cpu_workload(int nothing);

int roundNumber=0;
int roundsToPerform = 0;
int buffer[CORENUM][MAX_SAMPLES];

asmlinkage long sys_cpus_check_workload(int input1, int input2)
{
	int ret=-5;

	roundNumber=0;

	if (input1 == -1)
	{
		printk("boaz- going to call get_cpu_workload\n");
		if (input2 < 1)
			roundsToPerform =5;
		else
			roundsToPerform = input2;
		get_cpu_workload(0);
		printk("boaz-done with get_cpu_workload, ret was %d\n",ret);
		boaz_sighn_to_print2 =0;
	}
	if(input1 >0)
	{
		printk("boaz- input 1 is %d and input2 is %d \n",input1,input2);
		printk("boaz- going to delete timer\n");
		ret =del_timer(&my_timer);
		printk("boaz- timer was del with %d returnd\n",ret);
	}
	return 0;
} 

void get_cpu_workload(int nothig)
{
	int cpu;
	struct rq *curRq;
	int i=0;
	int curRunablle=0;
	int ret=-5;

	for_each_present_cpu(cpu)
	{
		i++;
		curRq=cpu_rq(cpu);
		curRunablle=curRq->nr_running;
		printk("boaz in get_cpu_workload-round %d:cpu num is %d and number of proc is %d\n",roundNumber,i,curRunablle);
	}
	if (roundNumber < roundsToPerform)
	{
		setup_timer(&my_timer,get_cpu_workload,0);
		ret = mod_timer(&my_timer,jiffies + msecs_to_jiffies(SLEEP_TIME));
	}
	printk("boaz in get_cpu_workload- roundNumber is %d\n",roundNumber);
	roundNumber++;
}

