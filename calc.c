
// C program for array implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

extern int func_add;
extern int func_sub;
extern int func_mul;
extern int func_div;

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
int get_num_of_digits(int num);
void add_zero(bignum* target, int  to_be_array_size);
void free_stack(struct Stack* stack);
bignum* interrior_sub(bignum* big, bignum* small);
bignum* interrior_add(bignum* big, bignum* small);
void add_sign (bignum* res);
int fix_negative (bignum* num);
int bigger_digits (bignum* num1, bignum* num2);
int add_func(int num1 , int num2 , int carry);
int sub_func(int num1 , int num2 , int borrow);


int main(int argc, char *argv[]){
    char c;
    struct Stack* stack = createStack(1024);
    FILE * input = stdin;
    char* acc = (char*)malloc(sizeof(char));
    acc[0] = 0;
    int counter = 0;
    while((c = fgetc(input)) != EOF){
        if((48<= c)&&(c<=57)){
            counter++;
            acc = (char*)realloc(acc, (counter+1)*sizeof(char));
            acc[counter -1] = c;
            acc[counter] = 0;
        }
        if(c <= 32){
            if (0 != counter){
              copy_and_push(&counter, acc, stack);
            //   free (acc);
            //   acc = (char*)malloc(sizeof(char));
            memset(acc,0,counter + 1);
              counter = 0;
            }
        }
        if ((c =='+')||(c =='-')||(c =='*')||(c=='/')||(c =='p')||(c == 'q')){
            if ((c !='p') && (c != 'q')){
                 if(0!= counter){
                 copy_and_push(&counter, acc, stack);
                memset(acc,0,counter + 1);
                    counter = 0;
              }
                bignum* num1 = pop(stack);
                bignum* num2 = pop(stack);
                printf("num1 is %s\n",num1->digit);
                printf("num2 is %s\n",num2->digit);
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
                       printf("push p: %s", acc);
                       copy_and_push(&counter, acc, stack);
                    //    free (acc);
                    //    acc = (char*)malloc(sizeof(char));
                    memset(acc,0,counter + 1);
                       counter = 0;
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
                int len;
                len = strlen(acc);
                char* digits = (char*)malloc(len*sizeof(char)+1);
                strcpy(digits, acc);
                struct bignum* next_num = createBignum(digits,len);
                push(stack, next_num);
                *counter = 0;

}

void calc(struct Stack* stack , bignum* num1 , bignum* num2 ,char op){
    int is_num1_negative = fix_negative(num1);
    int is_num2_negative = fix_negative(num2);
    int big = bigger_digits(num1,num2);
    break_into_chuncks(num1);
    break_into_chuncks(num2);
    if (big == 1){add_zero(num2, (num1->array_size));}
			else{add_zero(num1, (num2->array_size));}
    bignum* res;
    switch(op){
		    case '+':
            if ((!is_num1_negative) && (!is_num2_negative)){
              res = interrior_add(num1,num2);
            }
            else if (is_num1_negative && is_num2_negative){
              res = interrior_add(num1,num2);
              add_sign(res);
            }
            else {
              printf(" enter one of them is negative");
              if (big == 1){res = interrior_sub(num1,num2);}
              else {res = interrior_sub(num2,num1);}
              if ((is_num1_negative && (big==1)) || (is_num2_negative && (big ==2))){
                add_sign(res);
              }
            }
             push(stack, res);
             break;

        case '-':
            if ((!is_num1_negative) && (!is_num2_negative)){
                if (big == 1){
                  res = interrior_sub(num1,num2);
                  add_sign(res);
                }
                else {
                  res = interrior_sub(num2,num1);
                }
            }
            else if ((is_num1_negative) && (is_num2_negative)){
              if (big == 1){
                  res = interrior_sub(num1,num2);
                }
              else {
                res = interrior_sub(num2,num1);
                add_sign(res);
              }
            }
            else {  // just one of them is negative  -7 +6
              res = interrior_add(num1,num2);
              if (is_num2_negative){
                add_sign(res);
              }
            }
             push(stack, res);
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
    acc[8] = 0;
    last_acc[8] = 0;
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
        printf("array in %d , is:  %ld " , i , bignum->array[i]);
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

bignum* interrior_sub(bignum* big, bignum* small){ // gets 2 bignums after rappeding by zero and sign deletion
  printf("small array in 0 is    %s\n", small->digit);
  int ans_num_of_digits = 0;
  int total_num_of_digits = 0;
  int flag = 1;
  char* digit = (char*)malloc((sizeof(char)));
  char* digit_tmp = (char*)malloc((sizeof(char)));
  digit[0] = 0;
  digit_tmp[0] = 0;
  int bigger_num_array_size = big->array_size;
  int carry = 0;
  for(int i = 0 ; i < bigger_num_array_size ; i++){
      flag = 1;
      printf("big array in 0 is    %d\n", big->array[i]);
      printf("small array in 0 is    %d\n", small->array[i]);
      int ans = (big->array[i])-(small->array[i]) - carry;
      //int ans = sub_func(big->array[i],small->array[i] , carry);
      printf("\n  enter sub  1 ans is before: %d\n",ans);
            if(ans < 0){
                printf("\n  enter sub 2 ans is before: %d\n",ans);
                ans = add_func(ans, pow(10,8), 0);
                carry = 1;
            }else if(ans == 0){
                total_num_of_digits = total_num_of_digits + 8;
                char str[total_num_of_digits+1];
                for(int i = 0; i <= total_num_of_digits; i++){
                    str[i] = 0;
                }
                strcpy(str,"00000000");
                digit = (char*)realloc(digit ,(total_num_of_digits+1)*(sizeof(char)));
                digit[total_num_of_digits] = 0;
                carry = 1;
                flag = 0;
                strcat(str,digit);
                strcpy(digit,str);
            }
            else{carry = 0;}
            if(flag){
             ans_num_of_digits = get_num_of_digits(ans);
              total_num_of_digits = total_num_of_digits + ans_num_of_digits;
              digit_tmp = (char*)realloc(digit_tmp ,(total_num_of_digits + 1)*(sizeof(char)));
              digit = (char*)realloc(digit ,(total_num_of_digits+1)*(sizeof(char)));
              digit[total_num_of_digits] = 0;
              digit_tmp[total_num_of_digits] = 0;
              for(int i = 0; i < (total_num_of_digits + 1);i++ ){
                  digit_tmp[i] = 0;
              }
              printf("i is: %d\n", i);
              printf("ans is: %d\n",ans);
              sprintf(digit_tmp, "%d", ans);
              strcat(digit_tmp,digit);
              strcpy(digit,digit_tmp);
              strcpy(digit_tmp,"");
            }
            }
           bignum* res = createBignum(digit, total_num_of_digits);
           free(digit_tmp);
           return res;
  }

bignum* interrior_add(bignum* big, bignum* small){ // gets 2 bignums after rappeding by zero and sign deletion
  int ans_num_of_digits = 0;
  int total_num_of_digits = 0;
  int flag = 1;
  char* digit = (char*)malloc((sizeof(char)));
  char* digit_tmp = (char*)malloc((sizeof(char)));
  digit[0] = 0;
  digit_tmp[0] = 0;
  int bigger_num_array_size = big->array_size;
  int carry = 0;
  for(int i = 0 ; i < bigger_num_array_size ; i++){
      flag = 1;
      int ans = add_func(big->array[i],small->array[i] , carry);
      printf("\n enter add -- ans is before: %d\n",ans);
            if(ans > pow(10,8)){
                ans = sub_func(ans, pow(10,8), 0);
                carry = 1;
            }else if(ans == pow(10,8)){
                total_num_of_digits = total_num_of_digits + 8;
                char str[total_num_of_digits+1];
                for(int i = 0; i <= total_num_of_digits; i++){
                    str[i] = 0;
                }
                strcpy(str,"00000000");
                digit = (char*)realloc(digit ,(total_num_of_digits+1)*(sizeof(char)));
                digit[total_num_of_digits] = 0;
                carry = 1;
                flag = 0;
                strcat(str,digit);
                strcpy(digit,str);
            }
            else{carry = 0;}
            if(flag){
             ans_num_of_digits = get_num_of_digits(ans);
              total_num_of_digits = total_num_of_digits + ans_num_of_digits;
              digit_tmp = (char*)realloc(digit_tmp ,(total_num_of_digits + 1)*(sizeof(char)));
              digit = (char*)realloc(digit ,(total_num_of_digits+1)*(sizeof(char)));
              digit[total_num_of_digits] = 0;
              digit_tmp[total_num_of_digits] = 0;
              for(int i = 0; i < (total_num_of_digits + 1);i++ ){
                  digit_tmp[i] = 0;
              }
              printf("i is: %d\n", i);
              printf("ans is: %d\n",ans);
              sprintf(digit_tmp, "%d", ans);
              strcat(digit_tmp,digit);
              strcpy(digit,digit_tmp);
              strcpy(digit_tmp,"");
            }
            }
           bignum* res = createBignum(digit, total_num_of_digits);
           free(digit_tmp);
           return res;
}

void add_sign (bignum* res){
  char* digit_tmp = (char*)malloc(2+(sizeof(res->digit)));
  for(int i = 0; i < (2+(sizeof(res->digit))) ; i++ ){
      digit_tmp[i] = 0;
  }  
  strcpy(digit_tmp,"_");
  strcat(digit_tmp,res->digit);
  strcpy(res->digit,digit_tmp);
  free(digit_tmp);
  res->number_of_digits++;
}

int fix_negative (bignum* num){
  int res = 0;
  if ('_' ==(num->digit[0])){//changed from '-' to '_'
    res=1;
    num->number_of_digits--;
    num->digit[0]='0';
  }
  return res;
}

int bigger_digits (bignum* num1, bignum* num2){
  int res =1;
  if ((num2->number_of_digits)>(num1->number_of_digits)){ res =2;}

  if ((num2->number_of_digits)==(num1->number_of_digits)){
    char* num1_pre = (char*)malloc(3);
    num1_pre[0]=num1->digit[0];
    num1_pre[1]=num1->digit[1];
    num1_pre[2] = 0;
    char* num2_pre = (char*)malloc(3);
    num2_pre[0]=num2->digit[0];
    num2_pre[1]=num2->digit[1];
    num2_pre[2] = 0;
    int num1_pre_int= atoi(num1_pre);
    int num2_pre_int= atoi(num2_pre);
    if(num2_pre_int > num1_pre_int){res=2;}
    free (num1_pre);
    free(num2_pre);
  }
  return res;


}
