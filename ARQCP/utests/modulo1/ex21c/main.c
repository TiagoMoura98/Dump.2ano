#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(int num  )
{
    int vec[100];
    int *ptrvec[100]; 


    // setup 
      
	memset(vec, 0x55, sizeof vec);
	memset(ptrvec, 0x55, sizeof vec);
    	
        init_data_ptrs(vec+1,num,ptrvec+1);  

    
    TEST_ASSERT_EQUAL_PTR_MESSAGE(0x5555555555555555, ptrvec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_PTR_MESSAGE(0x5555555555555555, ptrvec[num+1],"Sentinel 2");    // check sentinel 
    if (num>0)
    { 
       for (int i=0; i<num;i++) 
	      TEST_ASSERT_EQUAL_PTR(&vec[i+1],ptrvec[i+1]);  
    } 
    TEST_ASSERT_EACH_EQUAL_INT(0x55555555,vec,100);  // no changes on the int vector!!!! 

 
}


void test_NullVec()
{ 
    run_test(0); 
}
void test_One()
{ 
    run_test(1); 
}
void test_Three()
{ 
    run_test(3); 
}
void test_Four()
{ 
    run_test(33); 
}




int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_NullVec);
    RUN_TEST(test_One);
    RUN_TEST(test_Three);
    RUN_TEST(test_Four);
    return UNITY_END();  

  } 






