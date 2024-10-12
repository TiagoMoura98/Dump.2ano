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
	format_word(vec+1);
    
    TEST_ASSERT_EQUAL_CHAR(0xaa, vec[strlen(str)+2]);    // check sentinel 
    TEST_ASSERT_EQUAL_CHAR(0xaa, vec[0]);    // check sentinel 
    
    TEST_ASSERT_EQUAL_STRING(expected,vec+1); 
    
}


void test_Hex()
{ 
    run_test("hex","Hex"); 
}
void test_AbcD()
{ 
    run_test(" A    b   c   D   ","A B C D"); 
}
void test_Abba()
{ 
    run_test("  Abba   ","Abba"); 
}

void test_Spaces()
{ 
    run_test("     ",""); 
}
void test_NullString()
{ 
    run_test("",""); 
}
void test_FullString()
{ 
    run_test(" tHe numBeR    must bE saVed    ","The Number Must Be Saved"); 
}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_Hex);
    RUN_TEST(test_AbcD);
    RUN_TEST(test_Abba);
    RUN_TEST(test_Spaces);
    RUN_TEST(test_NullString);
    RUN_TEST(test_FullString);
    return UNITY_END();  

  } 






