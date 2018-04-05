
// C program for array implementation of stack
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <math.h>

// #define SV_DEBUG(msg) do { \
// 	printf( "SV DEBUG %s " ,  __FILE__ ); printf(": %s " , __FUNCTION__); \
//   printf(": %s " , __LINE__);\
// 	printf("( %s )", msg);  } while(0);


// #define SV_DEBUG(msg)




extern int func_add;
extern int func_sub;
extern int func_mul;
extern int func_div;

typedef struct bignum
{
  long long number_of_digits;
    char *digit;
    long *array;
    int array_size;
} bignum;

// A structure to represent a stack
struct Stack
{
    int top;
    unsigned capacity;
    bignum **array;
};

struct bignum *createBignum(char *_digit, long _number_of_digits)
{
    struct bignum *_bignum = (struct bignum *)malloc(sizeof(struct bignum));
    _bignum->digit = _digit;
    _bignum->number_of_digits = _number_of_digits;
    _bignum->array = (long *)malloc(sizeof(long));
    _bignum->array_size = 0;
    return _bignum;
}

// function to create a stack of given capacity. It initializes size of
// stack as 0
struct Stack *createStack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (bignum **)malloc(sizeof(bignum *) * 1024 + 1);
    return stack;
}

// Stack is full when top is equal to the last index
int isFull(struct Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

// Stack is empty when top is equal to -1
int isEmpty(struct Stack *stack)
{
    return stack->top == -1;
}

// Function to add an item to stack.  It increases top by 1
void push(struct Stack *stack, bignum *item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

// Function to remove an item from stack.  It decreases top by 1
bignum *pop(struct Stack *stack)
{

    return stack->array[stack->top--];
}

bignum *peek(struct Stack *stack)
{

    return stack->array[stack->top];
}

void printStack(struct Stack *stack);
void copy_and_push(int *counter, char *acc, struct Stack *stack);
void print_array(bignum *bignum);
bignum* calc(struct Stack *stack, bignum *num1, bignum *num2, char op, int to_push ,int to_break );
void break_into_chuncks(bignum *bignum);
int get_num_of_digits(int num);
void add_zero(bignum *target, int to_be_array_size);
void free_stack(struct Stack *stack);
bignum *interrior_sub(bignum *big, bignum *small);
bignum *interrior_add(bignum *big, bignum *small);
void add_sign(bignum *res);
int fix_negative(bignum *num);
int bigger_digits(bignum *num1, bignum *num2);
int add_func(int num1, int num2, int carry);
int sub_func(int num1, int num2, int borrow);
bignum *interrior_mul(bignum *num1, bignum *num2, bignum *res);
int is_equal_one(bignum *num);
void dev_by_two(bignum *num);
int is_odd(bignum *);
bignum *interrior_div(bignum *num1, bignum *num2, bignum *res, bignum *factor);
bignum *div_help(bignum *num1, bignum *num2, bignum *res, bignum *factor);
void copy_bignum_and_free(bignum* dst ,bignum* src);
void fix_zero(bignum* num);
bignum* check_if_zero(bignum* target);
int check_if_zero_bool(bignum* target);



int main(int argc, char *argv[]){
    char c;
    struct Stack *stack = createStack(1024);
    FILE *input = stdin;
    char *acc = (char *)malloc(sizeof(char));
    acc[0] = 0;
    int counter = 0;
    while ((c = fgetc(input)) != EOF){
        if ( ((48 <= c) && (c <= 57) )|| (c == 95) ){
            counter++;
            acc = (char *)realloc(acc, (counter + 1) * sizeof(char));
            acc[counter - 1] = c;
            acc[counter] = 0;
        }
        if (c <= 32){
            if (0 != counter){
                copy_and_push(&counter, acc, stack);
                //   free (acc);
                //   acc = (char*)malloc(sizeof(char));
                memset(acc, 0, counter + 1);
                counter = 0;
            }
        }
        if ((c == '+') || (c == '-') || (c == '*') || (c == '/') || (c == 'p') || (c == 'q')){
            if ((c != 'p') && (c != 'q')){
                if (0 != counter){
                    copy_and_push(&counter, acc, stack);
                    memset(acc, 0, counter + 1);
                    counter = 0;
                }
                bignum *num1 = pop(stack);
                bignum *num2 = pop(stack);
                printf("num1 is %s\n", num1->digit);
                printf("num2 is %s\n", num2->digit);
                char op = c;
                calc(stack, num1, num2, op,1,1);
                free(num1->array);
                free(num2->array);
                free(num1->digit);
                free(num2->digit);
                free(num1);
                free(num2);
            }
            else if (c == 'p'){
                if (0 != counter){
                    printf("push p: %s", acc);
                    copy_and_push(&counter, acc, stack);
                    //    free (acc);
                    //    acc = (char*)malloc(sizeof(char));
                    memset(acc, 0, counter + 1);
                    counter = 0;
                }
                bignum *new_peek = peek(stack);
                printf("%s\n", new_peek->digit);
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

void printStack(struct Stack *stack){
    for (int i = stack->top; i >= 0; i--){
        printf("%s\n", (stack->array[i]->digit));
    }
}

void copy_and_push(int* counter, char* acc, struct Stack* stack){
                int minus_flag=0;
                int len = strlen(acc);
                int not_zero_index=0;
                if (acc[0]=='_'){
                  minus_flag=1;
                  not_zero_index++;
                  len--;
                }
                while (acc[not_zero_index]=='0'){
                  not_zero_index++;
                  len--;
                }
                  char* digits = (char*)malloc(len*sizeof(char)+1);
                  for(int i = 0; i < (len+1) ; i++){
                      digits[i] = 0;
                  }
                  int j=0;
                  for (int i=not_zero_index; i < strlen(acc); i++){
                    digits[j]= acc[i];
                    j++;
                  }
                  struct bignum* next_num = createBignum(digits,len);
                  if (minus_flag){
                    add_sign(next_num);
                  }
                push(stack, next_num);
                *counter = 0;

}

bignum* calc(struct Stack *stack, bignum *num1, bignum *num2, char op, int to_push ,int to_break ){
    int is_num1_negative = fix_negative(num1);
    int is_num2_negative = fix_negative(num2);
    int big = bigger_digits(num1, num2);
    if(to_break){
        break_into_chuncks(num1);
        break_into_chuncks(num2);
    }
    char* digit;
    char* digit_res;
    char* digit_factor;
    if (big == 1){
        add_zero(num2, (num1->array_size));
    }
    else{
        add_zero(num1, (num2->array_size));
    }
    bignum *res;
    switch (op){
    case '+':
        if ((!is_num1_negative) && (!is_num2_negative)){
            res = interrior_add(num1, num2);
              fix_zero(res);
        }
        else if (is_num1_negative && is_num2_negative){
            res = interrior_add(num1, num2);
              fix_zero(res);
            add_sign(res);
        }
        else{
            printf(" enter one of them is negative");
            if (big == 1){
                res = interrior_sub(num1, num2);
                  fix_zero(res);
            }
            else{
                res = interrior_sub(num2, num1);
                  fix_zero(res);
            }
            if ((is_num1_negative && (big == 1)) || (is_num2_negative && (big == 2))){
                add_sign(res);
            }
        }
        if(to_push){
            res = check_if_zero(res);
            push(stack, res);
        }


        break;

    case '-':
        if ((!is_num1_negative) && (!is_num2_negative)){
            if (big == 1){
                res = interrior_sub(num1, num2);
                fix_zero(res);
                add_sign(res);
            }
            else{
                res = interrior_sub(num2, num1);
                fix_zero(res);

            }
        }
        else if ((is_num1_negative) && (is_num2_negative)){
            if (big == 1){
                res = interrior_sub(num1, num2);
                fix_zero(res);
            }
            else{
                res = interrior_sub(num2, num1);
                fix_zero(res);
                add_sign(res);
            }
        }
        else{ // just one of them is negative  -7 +6
            res = interrior_add(num1, num2);
              fix_zero(res);
            if (is_num2_negative)
            {
                add_sign(res);
            }
        }
          if(to_push){
            res = check_if_zero(res);
            push(stack, res);
        }
        break;
    case '*':
        digit = (char*)malloc(sizeof(char)+1);
        digit[0] = '0';
        digit[1] = 0;
        res = createBignum(digit, 1);
        if(check_if_zero_bool(num1) || check_if_zero_bool(num2)){
            push(stack,res);
            return res;
        }
        break_into_chuncks(res);
        if (((!is_num1_negative) && (!is_num2_negative)) || ((is_num1_negative) && (is_num2_negative))){
            if (big == 1){
                res = interrior_mul(num1, num2, res);
            }
            else{
                res = interrior_mul(num2, num1, res);
            }
        }
        else{ // just one of them is negative  -7 +6
            if (big == 1){
                res = interrior_mul(num1, num2, res);
            }
            else{
                res = interrior_mul(num2, num1, res);
            }
            add_sign(res);
        }
         if(to_push){
            res = check_if_zero(res);
            push(stack, res);
        }
        break;
    case '/':

        digit_res = (char*)malloc(sizeof(char)+1);
        digit_res[0] = '0';
        digit_res[1] = 0;
        res = createBignum(digit_res, 1);
        digit_factor = (char*)malloc(sizeof(char)+1);
        digit_factor[0] = '1';
        digit_factor[1] = 0;
        bignum* factor = createBignum(digit_factor, 1);

        if(check_if_zero_bool(num1) || check_if_zero_bool(num2)){///what to do if dev by zero?
            push(stack,res);
            return res;
        }
        break_into_chuncks(res);
        break_into_chuncks(factor);
        if (((!is_num1_negative) && (!is_num2_negative)) || ((is_num1_negative) && (is_num2_negative))){
            if (big == 1){
                res = interrior_div(num1, num2, res,factor);
            }
            else{
                res = interrior_div(num2, num1, res,factor);
            }
        }
        else{ // just one of them is negative  -7 +6
            if (big == 1){
                res = interrior_div(num1, num2, res,factor);
            }
            else{
                res = interrior_div(num2, num1, res,factor);
            }
            add_sign(res);
        }
         if(to_push){
            res = check_if_zero(res);
            push(stack, res);
        }



        break;
    }
    return res;
}

void break_into_chuncks(bignum *bignum){
    int counter = 7;
    char *acc = (char *)malloc(9 * sizeof(char));
    char *last_acc = (char *)malloc(9 * sizeof(char));
    acc[8] = 0;
    last_acc[8] = 0;
    int num_of_digits = bignum->number_of_digits;
    unsigned int res;
    int place_in_array = 0;
    for (int i = (num_of_digits - 1); i >= 0; i--){
        if ((counter <= 7)){
            acc[counter] = bignum->digit[i];
            counter--;
        }
        if (counter == (-1)){
            counter = 7;
            res = atol(acc);
            bignum->array_size++;
            bignum->array = realloc(bignum->array, (bignum->array_size) * sizeof(long));
            (bignum->array[place_in_array++]) = res;
        }
        if (i == 0){
            for (int i = 0; i < 8; i++){
                last_acc[i] = '0';
            }
            for (int i = (counter + 1); i <= 7; i++){
                last_acc[i] = acc[i];
            }
            res = atol(last_acc);
            bignum->array_size++;
            bignum->array = realloc(bignum->array, (bignum->array_size) * sizeof(long));
            (bignum->array[place_in_array++]) = res;
        }
    }
    free(acc);
    free(last_acc);
}

void print_array(bignum *bignum){
    for (int i = 0; i < (bignum->array_size); i++){
        printf("array in %d , is:  %ld ", i, bignum->array[i]);
    }
}

int get_num_of_digits(int num){
    int counter = 0;
    while (num != 0){
        num = (num / 10);
        counter++;
    }
    return counter;
}

void add_zero(bignum *target, int to_be_array_size){
    target->array = (long *)realloc(target->array, (to_be_array_size + 1) * sizeof(long));
    for (int i = (target->array_size); i < to_be_array_size; i++){
        target->array[i] = 0;
        (target->array_size)++;
    }
}

void free_stack(struct Stack *stack){
    while (!isEmpty(stack)){
        bignum *num = pop(stack);
        free(num->array);
        free(num->digit);
        free(num);
    }
    free(stack->array);
}

bignum* interrior_sub(bignum* big, bignum* small){ // gets 2 bignums after rappeding by zero and sign deletion

  int ans_num_of_digits = 0;
  int total_num_of_digits = 0;
  char *digit = (char *)malloc((sizeof(char)));
  char *digit_tmp = (char *)malloc(9*(sizeof(char)));
  digit[0] = 0;
  for (int i = 0; i < (9*(sizeof(char))); i++){digit_tmp[i] = 0;}
  int bigger_num_array_size = big->array_size;
  int borrow = 0;
  for (int i = 0; i < bigger_num_array_size; i++){
      printf("big in array is:    %d\n",big->array[i] );
      int ans = sub_func(big->array[i], small->array[i], borrow);
      if (ans <0){
            printf("not suppose to enter");
            ans = add_func(ans, pow(10, 8), 0);
            borrow = 1;
        }
      else{borrow =0;}
      if (i+1 != bigger_num_array_size){
            total_num_of_digits = total_num_of_digits + 8;
            char *str = (char *)malloc( (total_num_of_digits + 1) * (sizeof(char)));
            for (int i = 0; i < (total_num_of_digits+1); i++){str[i] = 0;}
            ans_num_of_digits =get_num_of_digits(ans);
            for (int i=0; i<(8-ans_num_of_digits);i++){str[i]='0';}
            printf("str after concat a %s\n", str );
            printf("digit_tmp before concat a %s\n", digit_tmp );
            strcat(str,digit_tmp);
            printf("str after concat a %s\n", str );
            digit_tmp = (char *)realloc(digit_tmp,( (total_num_of_digits + 1) * (sizeof(char))));
            strcpy(digit_tmp, str);
            free(str);
            printf("this is digit tmp         %s\n", digit_tmp );
        }
        else {total_num_of_digits = total_num_of_digits + get_num_of_digits(ans);}

          if(ans!= 0){
          char temp_str[22];
          sprintf(temp_str, "%d", ans);
          strcat(digit_tmp, temp_str);
          }

          digit_tmp = (char *)realloc(digit_tmp,( (total_num_of_digits + 1) * (sizeof(char))));
          digit_tmp[total_num_of_digits]=0;
          strcat(digit_tmp, digit);
          digit = (char *)realloc(digit, (total_num_of_digits + 1) * (sizeof(char)));
          digit[total_num_of_digits]=0;
          strcpy(digit, digit_tmp);
          strcpy(digit_tmp, "");
    }


    bignum *res = createBignum(digit, total_num_of_digits);
    free(digit_tmp);
    return res;
  }

bignum *interrior_add(bignum *big, bignum *small){ // gets 2 bignums after rappeding by zero and sign deletion
  int ans_num_of_digits = 0;
  int total_num_of_digits = 0;
  char *digit = (char *)malloc((sizeof(char)));
  char *digit_tmp = (char *)malloc(9*(sizeof(char)));
  digit[0] = 0;
  for (int i = 0; i < (9*(sizeof(char))); i++){digit_tmp[i] = 0;}
  int bigger_num_array_size = big->array_size;
  int carry = 0;
  for (int i = 0; i < bigger_num_array_size; i++){
      int ans = add_func(big->array[i], small->array[i], carry);
      if (ans >= pow(10, 8)){
            ans = sub_func(ans, pow(10, 8), 0);
            carry = 1;
        }
      else{carry =0;}
      if (i+1 != bigger_num_array_size){
            total_num_of_digits = total_num_of_digits + 8;
            char *str = (char *)malloc( (total_num_of_digits + 1) * (sizeof(char)));
            for (int i = 0; i < (total_num_of_digits+1); i++){str[i] = 0;}
            ans_num_of_digits =get_num_of_digits(ans);
            for (int i=0; i<(8-ans_num_of_digits);i++){str[i]='0';}
            strcat(str,digit_tmp);
            printf("str after concat a %s\n", str );
            digit_tmp = (char *)realloc(digit_tmp,( (total_num_of_digits + 1) * (sizeof(char))));
            strcpy(digit_tmp, str);
            free(str);
            printf("this is digit tmp         %s\n", digit_tmp );
        }
        else {total_num_of_digits = total_num_of_digits + get_num_of_digits(ans);}

          if(ans!= 0){
          char temp_str[22];
          sprintf(temp_str, "%d", ans);
          strcat(digit_tmp, temp_str);
          }

          digit_tmp = (char *)realloc(digit_tmp,( (total_num_of_digits + 1) * (sizeof(char))));
          digit_tmp[total_num_of_digits]=0;
          strcat(digit_tmp, digit);
          digit = (char *)realloc(digit, (total_num_of_digits + 1) * (sizeof(char)));
          digit[total_num_of_digits]=0;
          strcpy(digit, digit_tmp);
          strcpy(digit_tmp, "");
    }


    bignum *res = createBignum(digit, total_num_of_digits);
    free(digit_tmp);
    return res;
  }




void add_sign(bignum *res){
  long long len = res->number_of_digits;
    char *digit_tmp = (char *)malloc((2+len)*sizeof(char));
    for(int i=0; i< (len+2);i++){digit_tmp[i]=0;}
    digit_tmp[0]='_';
    for(int i=0; i<len;i++){
      digit_tmp[i+1]=res->digit[i];
    }
    digit_tmp[len+1]=0;
    res->digit = (char *)realloc(res->digit,(len+2) *sizeof(char));
    res->digit[len+1] = 0;
    strcpy(res->digit, digit_tmp);
    free(digit_tmp);
    res->number_of_digits++;
}

int fix_negative (bignum* num){
  int res = 0;
  if ('_' ==(num->digit[0])){
    res=1;
    int digit_len= (num->number_of_digits)-1;
    char* digit_tmp = (char*)malloc(digit_len+1);
    for(int i = 0;i < digit_len+1; i++){
        digit_tmp[i] = 0;
    }
    for(int i=0;i<digit_len;i++){
      digit_tmp[i]=num->digit[i+1];
    }
    num->digit = (char*)realloc(num->digit , (1+(sizeof(num->digit))) );
    num->digit[sizeof(num->digit)] = 0;
    strcpy(num->digit,digit_tmp);
    free(digit_tmp);
    num->number_of_digits--;
  }
  return res;
}

int bigger_digits (bignum* num1, bignum* num2){
    printf("enter bigger digit\n");
  int res =1;
  int local_num1_num_of_digits = num1->number_of_digits;
  int local_num2_num_of_digits= num2 ->number_of_digits;
  int num1_index=0;
  while (((num1->digit[num1_index]) == '0')||((num1->digit[num1_index]) == '_') ){
    num1_index++;
    local_num1_num_of_digits--;
  }
  int num2_index=0;
  while (((num2->digit[num2_index])=='0')|| ((num2->digit[num2_index])=='_')){
    num2_index++;
    local_num2_num_of_digits--;
  }
  if (local_num2_num_of_digits > local_num1_num_of_digits){
    res =2;
  }
  else if(local_num2_num_of_digits == local_num1_num_of_digits){
    int i=0;
    res=0;
    while ((i<local_num1_num_of_digits)&& (!res)){
      if ((num2->digit[num2_index])> (num1->digit[num1_index])){res=2;}
      else if ((num2->digit[num2_index]) < (num1->digit[num1_index])){res=1;}
      num2_index++;
      num1_index++;
      i++;
    }
  }
  printf("line: %d\n",__LINE__);
  printf("bigger is num%d :\n",res);
  printf("line: %d\n",__LINE__);
  return res;

}

bignum *interrior_mul(bignum *num1, bignum *num2, bignum *res){ //num1 > num2 && both positive
    printf("enter mul res char is: %s\n", res->digit);
    printf("enter mul num1 char is: %s\n", num1->digit);
    printf("enter mul num2 char is: %s\n", num2->digit);
    fix_zero(num1);
    if(res->number_of_digits != 1){fix_zero(res);}
    if (is_equal_one(num2)){
        bignum* res_new = calc(0,res,num1,'+',0,0);
        copy_bignum_and_free(res,res_new);
        return res;
    }
    if (is_odd(num2)){
        printf("num2 is odd %s\n",num2->digit);
        bignum* res_new = calc(0,res,num1,'+',0,0);
        copy_bignum_and_free(res,res_new);

    }
    bignum* num1_new = calc(0,num1,num1,'+',0,0);
    copy_bignum_and_free(num1,num1_new);
    dev_by_two(num2);
    res = interrior_mul(num1, num2, res);
    return res;
}




int is_equal_one(bignum *num){
    int num_of_digits = num->number_of_digits;
    char first_char = num->digit[0];
    if ((num_of_digits == 1) && (first_char == '1')){
        return 1;
    }
    else
    {
        return 0;
    }
}



void dev_by_two(bignum *num){
    printf("num before dv by two %s\n",num->digit);
    int carry = 0;
    for (int i = 0; i < num->number_of_digits; i++){
        int digit = (num->digit[i] - '0');
        int new_digit = digit / 2;
        new_digit = new_digit + 5 * carry;
        char new_char = 48 + new_digit;
        num->digit[i] = new_char;
        carry = digit % 2;
    }
    fix_zero(num);
    printf("num after dv by two %s\n",num->digit);
}

int is_odd(bignum *num){
    char c = num->digit[num->number_of_digits-1];
    int x = c - '0';
    int mod = x % 2;
    if (mod == 0){
        return 0;
    }
    else
    {
        return 1;
    }
}

bignum *interrior_div(bignum *num1, bignum *num2, bignum *res, bignum *factor){
    printf("enter div res char is: %s\n", res->digit);
    printf("enter div factor char is: %s\n", factor->digit);
    printf("enter div num1 char is: %s\n", num1->digit);
    printf("enter div num2 char is: %s\n\n", num2->digit);
    div_help(num1, num2, res, factor);
    printf("finish div_help\n\n");
    if (bigger_digits(num1, num2) == 1 || bigger_digits(num1, num2) == 0){
        bignum* num1_new = calc(0,num1,num2,'-',0,0);
        copy_bignum_and_free(num1,num1_new);

        bignum* res_new = calc(0,res,factor,'+',0,0);
        copy_bignum_and_free(res,res_new);
    }
    return res;
}

bignum *div_help(bignum *num1, bignum *num2, bignum *res, bignum *factor){
    printf("line: %d\n",__LINE__);
printf("enter div_help res char is: %s\n", res->digit);
    printf("enter div_help factor char is: %s\n", factor->digit);
    printf("enter div_help num1 char is: %s\n", num1->digit);
    printf("enter div_help num2 char is: %s\n\n", num2->digit);
   printf("line: %d\n",__LINE__);
if(bigger_digits(num1, num2) == 2){
    printf("line: %d\n",__LINE__);
    dev_by_two(num2);
    printf("line: %d\n",__LINE__);
    dev_by_two(factor);
    printf("line: %d\n",__LINE__);
    return res;
}
bignum* factor_new = calc(0,factor,factor,'+',0,0);
copy_bignum_and_free(factor,factor_new);
printf("line: %d\n",__LINE__);
bignum* num2_new = calc(0,num2,num2,'+',0,0);
copy_bignum_and_free(num2,num2_new);
div_help(num1, num2, res, factor);
printf("line: %d\n",__LINE__);
if (bigger_digits(num1, num2) == 1 || bigger_digits(num1, num2) == 0){
printf("line: %d\n",__LINE__);
    bignum* num2_new = calc(0,num1,num2,'-',0,0);
    copy_bignum_and_free(num2,num2_new);
printf("line: %d\n",__LINE__);
    bignum* res_new = calc(0,res,factor,'+',0,0);
    copy_bignum_and_free(res,res_new);
}
printf("line: %d\n",__LINE__);
dev_by_two(num2);
printf("line: %d\n",__LINE__);
dev_by_two(factor);
printf("line: %d\n",__LINE__);
return res;

}


void copy_bignum_and_free(bignum* dst ,bignum* src){
    free(dst->digit);
    dst->digit = src->digit;
    dst->number_of_digits = src->number_of_digits;
    free(dst->array);
    dst->array = (long *)malloc(sizeof(long));
    dst->array_size = 0;
    break_into_chuncks(dst);
    free(src->array);
    free(src);


}


void fix_zero(bignum* num){
                char* acc = num->digit;
                int len = strlen(acc);
                int not_zero_index=0;

                while (acc[not_zero_index]=='0'){
                  not_zero_index++;
                  len--;
                }
                  char* digits = (char*)malloc(len*sizeof(char)+1);
                  for(int i = 0; i < (len+1) ; i++){
                      digits[i] = 0;
                  }
                  int j=0;
                  for (int i=not_zero_index; i < strlen(acc); i++){
                    digits[j]= acc[i];
                    j++;
                  }
                  num->digit = digits;
                  num->number_of_digits = len;
                  free(acc);


}

bignum* check_if_zero(bignum* target) {
  int is_zero=1;
  int index=0;
  int digit_len = target->number_of_digits;
  if (target->digit[index]=='_'){
    index++;
   }
  while ((index< digit_len)&& (is_zero) )
    {
    if ((target->digit[index])!= '0'){is_zero=0;}
    index++;
    }

    if (is_zero){
      char* digit = (char*) malloc(2*sizeof(char));
      digit[0]='0';
      digit[1]=0;
        bignum *temp = createBignum(digit, 1);
        free(target->digit);
        free(target->array);
        return temp;
    }
    return target;
}


int check_if_zero_bool(bignum* target){
  int is_zero=1;
  int index=0;
  int digit_len = target->number_of_digits;
  if (target->digit[index]=='_'){
    index++;
   }
  while ((index< digit_len)&& (is_zero) )
    {
    if ((target->digit[index])!= '0'){is_zero=0;}
    index++;
    }

    return is_zero;
}
