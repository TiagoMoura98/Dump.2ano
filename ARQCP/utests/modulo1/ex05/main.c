#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(int vec[], int exp[], int num  )
{
    int in_vec[100];
    int out_vec[100];

    // setup 
        memset(in_vec, 0xaa, sizeof in_vec);
        memset(out_vec, 0xaa, sizeof out_vec);
    	
   memcpy(in_vec,vec,num*sizeof(int));  // input vec  
	copy_vec(in_vec,num,out_vec);
    if (num>0)                                    // do this only if the vector has elements 
    {
	TEST_ASSERT_EQUAL_INT_ARRAY(exp,out_vec,num);    // output == expected 
	TEST_ASSERT_EQUAL_INT_ARRAY(vec,in_vec,num);     // input == original  
    }
    TEST_ASSERT_EQUAL_INT8(0xaa, out_vec[num]& 0xff);   /// check sentinel 
    TEST_ASSERT_EQUAL_INT8(0xaa, in_vec[num]& 0xff);    // check sentinel 

    
}


void test_NullVector()
{ 
    run_test( (int[]){0}, (int[]){0},0); 

}


void test_Vector_One()
{ 
    run_test( (int[]){1}, (int[]){1}, 1); 

}


void test_Vector_Three()
{ 
    run_test( (int[]){-1,0,2}, (int[]){-1,0,2}, 3); 

}



void test_Vector_Four()
{ 
    run_test( (int[]){-1,-1,-1,2}, (int[]){-1,-1,-1,2}, 4); 

}


void test_Vector_Nine()
{ 
    run_test( (int[]){0,1,2,3,4,5,6,7,-2}, (int[]){0,1,2,3,4,5,6,7,-2}, 9); 

}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVector);
    RUN_TEST(test_Vector_One);
    RUN_TEST(test_Vector_Three);
    RUN_TEST(test_Vector_Four);
    RUN_TEST(test_Vector_Nine);
    return UNITY_END();  

  } 






