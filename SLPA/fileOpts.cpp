# 13 "fileOpts.cpp"
#include "fileOpts.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <stdlib.h>
using namespace std;

bool isFileExist(const char* fileName){
 fstream fp;
 fp.open(fileName,fstream::in);

 if(fp.is_open()){
  fp.close();
  return true;
 }else
  return false;
}

void writeToTxt(string fileName, bool isappend, vector<string>& data){


 fstream fp;

 if(isappend)
  fp.open(fileName.c_str(),fstream::app);
 else
  fp.open(fileName.c_str(),fstream::out);

 if(fp.is_open()){

  for(int i=0;i<data.size();i++){
   fp<<data[i]<<endl;
  }
  fp.close();


 }
 else{
  cout<<"open failed"<<endl;
 }
}

void extractFileName_FullPath (const string& str,string& shortfile, string& file, string& path)
{
 size_t found;
 found=str.find_last_of("/\\");


 if(found==string::npos){
  path="";
  file=str;
 }
 else{
  path=str.substr(0,found);
  file=str.substr(found+1);
 }


 found=file.find_last_of(".");
 if(found==string::npos){
  shortfile=file;
 }
 else{
  shortfile=file.substr(0,found);
 }


}
