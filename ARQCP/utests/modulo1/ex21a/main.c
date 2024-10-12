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


    // setup 
      
	memset(vec, 0x55, sizeof vec);
        init_data(vec+1,num);  

    
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, vec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, vec[num+1],"Sentinel 2");    // check sentinel 

    if (num>0)
    { 
       for (int i=0; i<num;i++) 
	      TEST_ASSERT_FALSE(vec[i+1]==0x55555555);  
    } 
 
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






