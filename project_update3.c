#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

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


pthread_mutex_t lock_1;
pthread_mutex_init(Lock_1,NULL);

int th1_check=0, th2_check=0 ,th3_check=0, th4_check=0;

double cpu_perc=0.0;

void reset_check(){

th1_check=0; th2_check=0 ;th3_check=0; th4_check=0;

}

void handle_signal(int sig){
	printf("The program has been terminated");
	exit("SYSTEM EXIT");
}

void skip_lines(FILE *fp, int numlines){

    int cnt = 0;
    char ch;
    while((cnt < numlines) && ((ch = getc(fp)) != EOF))
    {
        if (ch == '\n')
            cnt++;
    }
    return;
}

void* reader(struct cpustat *st, int cpunum){
    
    FILE *fp = fopen("/proc/stat", "r");
    
    int lskip = cpunum+1;
    skip_lines(fp, lskip);
    char cpun[255];
    
    mutex_lock(&lock1);
    
    fscanf(fp, "%s %lu %lu %lu %lu %lu %lu %lu", cpun, &(st->t_user), &(st->t_nice), 
        &(st->t_system), &(st->t_idle), &(st->t_iowait), &(st->t_irq),
        &(st->t_softirq));
        
    mutex_unlock(&lock1);
    fclose(fp);
    th1_check++;
	return;
}


void* analyzer(struct cpustat *prev, struct cpustat *cur){

    mutex_lock(&lock1);
    
    int idle_prev = (prev->t_idle) + (prev->t_iowait);
    int idle_cur = (cur->t_idle) + (cur->t_iowait);

    int nidle_prev = (prev->t_user) + (prev->t_nice) + (prev->t_system) + (prev->t_irq) + (prev->t_softirq);
    int nidle_cur = (cur->t_user) + (cur->t_nice) + (cur->t_system) + (cur->t_irq) + (cur->t_softirq);
    
    mutex_unlock(&lock1);

    int total_prev = idle_prev + nidle_prev;
    int total_cur = idle_cur + nidle_cur;

    double totald = (double) total_cur - (double) total_prev;
    double idled = (double) idle_cur - (double) idle_prev;

    cpu_perc = (1000 * (totald - idled) / totald + 1) / 10;
    
    th2_check++;   

    return;
}

void* printer(){

    printf("CPU: %lf%%\n", cpu_perc);
    th3_check++;
    return;
}


void* logger(){

    FILE *fp = fopen("Desktop/project/logger", "w+");
    
    if(th1_check==1){fprintf(fp, "%s ","Reader is okay","/n");}
	
    if(th2_check==1){fprintf(fp, "%s ","Analyzer is okay","/n");}
	
    if(th3_check==1){fprintf(fp, "%s ","Printer is okay","/n");}  
    
    fprintf(fp, "%s ","Logger is okay","/n");
    fclose(fp);
    th4_check++;
    return;
}    

void* watchdog(){

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
        }else {
        printf("The watchdog didn't recieve any messages from the threads for the last 2s.");
        handle_signal(int sig);
}
        
	
	return;
}


int main (void){

    struct cpustat st0_0, st0_1;
    struct signation sa;
    sa.sa_hundler= &handle_sigstp;
    sa.sa_flags = SA_RESTART;
    sigcation(SIGTERM,&sa,NULL);
    
    pthread_t Reader,Analyzer,Printer,Logger,Watchdog;
    
    while (1){
    
        pthread_create(&Logger,NULL,&logger(),NULL);
        pthread_create(&Watchdog,NULL,&watchdog(),NULL);
 	pthread_create(&Reader,NULL,&reader(),(&st0_0, -1));
 	
        sleep(1);
        
        pthread_create(&Reader,NULL,&reader(),(&st0_1, -1));
        pthread_create(&Analyzer,NULL,&analyzer(),(&st0_1, -1));
        pthread_create(&Printer,NULL,&printer(),(&st0_1, -1));
        
        pthread_join(&Logger,NULL);
        pthread_join(&Watchdog,NULL);
 	pthread_join(&Reader,NULL);
 	pthread_join(&Reader,NULL);
        pthread_join(&Analyzer,NULL);
 	pthread_join(&Printer,NULL);
 	
    }
    return 0;
}
