// Jan Mazur
// 281141

// function "timeval_subtract" taken fom http://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html

#include "utils.h"


/* Subtract the ‘struct timeval’ values X and Y,
   storing the result in RESULT.
   Return 1 if the difference is negative, otherwise 0. */

int timeval_subtract(struct timeval *result, struct timeval *x, 
    struct timeval *y){
    /* Perform the carry for the later subtraction by updating y. */
    if(x->tv_usec < y->tv_usec){
        int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
        y->tv_usec -= 1000000 * nsec;
        y->tv_sec += nsec;
    }
    if(x->tv_usec - y->tv_usec > 1000000){
        int nsec = (x->tv_usec - y->tv_usec) / 1000000;
        y->tv_usec += 1000000 * nsec;
        y->tv_sec -= nsec;
    }

    /* Compute the time remaining to wait.
     tv_usec is certainly positive. */
    result->tv_sec = x->tv_sec - y->tv_sec;
    result->tv_usec = x->tv_usec - y->tv_usec;

    /* Return 1 if result is negative. */
    return x->tv_sec < y->tv_sec;
}

void print_partial_result(char sender_ip_strings[][20], 
    struct timeval* czasy, int got, int ttl){

    if(got == 0){
        printf("%d. *\n",ttl);
        return;
    }

    printf("%d. ",ttl);

    for(int i=0; i<got; i++){
        if(i == 1 && strcmp(sender_ip_strings[0],sender_ip_strings[1]) == 0) continue;
        if( (i == 2 && strcmp(sender_ip_strings[1],sender_ip_strings[2]) == 0 )
            || (i==2 && strcmp(sender_ip_strings[0],sender_ip_strings[2]) == 0 )) continue;
        printf("%s ",sender_ip_strings[i]);
    }
  
  /*
   *      "W przypadku nieotrzymania trzech odpowiedzi w ustalonym
   *      czasie zamiast średniego czasu odpowiedzi należy wyświetlić ???.""
   *      Czyli jeśli dostanę 2 lub 1 odpowiedź ( nie ważne czy od tego samego routera czy od innych),
          w wyznaczonym czasie wyświetlam ???. Wszystkie pakiety które dochodzą po czasie są ignorowane.

          Jeśli jednak miałbym wyświetlać czasy jakoś inaczej to wymaga to zmodyfikowania dosłownie jednego warunku.
   */

    if(got == 1 || got == 2){
        printf("???\n");
        return; 
    }

    int srednia = 0;
    for(int i=0; i<got; i++){
       srednia += czasy[i].tv_usec / 1000;
    }
    srednia /= got;

    printf("%dms\n",srednia);

}
