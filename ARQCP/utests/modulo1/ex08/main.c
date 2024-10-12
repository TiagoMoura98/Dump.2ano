#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(unsigned int num, int expected )
{
    unsigned int in_vec[3];

    int result; 

    // setup  

    in_vec[1]=num;  
    in_vec[0]=0xaaaaaaaa; 	
    in_vec[2]=0xaaaaaaaa; 	
	result=sum_integer_bytes(in_vec+1);
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[2]& 0xff);    // check sentinel 
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[0]& 0xff);    // check sentinel 
    
    TEST_ASSERT_EQUAL_INT(expected,result); 
    
}


void test_Zero()
{ 
    run_test( 0,0); 

}


void test_One()
{ 
    run_test( 1, 1); 

}


void test_Four()
{ 
    run_test( 0x01010101, 4); 

}



void test_Minus()
{ 
    run_test( 0xffffffff, 1020); 

}


void test_Last()
{ 
    run_test( 0xff000001,256); 

}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_Zero);
    RUN_TEST(test_One);
    RUN_TEST(test_Four);
    RUN_TEST(test_Minus);
    RUN_TEST(test_Last);
    return UNITY_END();  

  } 






