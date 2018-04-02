
// C program for array implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

extern fanc_add;
extern fanc_sub;
extern fanc_mul;
extern fanc_div;

 typedef struct bignum {
    long number_of_digits;
    char *digit;
    long* array;
    int array_size;
} bignum;

// A structure to represent a stack
struct Stack{
    int top;
    unsigned capacity;
    bignum** array;
};



 struct bignum* createBignum(char* _digit, long _number_of_digits){
        struct bignum* _bignum = (struct bignum*) malloc(sizeof(struct bignum));
        _bignum->digit = _digit;
        _bignum->number_of_digits=_number_of_digits;
        _bignum->array =  (long*)malloc(sizeof(long));
        _bignum->array_size = 0;
        return _bignum;
}

// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack* createStack(unsigned capacity){
    struct Stack* stack = (struct Stack*) malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (bignum**) malloc(sizeof(bignum*)*1024+1);
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack* stack){
    return stack->top == stack->capacity - 1; }

// Stack is empty when top is equal to -1
int isEmpty(struct Stack* stack)
{   return stack->top == -1;  }

// Function to add an item to stack.  It increases top by 1
void push(struct Stack* stack, bignum* item){
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;

}

// Function to remove an item from stack.  It decreases top by 1
bignum* pop(struct Stack* stack){

    return stack->array[stack->top--];
}

bignum* peek(struct Stack* stack){

    return stack->array[stack->top];
}

void printStack(struct Stack* stack);
void copy_and_push(int* counter, char* acc, struct Stack* stack);
void print_array(bignum* bignum);
void calc(struct Stack* stack , bignum* num1 , bignum* num2 ,char op);
void break_into_chuncks(bignum* bignum);
int asm_add_func(int num1 ,int num2);
int get_num_of_digits(int num);
void add_zero(bignum* target, int  to_be_array_size);
void free_stack(struct Stack* stack);

int main(int argc, char *argv[]){
    char c;
    struct Stack* stack = createStack(1024);
    FILE * input = stdin;
    char* acc = (char*)malloc(sizeof(char));
    int counter = 0;
    while((c = fgetc(input)) != EOF){
        if((48<= c)&&(c<=57)){
        counter++;
        acc = (char*)realloc(acc, (counter+1)*sizeof(char));
        acc[counter-1] = c ;
        }
        if(c <= 32){
            if (0 != counter){
              copy_and_push(&counter, acc, stack);
            }
        }
        if ((c =='+')||(c =='-')||(c =='*')||(c=='/')||(c =='p')||(c == 'q')){
         if ((c !='p') && (c != 'q')){
             if(0!= counter){
                copy_and_push(&counter, acc, stack);}
                bignum* num1 = pop(stack);
                bignum* num2 = pop(stack);
                char op = c;
                calc(stack, num1,num2,op);
                free(num1->array);
                free(num2->array);
                free(num1->digit);
                free(num2->digit);
                free(num1);
                free(num2);
          }
            else if (c =='p'){
                   if (0 != counter){
                    copy_and_push(&counter, acc, stack);
                    }
                    bignum* new_peek = peek(stack);
                    printf("%s\n",new_peek->digit);
                    //break_into_chuncks(new_peek);
                    //print_array(new_peek);
					//printf("print stack: \n");
					//printStack(stack);
            }
            else if (c == 'q'){
                free_stack(stack);
                free(stack);
                free(acc);
                exit(0);
            }

        }
    }
    free(stack);
    free(acc);
    return 0;
}




void printStack(struct Stack* stack){
  for(int i = stack->top; i >= 0; i--){
        printf("%s\n",(stack->array[i]->digit));
}
}

void copy_and_push(int* counter, char* acc, struct Stack* stack){
                char* digits = (char*)malloc((*counter+1)*(sizeof(char)));
//                 for(int i = 0; i < (*counter) ;i++){
//                     digits[i] = acc[i];
//                 }
                strcpy(digits, acc);
                int size =  strlen(digits);
                struct bignum* next_num = createBignum(digits,size);
                push(stack, next_num);
                *counter = 0;

}

void calc(struct Stack* stack , bignum* num1 , bignum* num2 ,char op){
    break_into_chuncks(num1);
    break_into_chuncks(num2);
    int bigger_num_array_size = num1->array_size;
	int bigger = 1;
    if(num1->array_size < num2->array_size){
        bigger_num_array_size = num2->array_size;
		bigger = 2;
    }
   // bignum* res = createBignum("" , 0);
    int carry = 0;
    int ans_num_of_digits = 0;
    int total_num_of_digits = 0;
    char* digit;
    char* digit_tmp;
    switch(op){
		case '+':
                digit = (char*)malloc((sizeof(char)));
                digit_tmp = (char*)malloc((sizeof(char)));
			if (bigger == 1){add_zero(num2, bigger_num_array_size);}
			else{add_zero(num1, bigger_num_array_size);}
			for(int i = 0 ; i < bigger_num_array_size ; i++){
				int ans = add_func(num1->array[i],num2->array[i] , carry);
				if(ans >= pow(10,8)){
					ans = ans - pow(10,8);
					carry = 1;
				}else{carry = 0;}
				 ans_num_of_digits = get_num_of_digits(ans);
          total_num_of_digits = total_num_of_digits + ans_num_of_digits;
          digit_tmp = (char*)realloc(digit_tmp ,(total_num_of_digits+1)*(sizeof(char)));
          digit = (char*)realloc(digit ,(total_num_of_digits+1)*(sizeof(char)));
          sprintf(digit_tmp, "%d", ans);
          strcat(digit_tmp,digit);
          strcpy(digit,digit_tmp);
          strcpy(digit_tmp,"");
				} 
             bignum* res = createBignum(digit , total_num_of_digits);
            //  res->digit = digit;
            //  res->number_of_digits = total_num_of_digits;
             push(stack, res);
             free(digit_tmp);
             break;

        case '-':
        break;
        case '*':
        break;
        case '/':
        break;

         }




}

void break_into_chuncks(bignum* bignum){
    int counter = 7 ;
    char* acc = (char*)malloc(9*sizeof(char));
    char* last_acc = (char*)malloc(9*sizeof(char));
    int num_of_digits = bignum->number_of_digits;
    unsigned int res;
    int place_in_array = 0;
    for(int i=(num_of_digits-1); i>=0;i--){
        if((counter <= 7) ){
          acc[counter] = bignum->digit[i];
          counter --;
        }
        if(counter == (-1)){
            counter = 7 ;
             res = atol(acc);
              bignum->array_size ++;
            bignum->array = realloc(bignum->array, (bignum->array_size)*sizeof(long));
            (bignum->array[place_in_array++] ) = res;


        }
        if ( i == 0){
           for(int i=0; i<8;i++){last_acc[i]='0';}
            for (int i=(counter+1);i<=7;i++){
                last_acc[i]=acc[i];
            }
              res = atol(last_acc);
              bignum->array_size ++;
              bignum->array =  realloc(bignum->array, (bignum->array_size)*sizeof(long));
            (bignum->array[place_in_array++] ) = res;

        }
    }
    free(acc);
    free(last_acc);
}

void print_array(bignum* bignum){
    for(int i = 0; i < (bignum->array_size) ; i++){
        printf("array in %d , is:  %d " , i , bignum->array[i]);
    }
}

int get_num_of_digits(int num){
 int counter = 0;
 while(num !=0){
     num = (num/10);
     counter++;
  }
 return counter;
  }

void add_zero(bignum* target, int  to_be_array_size){
        target->array = (long*)realloc(target->array,(to_be_array_size+1)*sizeof(long));
		for (int i = (target->array_size); i < to_be_array_size ; i++){
		target->array[i] = 0;
		(target-> array_size)++;
		}
}

void free_stack(struct Stack* stack){
    while(!isEmpty(stack)){
        bignum* num = pop(stack);
        free(num->array);
        free(num->digit);
        free(num);
    }
    free(stack->array);
}
