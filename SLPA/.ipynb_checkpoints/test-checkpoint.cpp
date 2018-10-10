# 13 "CommonFuns.cpp"
#include "CommonFuns.h"

using namespace std;
# 29 "CommonFuns.cpp"
void sortMapInt_Int(map<int,int> & words, vector<pair<int,int> > & wordsvec){




 for ( map<int,int>::iterator it=words.begin() ; it != words.end(); it++ ){
  wordsvec.push_back(*it);
 }

 sort (wordsvec.begin(), wordsvec.begin()+wordsvec.size(), sort_pair_INT_INT());
}


double myround( double value ){
 return floor( value + 0.5 );
}

void createHistogram(map<int,int>& hist, const vector<int>& wordsList){

 hist.clear();
 map<int,int>::iterator mit;

 for(int i=0;i<wordsList.size();i++){
  int key=wordsList[i];

  if(hist.count(key)>0){

   mit=hist.find(key);
   int count=mit->second+1;


   hist.erase (mit);
   hist.insert(pair<int,int>(key,count));
  }
  else{

   hist.insert(pair<int,int>(key,1));
  }
 }

}

string int2str(int i){
 string s;
 stringstream out;
 out << i;
 s = out.str();

 return s;
}

string dbl2str(double f){
 string s;
 stringstream out;
 out << f;
 s = out.str();

 return s;
}
