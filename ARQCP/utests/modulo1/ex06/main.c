#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(int vec[],  int num , int expected )
{
    int in_vec[100];

    int result; 

    // setup 
        memset(in_vec, 0xaa, sizeof in_vec);
     
    	
	memcpy(in_vec+1,vec,num*sizeof(int));  // input vec  
	result=sum_even(in_vec+1,num);
    if (num>0)                                    // do this only if the vector has elements 
    {
	TEST_ASSERT_EQUAL_INT_ARRAY(vec,in_vec+1,num);     // input == original  
    }
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[num+1]& 0xff);    // check sentinel 
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[0]& 0xff);    // check sentinel 
    
    TEST_ASSERT_EQUAL_INT(expected,result); 
    
}


void test_NullVector()
{ 
    run_test( (int[]){0},0,0); 

}


void test_Vector_One()
{ 
    run_test( (int[]){1}, 1, 0); 

}


void test_Vector_One_Zero_MinusOne()
{ 
    run_test( (int[]){1,0,-1}, 3, 0); 

}



void test_Vector_Minus_Three()
{ 
    run_test( (int[]){-1,-1,-1}, 3, 0); 

}


void test_Vector_Nine()
{ 
    run_test( (int[]){1,1,1,1,18,1,1,1,2},9,20); 

}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVector);
    RUN_TEST(test_Vector_One);
    RUN_TEST(test_Vector_One_Zero_MinusOne);
    RUN_TEST(test_Vector_Minus_Three);
    RUN_TEST(test_Vector_Nine);
    return UNITY_END();  

  } 






