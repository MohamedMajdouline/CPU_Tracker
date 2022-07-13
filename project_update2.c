#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

unsigned sleep(unsigned sec);

struct cpustat {
    unsigned long t_user=0;
    unsigned long t_nice=0;
    unsigned long t_system=0;
    unsigned long t_idle=0;
    unsigned long t_iowait=0;
    unsigned long t_irq=0;
    unsigned long t_softirq=0;
    
};

int th1_check=0, th2_check=0 ,th3_check=0, th4_check=0;

double cpu_perc=0.0;

void reset_check(){
th1_check=0; th2_check=0 ;th3_check=0; th4_check=0;
}

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
    fscanf(fp, "%s %lu %lu %lu %lu %lu %lu %lu", cpun, &(st->t_user), &(st->t_nice), 
        &(st->t_system), &(st->t_idle), &(st->t_iowait), &(st->t_irq),
        &(st->t_softirq));
    fclose(fp);
    th1_check++;
	return;
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

    cpu_perc = (1000 * (totald - idled) / totald + 1) / 10;
    th2_check++;
    

    return;
}

void Printer()
{
    printf("CPU: %lf%%\n", cpu_perc);
    th3_check++;
    return;
}


void Logger(char &msg_th, int &th4_check),{
    FILE *fp = fopen("Desktop/project/logger", "w+");
    if(th1_check==1){fprintf(fp, "%s ","Reader is okay","/n");}
	
    if(th2_check==1){fprintf(fp, "%s ","Analyzer is okay","/n");}
	
    if(th3_check==1){fprintf(fp, "%s ","Printer is okay","/n");}  
    
    fclose(fp);
    fprintf(fp, "%s ","Logger is okay","/n");
    th4_check++;
    return;
}    

void Watchdog(int &th1_check, int &th2_check ,int &th3_check, int &th4_check){
	time_t tmp = time(NULL);
        loop((time(NULL)-tmp)<2){
	if(th1_check==1){printf("Reader is okay");}
	
	if(th2_check==1){printf("Analyzer is okay.");}
	
	if(th3_check==1){printf("Printer is okay");}
	
	if(th4_check==1){printf("Logger is okay");}
	
	if((th1_check*th2_check*th3_check*th4_check)==1){
		tmp = time(now);
		reset_check();
		}
        }else assert("The watchdog didn't recieve any messages from the threads for the last 2s.");
	
	return;
}

int main (void)
{
    struct cpustat st0_0, st0_1;
    while (1)
    {
        Logger();
        Watchdog();
        Reader(&st0_0, -1);
        sleep(1);
        Reader(&st0_1, -1);
        Analayzer();
        Printer();
    }
    return 0;
}
