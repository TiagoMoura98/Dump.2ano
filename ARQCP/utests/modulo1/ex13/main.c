#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(short vec_in[], int n , short  vec_expected[] , int res  )
{
    short vec[100];
    short out_vec[100];

    int r;  

    // setup 
      
	memset(vec, 0x55, sizeof vec);
	memset(out_vec, 0x55, sizeof out_vec);
	memcpy(vec+1,vec_in,n*sizeof(short)); 
	r=sort_without_reps(vec+1,n,out_vec+1); 
    
    TEST_ASSERT_EQUAL_INT16_MESSAGE(0x5555, vec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_INT16_MESSAGE(0x5555, vec[n+1],"Sentinel 2");    // check sentinel 
    TEST_ASSERT_EQUAL_INT16_MESSAGE(0x5555, out_vec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_INT16_MESSAGE(0x5555, out_vec[n+1],"Sentinel 2");    // check sentinel 
    if (n>0)
    TEST_ASSERT_EQUAL_INT16_ARRAY(vec_in, vec+1,n);    // check if original vector was not changed  
  
    TEST_ASSERT_EQUAL_INT_MESSAGE(res,r,"Number of elements");  
    if (r>0)
    TEST_ASSERT_EQUAL_INT16_ARRAY(vec_expected, out_vec+1,r);    // check result 
    
}


void test_NullVec()
{ 
    run_test((short[]){0},0,(short[]){0},0); 
}
void test_One()
{ 
    run_test((short[]){1},1,(short[]){1},1); 
}
void test_Three()
{ 
    run_test((short[]){2,0,1},3,(short[]){0,1,2},3); 
}
void test_Minus3()
{ 
    run_test((short[]){-1,-2,-3},3,(short[]){-3,-2,-1},3); 
}
void test_Ten()
{ 
    run_test((short[]){2,1,1,1,1,1,1,1,1},9,(short[]){1,2},2); 
}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVec);
    RUN_TEST(test_One);
    RUN_TEST(test_Three);
    RUN_TEST(test_Minus3);
    RUN_TEST(test_Ten);
    return UNITY_END();  

  } 






