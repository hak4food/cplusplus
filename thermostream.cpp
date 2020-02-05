//for testmethod framework interfaces
#include "testmethod.hpp"
//for testmethod API interfaces
#include "mapi.hpp"
//#include "tml_def.h"

#include <iostream>
#include <iomanip>
#include <string>
#include "stdlib.h"
#include <vector>
	
int thermal_control_datalog_temp(){
       
	   string line;
	   FILE *mypipe;
	   char buffer[1024];
	   char tmp_str[128];
	   STRING command;
	   vector<string>  vector_of_lines;
	   vector_of_lines.clear(); 
	   
	   sprintf(tmp_str,"echo TEMP? | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9");
	   system(tmp_str);
	    
	   sprintf(tmp_str,"echo TEMP? | /opt/sicl/bin/iread lan[192.168.0.103]:gpib0,9");
	   command.assign(tmp_str);
	   
	   string pipe_command = command + " 2<&1";
	 
	   mypipe = popen(command.c_str(),"r");
	 
	   if ( mypipe == NULL ) {
	      return -1;
	   }
	 
	   while (!feof(mypipe)) {
	 
	     fgets(buffer,1024,mypipe);
	     vector_of_lines.push_back(buffer);
	 
	   } 
	   int error_code = pclose(mypipe);
	   
	   double temp;
	   temp = atof(vector_of_lines[0].c_str());
	   
	   char strt[128];
	   sprintf(strt, "COND: ACTUAL_TEMP=%3.1fC", temp);
	   PUT_DATALOG(strt);
	   
	   return error_code;   
}

void thermal_control_write(double set_temp, int soak_time)
{
    char    tmp_str[128];
    STRING  tmp_string;
    int setn_point=-1;
   
    if(set_temp >= -50 && set_temp < 15 ) {
        setn_point = 2;
    } else if(set_temp >= 15 && set_temp < 35 ){
        setn_point = 1;
    } else if(set_temp >= 35 && set_temp < 100 ){
        setn_point = 0; 
    } else {
    	cout << "Invalid test temp: " << set_temp << endl;
    }
   
    sprintf(tmp_str,"echo SETN %d | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9",setn_point);
    system(tmp_str);
   
    sprintf(tmp_str,"echo SETP %f | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9",set_temp);
    system(tmp_str);
   
    sprintf(tmp_str,"echo SOAK %d | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9",soak_time);
    system(tmp_str);
    
    sprintf(tmp_str,"echo FLOW 1 | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9");
    system(tmp_str);
    
    sprintf(tmp_str,"echo STND 1 | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9");
    system(tmp_str);
   
    char cond[128];
    sprintf(cond, "COND: TEMP=%3.1fC", set_temp);
    PUT_DATALOG(cond);
    
    cout << "Soaking..." << endl;
    WAIT_TIME(soak_time);
    
	thermal_control_datalog_temp(); 
}

void thermal_control_head(int head_value){
       
        //if 1 then the head is down;
        //if 0 then the head is up;
    char    tmp_str[128];
    STRING  tmp_string;
   
    sprintf(tmp_str,"echo STND %d | /opt/sicl/bin/iwrite lan[192.168.0.103]:gpib0,9",head_value);
    system(tmp_str);
   
}

/**
 *Testmethod class.
 *
 *For each testsuite using this testmethod, one object of this
 *class is created.
 */
class jtag_set_Thermostream: public testmethod::TestMethod
{
protected:
protected:
/**
 *Initialize the parameter interface to the testflow.
 *This method is called just once after a testsuite is created.
 *
 *Note: TestMethod API should not be used in this method.
 */
virtual void initialize()
{
}

/**
 *This test is invoked per site.
 */
virtual void run()
{
	int temp;
	 
    ON_FIRST_INVOCATION_BEGIN();

    	GET_USER_FLAG("test_temp",&temp);
    	
   		if(temp == 0) 
   	        thermal_control_write(-35,240);
   		else if(temp == 1)
	        thermal_control_write(90,240);
   		else if(temp == 2)
   	        thermal_control_write(25,240);
   		else if(temp == -1)
   			thermal_control_head(0); //raise head
   		else
   			cout << "Unrecognized temperature corner: " << temp << endl;

    ON_FIRST_INVOCATION_END();

    return;
}

/**
 *This function will be invoked once the specified parameter's value is changed.
 *@param parameterIdentifier
 *
 *Note: TestMethod API should not be used in this method.
 */
virtual void postParameterChange(const string& parameterIdentifier)
{
  //add your code
}

};
REGISTER_TESTMETHOD("jtag.set_Thermostream",jtag_set_Thermostream);


###################################################################
###                  Bryan Gonzalez                             ### 
###               github.com/rbgonzalez                         ###
###	              July 23, 2008                             ###
###           gonzalez.bryan@gmail.com                          ###
################################################################### 
