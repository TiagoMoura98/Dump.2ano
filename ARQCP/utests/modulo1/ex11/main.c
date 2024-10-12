#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(int vec_in[], int n , int expected  )
{
    int vec[100];

    int r;     
    // setup 
      
	memset(vec, 0x55, 100*sizeof(int));
	if (n>0) 
		memcpy(vec+1,vec_in,n*sizeof(int));  
        r=sum_odd(vec+1); 
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, vec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, vec[n+1],"Sentinel 2");    // check sentinel 
    if (n>0)
    TEST_ASSERT_EQUAL_INT_ARRAY(vec_in, vec+1,n);    // check if input has not changed 
  
    TEST_ASSERT_EQUAL_INT_MESSAGE(expected,r,"Sum of odd integers");  
        
}


void test_NullVec()
{ 
    run_test((int[]){0},1,0); 
}
void test_One()
{ 
    run_test((int[]){1,1},2,1); 
}
void test_Two()
{ 
    run_test((int[]){3,0,1,2},4,1); 
}
void test_Minus3()
{ 
    run_test((int[]){3,-1,-2,-3},4,-4); 
}
void test_Ten()
{ 
    run_test((int[]){9,2,1,1,1,1,1,1,1,1},10,8); 
}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVec);
    RUN_TEST(test_One);
    RUN_TEST(test_Two);
    RUN_TEST(test_Minus3);
    RUN_TEST(test_Ten);
    return UNITY_END();  

  } 






