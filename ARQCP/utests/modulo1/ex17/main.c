#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(float notas_in[], int n_notas, int  exp_vec[21])
{
    float  vec[100]={0};
    int out_res[23]={0x55555555};     
  
    out_res[22]=0x55555555;  

    int i; 
    // setup 
    for (i=0;i<n_notas;i++)
        vec[i+1]=notas_in[i]; 


    frequencies(vec+1,n_notas,out_res+1); 

    	
        
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0, vec[0],"Sentinel 1");    // check sentinel 
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(0, vec[n_notas+1],"Sentinel 2");    // check sentinel
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, out_res[0],"Sentinel 1 ints");    // check sentinel 
    TEST_ASSERT_EQUAL_INT_MESSAGE(0x55555555, out_res[22],"Sentinel 2 ints");    // check sentinel

    TEST_ASSERT_EQUAL_INT_ARRAY(exp_vec,out_res+1,21);

    
}


void test_Null()
{ 
    run_test( (float []){0}, 0,
                  //                      1   1 1 1 1 1  1 1 1 1 2 
		  // 0 1 2 3 4 5  6 7 8 9 0   1 2 3 4 5  6 7 8 9 0
            (int[]) {0,0,0,0,0,0, 0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0}); 
}

void test_One()
{ 
    run_test( (float []){1.5}, 1,
                  //                      1   1 1 1 1 1  1 1 1 1 2 
		  // 0 1 2 3 4 5  6 7 8 9 0   1 2 3 4 5  6 7 8 9 0
       	   (int[])  {0,1,0,0,0,0, 0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0}); 
}


void test_Case1()
{ 
    run_test( (float []){2,0,1}, 3,
                  //                      1   1 1 1 1 1  1 1 1 1 2 
		  // 0 1 2 3 4 5  6 7 8 9 0   1 2 3 4 5  6 7 8 9 0
       	   (int[])  {1,1,1,0,0,0, 0,0,0,0,0,  0,0,0,0,0, 0,0,0,0,0}); 
}

void test_Case2()
{ 
    run_test( (float []){20,10,10,10,10,10,10,10,10,10}, 10,
                  //                      1   1 1 1 1 1  1 1 1 1 2 
		  // 0 1 2 3 4 5  6 7 8 9 0   1 2 3 4 5  6 7 8 9 0
       	   (int[])  {0,0,0,0,0,0, 0,0,0,0,9,  0,0,0,0,0, 0,0,0,0,1}); 
}

int main()
  { 
    UNITY_BEGIN();
    RUN_TEST(test_Null);
    RUN_TEST(test_One);
    RUN_TEST(test_Case1);
    RUN_TEST(test_Case2);
    return UNITY_END();  
  } 






