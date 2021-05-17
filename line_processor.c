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


// PLEASE NOTE: GLOBALS ARE LARGELY TAKEN FROM THE 
// EXPLORATIONS WITH A FEW MODIFICATIONS.

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


// THREAD 4 GLOBAL
int printBuffer_size = 0;


// PLEASE NOTE: GET AND PUT BUFFERS ARE LARGELY TAKEN FROM THE 
// EXPLORATIONS WITH A FEW MODIFICATIONS. 

// Consumer Side
void get_buff_1(char *item){
    // Lock the mutex before putting the item in the buffer
    pthread_mutex_lock(&mutex_1); //shared resource
    while(count_1 == 0)
    {
        //if nothing in buffer, wait until producer tells us something there
        pthread_cond_wait(&full_1, &mutex_1);
    }
    // Put the item in the buffer
    strcpy(item, buffer1[cond_idx_1]); 
    // Decrement the index where we are getting.
    cond_idx_1 = cond_idx_1 + 1;
    count_1--;
    // Unlock the mutex
    pthread_mutex_unlock(&mutex_1);

}

// Production side of buffer 1
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
    pthread_mutex_lock(&mutex_2); 
    while(count_2 == 0)
    {
        //if nothing in buffer, wait until producer tells us something there
        pthread_cond_wait(&full_2, &mutex_2);
    }
    // Put the item in the buffer
    strcpy(item, buffer2[cond_idx_2]); 
    // Decrement the index where we are getting.
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
        //if nothing in buffer, wait until producer tells us something there
        pthread_cond_wait(&full_3, &mutex_3); 
    }
    // Put the item in the buffer
    strcpy(item, buffer3[cond_idx_3]); 
    // Decrement the index where we are getting.
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

// Expand the token from ++ for ^
char* expandToken(char* token) 
{
    char* term = "++";
	char *replace = "^";
	
    if (!strstr(token, term)) 
    {
        return token;
    }
    // else ++ is found
    char* buf = malloc(strlen(token) + 1);
    char* p = NULL;
    char* rest = token; 
        
    // while a "++" is present in the string
    while ((p = strstr(rest, term)))
    {
        // Move everything before the ++ to the buffer
        strncat(buf, rest, (size_t)(p - rest));
        // Append the caret onto the buffer in the place of ++
        strcat(buf, replace);
        // rest is now everything after the first ++ is found. Loop back to check for more
        rest = p + (strlen(replace) + 1);
    }
    // If no more "++" found, append what's leftover in rest to the buffer  
    strcat(buf, rest);
    return buf;
}

// producer thread -  get the input
void *t1(void *args)
{
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

// remove the new line
void *t2(void *args)
{
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
            input[strcspn(input, "\n")] = 0; // remove new line
            strcat(input, " ");  // add a space
        }
        put_buff_2(input); // put output into buffer 2
    }
    return NULL;
}

// check for "++" in input
void *t3(void *args)
{
    int stop = 0;
    char *input = malloc(1024);

    while(!stop)
    {
        get_buff_2(input); // get stuff out of buff 2
        if(strcmp(input, "STOP\n") == 0)
        {
            stop = 1;
        }
        // Send to check for ++ to possible expand into ^
        char* expanded = expandToken(input);
        strcpy(input, expanded);
        put_buff_3(input); // put output into buffer 3
    }
    return NULL;
}

void *t4(void *args){
    int stop = 0;
    char *input = malloc(1024);
    char print_buffer[50*1024];
    int line_char_count = 0;
    int cache_j;
    int last_line_printed = -1;

    while(!stop)
    {
        get_buff_3(input); // get stuff out of buff 3
        strcat(print_buffer, input); // cat input to print buffer
        if(strcmp(input, "STOP\n") == 0)
        {
            stop = 1;    
        }
        // get the length of the buffer
        printBuffer_size += strlen(input);
     
        // Loop though the entire buffer
        for(int j = cache_j; j < printBuffer_size; j++)
        {
            // increment the character count for the line
            line_char_count += 1;
            
            if(line_char_count == 80)
            {
                int end_of_line = line_char_count + last_line_printed;
                last_line_printed = last_line_printed + 1;

                // print each character starting at the last line printed
                // and increment until end of line is hit
                while(last_line_printed <= end_of_line)
                {
                    printf("%c", print_buffer[last_line_printed]);
                    last_line_printed += 1;
                }
                // 80 characters have been reached so
                // ...set the last line
                // ...reset the character count for the line
                printf("\n");  
                last_line_printed = j;
                line_char_count = 0;

            }
        // next
        cache_j = j + 1;    
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

    return 0;
}