# 13 "Net.cpp"
#include "Net.h"
#include "fileOpts.h"
#include "CommonFuns.h"
#include "CommonFuns_TMP.h"
#include <cstdlib>
#include <exception>
#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

#include "SLPA.h"



typedef std::tr1::unordered_set<NODE *> UOrderedSet_NODEptr;


struct sortIDs {
 bool operator() (int i,int j) { return (i<j);}
} sortIDs_inc;


Net::Net(string path,string name,string fname){
 networkPath=path;
 netName=name;

 fileName=fname;
}


Net::~Net() {
 while (!NODES.empty()) {
  delete NODES.back();
  NODES.pop_back();
 }
}

int Net::getNumberofEdges(){
 int m=0;
 for(int i=0;i<NODES.size();i++)
  m+=NODES[i]->numNbs;

 return m;
}

void Net::readNet(string fileName,bool isSymmetrize){





 string oneLine, whiteSpace=" \t\n";
 fstream fp;
 fp.open(fileName.c_str(),fstream::in);

 if(fp.is_open()){


  while ( fp.good()){
   getline (fp,oneLine);



   if(oneLine.find_first_not_of(whiteSpace)==string::npos) continue;


   if(!isdigit(oneLine.at(oneLine.find_first_not_of(whiteSpace)))) continue;



   int fromID,toID;
   double w=1.0;

   stringstream linestream(oneLine);
   if((linestream>>fromID) && (linestream>>toID)){



    if(fromID==toID) continue;

    pre_ReadInOneEdge(fromID,toID);
    if(isSymmetrize)
     pre_ReadInOneEdge(toID,fromID);
   }
  }

  fp.close();
 }
 else{
  cout<<"open failed"<<endl;
  exit(1);
 }
}

void Net::readNetwork_EdgesList(string fileName, bool isUseLargestComp, bool isSymmetrize){



 if(!isFileExist(fileName.c_str())){
  cout<< fileName << " not found!" <<endl;
  exit(1);
 }

 NODES.clear();
 NODESTABLE.clear();





 time_t st=time(NULL);

 readNet(fileName,isSymmetrize);




 pre_convert_nbSet2_nbList();
 N=NODES.size();
 M=getNumberofEdges();

 cout<<"Reading in the network takes :" <<difftime(time(NULL),st)<< " seconds."<<endl;




 if(isUseLargestComp){
  cout<<"Using largest connected component only.......\n";

  pre_findGiantComponents();
 }
}

void Net::pre_ReadInOneEdge(int fromID,int toID){
 map<int,NODE *>::iterator mit;
 NODE *vp;

 if (NODESTABLE.count(fromID)>0){
  mit=NODESTABLE.find(fromID);
  vp=mit->second;

  vp->nbSet.insert(toID);
 }
 else{
  vp=new NODE(fromID);
  vp->nbSet.insert(toID);

  NODES.push_back(vp);
  NODESTABLE.insert(pair<int,NODE *>(fromID,vp));
 }
}

void Net::showVertices(){
 cout<<"-----------------------"<<endl;
 cout<<NODES.size()<<endl;
 for(int i=0;i<NODES.size();i++){
  NODE * vp=NODES[i];
  cout<< vp->ID<<endl;
 }
}

void Net::showVertices_Table(){
 cout<<"-----------------------"<<endl;
 cout<<NODESTABLE.size()<<endl;

 map<int,NODE *>::iterator it;
 for(it=NODESTABLE.begin();it!=NODESTABLE.end();it++){
  NODE * v=it->second;
  cout<< v->ID<<endl;

  for(int j=0;j<v->numNbs;j++)
   cout<< "->" << v->nbList_P[j]->ID<<endl;
 }
}

void Net::pre_convert_nbSet2_nbList(){




 UOrderedSet_INT::iterator sit;
 NODE * v;
 NODE * nbv;


 for(int i=0;i<NODES.size();i++){
  v=NODES[i];

  v->nbList_P.clear();
  for(sit=v->nbSet.begin();sit!=v->nbSet.end();sit++){
   nbv=NODESTABLE.find(*sit)->second;
   v->nbList_P.push_back(nbv);
  }
  v->numNbs=v->nbList_P.size();
 }
}
# 536 "Net.cpp"
void Net::pre_findGiantComponents(){
 time_t st=time(NULL);



 vector<vector<NODE *> > coms;

 UOrderedSet_NODEptr UnExpSet(NODES.begin(),NODES.end());
 UOrderedSet_NODEptr WorkingSet;

 NODE *v;
 NODE *nbv;

 while(!UnExpSet.empty()){

  vector<NODE *> com;

  v=*UnExpSet.begin();


  WorkingSet.insert(v);
  UnExpSet.erase(v);
  v->status=1;
  com.push_back(v);


  while(!WorkingSet.empty()){
   v=*WorkingSet.begin();


   for(int i=0;i<v->nbList_P.size();i++){
    nbv=v->nbList_P[i];

    if(nbv->status==0){

     WorkingSet.insert(nbv);
     UnExpSet.erase(nbv);
     nbv->status=1;
     com.push_back(nbv);
    }
   }


   WorkingSet.erase(v);
  }



  coms.push_back(com);
 }





 sortVecVec_bySize<NODE* >(coms);

 cout<<"check the sorting of sizes:"<<endl;
 int sum=0;
 for(int i=0;i<coms.size();i++){
  cout<<coms[i].size()<<endl;
  sum+=coms[i].size();
 }
 cout<<" sum size="<<sum<<" N="<<N<<endl;




 NODES.clear();
 NODES=coms[0];

 NODESTABLE.clear();
 for(int i=0;i<NODES.size();i++)
  NODESTABLE.insert(pair<int,NODE *>(v->ID,NODES[i]));

 N=NODES.size();
 M=getNumberofEdges();





 for(int i=1;i<coms.size();i++){
  for(int j=0;j<coms[i].size();j++)
   delete coms[i][j];
 }



 cout<<"Finding largest component in the network takes :" <<difftime(time(NULL),st)<< " seconds."<<endl;
}
