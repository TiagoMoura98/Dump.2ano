#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(int vec[],  int num , int min_exp, int max_exp , float avg_exp  )
{
    int in_vec[100];

    int s1=0x55555555;  // sentinel 1  
    int min,max;
	float avg; 
    int s2=0x55555555;   // sentinel 2 


    // setup 
        memset(in_vec, 0xaa, sizeof in_vec);
     
    	
	memcpy(in_vec+1,vec,num*sizeof(int));  // input vec  
	get_array_statistics( in_vec+1, num, &min, &max, &avg );
    if (num>0)                                    // do this only if the vector has elements 
    {
	TEST_ASSERT_EQUAL_INT_ARRAY(vec,in_vec+1,num);     // input == original  
    }
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[num+1]& 0xff);    // check sentinel 
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[0]& 0xff);    // check sentinel 
    
    TEST_ASSERT_EQUAL_INT(min_exp,min); 
    TEST_ASSERT_EQUAL_INT(max_exp,max); 
	TEST_ASSERT_FLOAT_WITHIN(0.001,avg_exp,avg); 

    TEST_ASSERT_EQUAL_INT(s1,0x55555555);		
    TEST_ASSERT_EQUAL_INT(s2,0x55555555);		
    
}


void test_NullVector()
{ 
    run_test( (int[]){0},0,0,0,0.0); 

}


void test_Vector_One()
{ 
    run_test( (int[]){1}, 1,1,1,1.0 ); 

}


void test_Vector_Two()
{ 
    run_test( (int[]){2,2,2}, 3, 2, 2, 2.0); 

}



void test_Vector_Minus_Three()
{ 
    run_test( (int[]){-1,-1,-1}, 3, -1, -1, -1.0); 

}


void test_Vector_Nine()
{ 
    run_test( (int[]){1,1,1,1,1,1,1,1,-8},9,-8,1,0.0); 

}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVector);
    RUN_TEST(test_Vector_One);
    RUN_TEST(test_Vector_Two);
    RUN_TEST(test_Vector_Minus_Three);
    RUN_TEST(test_Vector_Nine);
    return UNITY_END();  

  } 






