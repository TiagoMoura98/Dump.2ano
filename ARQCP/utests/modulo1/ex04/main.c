#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(char * str, char * expected )
{
    char vec[100];

    

    // setup 
        memset(vec, 0xaa, sizeof vec);
     
    	
	strcpy(vec+1,str);  // bad practice  
	capitalize(vec+1);
    
    TEST_ASSERT_EQUAL_CHAR(0xaa, vec[strlen(str)+2]);    // check sentinel 
    TEST_ASSERT_EQUAL_CHAR(0xaa, vec[0]);    // check sentinel 
    
    TEST_ASSERT_EQUAL_STRING(expected,vec+1); 
    
}


void test_Hex()
{ 
    run_test("hex","HEX"); 
}
void test_AbcD()
{ 
    run_test("AbcD","ABCD"); 
}
void test_Abba()
{ 
    run_test("Abba","ABBA"); 
}

void test_Spaces()
{ 
    run_test("     ","     "); 
}
void test_NullString()
{ 
    run_test("",""); 
}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_Hex);
    RUN_TEST(test_AbcD);
    RUN_TEST(test_Abba);
    RUN_TEST(test_Spaces);
    RUN_TEST(test_NullString);
    return UNITY_END();  

  } 






