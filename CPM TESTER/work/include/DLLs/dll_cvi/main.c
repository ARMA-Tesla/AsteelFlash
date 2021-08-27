//==============================================================================
//
// Title:       main.c
// Purpose:     A short description of the implementation.
//
// Created on:  14/08/2019 at 09:58:25 by DELL.
// Copyright:   . All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

//#include "main.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?

//#include <formatio.h>
#include "add.h"
#include "sub.h"
#include "div.h"
#include "mul.h"
#include <stdio.h>
#include "main.h"
#include "writeFile.h"
#include "writeFileConst.h" 

int main (int argc, char *argv[])
{ 
	double x= 3;
	double y= 4;
	double resultADD;
	double resultSUB;
	double resultDIV;
	double resultMUL; 
	
	resultADD= add(x,y);
	resultSUB= sub(x,y);
	resultDIV= div(x,y);
	resultMUL= mul(x,y);

  printf("Resultat de l'addition :%f\n",resultADD );
  printf("Resultat de la soustraction :%f\n",resultSUB ); 
  printf("Resultat de la division :%f\n",resultDIV ); 
  printf("Resultat de la multiplication :%f\n",resultMUL ); 
  getchar();
	
  
  char text [] = "hello world!";
	
  Path path = "C:\\Users\\DELL\\Desktop\\oussama.txt";	
  
  WriteFile(text, path);
  WriteFileConst(text);
	return 0;
	
	// TO DO:
}
