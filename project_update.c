#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

unsigned sleep(unsigned sec);

struct cpustat {
    unsigned long t_user;
    unsigned long t_nice;
    unsigned long t_system;
    unsigned long t_idle;
    unsigned long t_iowait;
    unsigned long t_irq;
    unsigned long t_softirq;
};

void skip_lines(FILE *fp, int numlines)
{
    int cnt = 0;
    char ch;
    while((cnt < numlines) && ((ch = getc(fp)) != EOF))
    {
        if (ch == '\n')
            cnt++;
    }
    return;
}

void Reader(struct cpustat *st, int cpunum)
{
    
    FILE *fp = fopen("/proc/stat", "r");
    int lskip = cpunum+1;
    skip_lines(fp, lskip);
    char cpun[255];
    //thread_mutex cpun[]
    fscanf(fp, "%s %lu %lu %lu %lu %lu %lu %lu", cpun, &(st->t_user), &(st->t_nice), 
        &(st->t_system), &(st->t_idle), &(st->t_iowait), &(st->t_irq),
        &(st->t_softirq));
    fclose(fp);
    //thread_mutex cpun[]
	return;
}

void Printer(struct cpustat *st, char *name)
{
    printf("%s: %lu %lu %lu %lu %lu %lu %lu\n", name, (st->t_user), (st->t_nice), 
        (st->t_system), (st->t_idle), (st->t_iowait), (st->t_irq),
        (st->t_softirq));
}

double Analyzer(struct cpustat *prev, struct cpustat *cur)
{
    int idle_prev = (prev->t_idle) + (prev->t_iowait);
    int idle_cur = (cur->t_idle) + (cur->t_iowait);

    int nidle_prev = (prev->t_user) + (prev->t_nice) + (prev->t_system) + (prev->t_irq) + (prev->t_softirq);
    int nidle_cur = (cur->t_user) + (cur->t_nice) + (cur->t_system) + (cur->t_irq) + (cur->t_softirq);

    int total_prev = idle_prev + nidle_prev;
    int total_cur = idle_cur + nidle_cur;

    double totald = (double) total_cur - (double) total_prev;
    double idled = (double) idle_cur - (double) idle_prev;

    double cpu_perc = (1000 * (totald - idled) / totald + 1) / 10;

    return cpu_perc;
}

void Watchdog(int &th1_check, int &th2_check ,int &th3_check, int th4_check,char* &msg_th){
	time_t temp = time(NULL);
    loop((time(NULL)-Tmp)>2){
	if(thread1==1){assert("thread1 Error");temp = time(now);thread--;}
	
	if(thread2==1){assert("thread2 Error");temp = time(now);thread--;}
	
	if(thread3==1){assert("thread3 Error");temp = time(now);thread--;}
	
	if(thread4==1){assert("thread4 Error");temp = time(now);thread--;}
    }	
    	
	msg_th= "The watchdog didn't recieve any message from the threads for the last 2s.";
	assert("The watchdog didn't recieve any message from the threads for the last 2s.");
}

void Logger(char* &msg_th, int &th4_check),{
    FILE *fp = fopen("Desktop/project/logger", "w+");
    for(;;){
    fprintf(fp, "%s ",msg_th,"/n");
    	if(th4_check==-1){
	break;
    	}
    }
    fclose(fp);
}    


int main (void)
{
    struct cpustat st0_0, st0_1;
    while (1)
    {
        Reader(&st0_0, -1);
        sleep(1);
        Reader(&st0_1, -1);
        printf("CPU: %lf%%\n", analyzer(&st0_0, &st0_1));
    }
    return 0;
}
