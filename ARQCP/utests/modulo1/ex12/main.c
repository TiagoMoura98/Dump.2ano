#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(int vec_in[], int n , int  vec_expected[]  )
{
    int vec[100];

    

    // setup 
      
      memset(vec, 0xaa, sizeof vec);
        vec[0]=0x55555555;
        vec[n+1]=0x55555555;
	memcpy(vec+1,vec_in,n*sizeof(int)); 
	array_sort(vec+1,n); 
    
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, vec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, vec[n+1],"Sentinel 2");    // check sentinel 
    if (n>0)
    TEST_ASSERT_EQUAL_INT_ARRAY(vec_expected, vec+1,n);    // check result 
   
    
}


void test_NullVec()
{ 
    run_test((int[]){0},0,(int[]){0}); 
}
void test_One()
{ 
    run_test((int[]){1},1,(int[]){1}); 
}
void test_Zero()
{ 
    run_test((int[]){0,1,2},3,(int[]){0,1,2}); 
}
void test_Minus3()
{ 
    run_test((int[]){-1,-2,-3},3,(int[]){-3,-2,-1}); 
}
void test_Ten()
{ 
    run_test((int[]){2,1,1,1,1,1,1,1,1},9,(int[]){1,1,1,1,1,1,1,1,2}); 
}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVec);
    RUN_TEST(test_One);
    RUN_TEST(test_Zero);
    RUN_TEST(test_Minus3);
    RUN_TEST(test_Ten);
    return UNITY_END();  

  } 






