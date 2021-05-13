#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

#define NUM_LINES 50
#define LINE_LENGTH 1024

// globals
char buffer1[NUM_LINES][LINE_LENGTH] = {{0}}; 
char buffer2[NUM_LINES][LINE_LENGTH] = {{0}};
char buffer3[NUM_LINES][LINE_LENGTH] = {{0}};


// BUFFER 1
int prod_idx_1 = 0;
int cond_idx_1 = 0;
int count_1 = 0;

pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer1
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

// BUFFER 2
int prod_idx_2 = 0;
int cond_idx_2 = 0;
int count_2 = 0;

pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffeR2
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;


// BUFFER 3
int prod_idx_3 = 0;
int cond_idx_3 = 0;
int count_3 = 0;

pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
// Initialize the condition variable for buffer3
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;


// Thread 4
//char print_buffer[50*1024];
//char print_buffer[NUM_LINES][LINE_LENGTH] = {{0}};
//char print_buffer[5000] = {{0}};
int print_idx = 0;
int print_count = 0;
int printBuffer_size = 0;
int string_count = 0;




void get_buff_1(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_1); //shared resource
    while(count_1 == 0)
    {
        pthread_cond_wait(&full_1, &mutex_1); //if nothing in buffer, wait until producer tells us something there
    }
    // Put the item in the buffer
    strcpy(item, buffer1[cond_idx_1]); 
    // Increment the index where the next item will be put.
    cond_idx_1 = cond_idx_1 + 1;
    count_1--;
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_1);

}

/*
 Put an item in buff_1
*/
//Production side of buffer 1 -- still needs consumer side of buff 1
void put_buff_1(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_1);
    // Put the item in the buffer
    strcpy(buffer1[prod_idx_1], item); // check what row and cpy to row
    // Increment the index where the next item will be put.
    prod_idx_1 = prod_idx_1 + 1;
    count_1++;
    // Signal to the consumer that the buffer is no longer empty
    pthread_cond_signal(&full_1);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_1);
}

void get_buff_2(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_2); //shared resource
    while(count_2 == 0)
    {
        pthread_cond_wait(&full_2, &mutex_2); //if nothing in buffer, wait until producer tells us something there
    }
    // Put the item in the buffer
    strcpy(item, buffer2[cond_idx_2]); 
    // Increment the index where the next item will be put.
    cond_idx_2 = cond_idx_2 + 1;
    count_2--;
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_2);

}

void put_buff_2(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_2);
    // Put the item in the buffer
    strcpy(buffer2[prod_idx_2], item); // check what row and cpy to row
    // Increment the index where the next item will be put.
    prod_idx_2 = prod_idx_2 + 1;
    count_2++;
    // Signal to the consumer that the buffer is no longer empty
    pthread_cond_signal(&full_2);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_2);
}

void get_buff_3(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_3); //shared resource
    while(count_3 == 0)
    {
        pthread_cond_wait(&full_3, &mutex_3); //if nothing in buffer, wait until producer tells us something there
    }
    // Put the item in the buffer
    strcpy(item, buffer3[cond_idx_3]); 
    // Increment the index where the next item will be put.
    cond_idx_3 = cond_idx_3 + 1;
    count_3--;
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_3);

}

void put_buff_3(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_3);
    // Put the item in the buffer
    strcpy(buffer3[prod_idx_3], item); // check what row and cpy to row
    // Increment the index where the next item will be put.
    prod_idx_3 = prod_idx_3 + 1;
    count_3++;
    // Signal to the consumer that the buffer is no longer empty
    pthread_cond_signal(&full_3);
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_3);
}

char* expandToken(char* token) {
    char* term = "++";
	char *replace = "^";
	
    if (!strstr(token, term)) {
        return token;
    }
    char* buf = malloc(strlen(token) + 1);
    char* p = NULL;
    char* rest = token;    
    while ((p = strstr(rest, term))) {
        //printf("This is rest: %s  This is term: %s \n", rest, term);
        strncat(buf, rest, (size_t)(p - rest));
        strcat(buf, replace);
        rest = p + (strlen(replace) + 1); //maybe the plus 1 helps
        //printf("This is rest2: %s  This is P: %s This is replace: %s\n", rest, p, replace);
    }    
    strcat(buf, rest);
    return buf;
}

// Print Buffer
void pb(char b[NUM_LINES][LINE_LENGTH]) 
{
    for(int i = 0; i < NUM_LINES; i++)
    {
        if(b[i][0] != '\0')
        {
            printf("%d | %s\n", i, b[i]);
        }
        else
        {
            break;
        }
    }
}

void print_t4(char p4[NUM_LINES][LINE_LENGTH])
{
int cache_i = 0;
int cache_j = 0;

int count = 0;
for(int i = cache_i; i < NUM_LINES ; i++)
{
    for ( int j = cache_j; j < LINE_LENGTH ; j++)
    {
        printf("THREAD 4: %c\n", p4[i][j]);
        count++;

        if(count >= 80)
        {
            count = 0;
            printf("\n");
            cache_i = i;
            cache_j = j;
        }
        else
        {
            break;
        }
    }
}
}


// producer thread
void *t1(void *args){
    int stop = 0;
    char *input = malloc(1024);
    while(!stop){
        fgets(input, 1024, stdin);
        if(strcmp(input, "STOP\n") == 0){
            stop = 1;
        }
        put_buff_1(input);
    }
    return NULL;
}


void *t2(void *args){
    int stop = 0;
    char *input = malloc(1024);
    while(!stop)
    {
        get_buff_1(input); // get stuff out of buff 1
        if(strcmp(input, "STOP\n") == 0)
        {
            stop = 1;
        }
        else
        {
            input[strcspn(input, "\n")] = 0; //remove /n // need to add a space I believe
            strcat(input, " ");
        }
        put_buff_2(input); // put output into buffer 2
    }
    return NULL;
}

void *t3(void *args){
    int stop = 0;
    char *input = malloc(1024);
    while(!stop)
    {
        get_buff_2(input); // get stuff out of buff 1
        if(strcmp(input, "STOP\n") == 0)
        {
            stop = 1;
        }
        char* expanded = expandToken(input);
        strcpy(input, expanded);
        // strcat(input, "*");
        // convert ++ to caret
        put_buff_3(input); // put output into buffer 2
    }
    return NULL;
}

void *t4(void *args){
    int stop = 0;
    char *input = malloc(1024);
    char print_buffer[50*1024];
    int arr_count = 0;
    int cache_j;
    //int string_count = 0;
    
    int last_line_printed = -1;
    //int printBuffer_size; // probably set to global

    while(!stop)
    {
        get_buff_3(input); // get stuff out of buff 3
        strcat(print_buffer, input);
        if(strcmp(input, "STOP\n") == 0)
        {
            stop = 1;    
        }
        // strcpy(print_buffer[print_idx], input);
        // print_idx = print_idx + 1;
        printBuffer_size += strlen(input);
        
        // printf("\n");
        // printf("Buffer is of %d size.\n", printBuffer_size);
        // printf("\n");
     
        for(int j = cache_j; j < printBuffer_size; j++)
        {
            // printf("\n");
            // printf("Starting J is: %d, starting Arr_count is: %d\n", j, arr_count);
            // printf("Starting Character is: %c\n", print_buffer[j]);
            // printf("\n");
            arr_count += 1;
            
            if(arr_count == 80)
            {
                int end_of_line = arr_count + last_line_printed;
                last_line_printed = last_line_printed + 1;
                // printf("we are in here\n");
                //printf("end of file is: %d Last Line Printed is: %d\n", end_of_line, last_line_printed);
                while(last_line_printed <= end_of_line)
                {
                    // printf("\n");
                    // printf("This is the character string:\n");
                    printf("%c", print_buffer[last_line_printed]);
                    // printf("\n");
                    last_line_printed += 1;
                }
                printf("\n");   
                last_line_printed = j;
                arr_count = 0;

            }
            //last_line_printed = j + 1;
            cache_j = j + 1;
            // printf("Ending J is: %d, (Cache J = %d) Ending Arr_count is: %d\n", j, cache_j, arr_count);
            
        }
        
    }
    return NULL;
}

  
int main()
{
  
    pthread_t input_t;
    pthread_t linesep_t;
    pthread_t replacePlus_t;
    pthread_t output_t;

    pthread_create(&input_t, NULL, t1, NULL);
    pthread_create(&linesep_t, NULL, t2, NULL);
    pthread_create(&replacePlus_t, NULL, t3, NULL);
    pthread_create(&output_t, NULL, t4, NULL);
    pthread_join(input_t, NULL);
    pthread_join(linesep_t, NULL);
    pthread_join(replacePlus_t, NULL);
    pthread_join(output_t, NULL);
    // pb(buffer1);
    // pb(buffer2);
    // pb(buffer3);
    //pb(print_buffer);

    return 0;
}