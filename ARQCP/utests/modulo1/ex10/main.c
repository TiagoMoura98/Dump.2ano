#include <string.h>  
#include "../../unity.h"
#include "asm.h" 



void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}



void run_test(char * str, char c, int offset )
{
    char vec[100];

    char * res;   

	char * expected; 

	if (offset==-1) 
		  expected =NULL; 
		  else 
		  expected=vec+offset;  // offset starts at 1 
		  

    // setup 
        memset(vec, 0xaa, sizeof vec);
     
    	
	strcpy(vec+1,str);  // bad practice  
	res=where_is(vec+1,c);
    
    TEST_ASSERT_EQUAL_CHAR(0xaa, vec[strlen(str)+2]);    // check sentinel 
    TEST_ASSERT_EQUAL_CHAR(0xaa, vec[0]);    // check sentinel 
    
    TEST_ASSERT_EQUAL_PTR(expected,res);  
    
}


void test_Null()
{ 
    run_test("",'c',-1); 
}
void test_AbcD()
{ 
    run_test("AbcD",'D',4); 
}
void test_Abba()
{ 
    run_test("Abba",'b',2); 
}

void test_Spaces()
{ 
    run_test("abzfg   ",' ',6); 
}
void test_Not_Found()
{ 
    run_test("abcdefghijklmn",'z',-1); 
}



int main()
  { 

    UNITY_BEGIN();
    RUN_TEST(test_Null);
    RUN_TEST(test_AbcD);
    RUN_TEST(test_Abba);
    RUN_TEST(test_Spaces);
    RUN_TEST(test_Not_Found);
    return UNITY_END();  

  } 






