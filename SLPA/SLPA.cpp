# 13 "SLPA.cpp"
#include "SLPA.h"
#include "CommonFuns.h"
#include "CommonFuns_TMP.h"
#include "rndNumbers.h"
#include "fileOpts.h"

#include <pthread.h>


typedef std::tr1::unordered_map<int, int> UOrderedH_INT_INT;

bool SLPA::isDEBUG=true;

SLPA::SLPA(string inputFileName,vector<double> THRS,int maxRun,int maxT,string outputDir,bool isUseLargestComp,int numThreads) {






 string a,b;
 fileName_net=inputFileName;
 extractFileName_FullPath(inputFileName,netName,a,b);

 networkPath="";
 net=new Net(networkPath,netName,fileName_net);




 for(int i=0;i<THRS.size();i++)
  this->THRS.push_back(THRS[i]);

 this->maxRun=maxRun;
 this->maxT=maxT;

 isSyn=false;
 this->isUseLargestComp=isUseLargestComp;



 this->outputDir=outputDir;

 this->numThreads=numThreads;



 start();
}

SLPA::~SLPA() {
 delete net;
}

void SLPA::pre_initial_THRCS(){
 THRCS.clear();
 for(int i=0;i<THRS.size();i++){
  THRCS.push_back((int)myround(THRS[i]*maxT));
 }
}


void SLPA::start(){



 bool isSymmetrize=true;

 net->readNetwork_EdgesList(fileName_net,isUseLargestComp,isSymmetrize);
 cout<<"Network info: N="<<net->N<< " M="<<net->M<<"(symmetric)"<<endl;
 cout<<"load "<<fileName_net<< " done.."<<endl;







 pre_initial_THRCS();




 for(int run=1;run<=maxRun;run++){

  cout<<" run="<<run<<"......"<<endl;


  initWQueue_more();


  if(isSyn){

  }
  else{
   GLPA_asyn_pointer();
  }



  post_createWQHistogram_MapEntryList();



  for(int i=0;i<THRCS.size();i++){
   int thrc=THRCS[i];
   double thrp=THRS[i];

   time_t st=time(NULL);
   cout<<"Progress: Thresholding thr="<<thrp<<"......."<<endl;
   string fileName=outputDir+"SLPA_"+netName+"_run"+int2str(run)+"_r"+dbl2str(thrp)+ ".icpm";

   if(isDEBUG) cout<<"cpm="<<fileName<<endl;
   post_threshold_createCPM_pointer(thrc,fileName);


  }
 }
}

void SLPA::initWQueue_more(){
 time_t st=time(NULL);
 cout<<"Progress: Initializing memory......."<<endl;


 NODE *v;
 for(int i=0;i<net->N;i++){

  v=net->NODES[i];



  v->WQueue.push_back(v->ID);
# 154 "SLPA.cpp"
 }

 cout<<" Take :" <<difftime(time(NULL),st)<< " seconds."<<endl;
}

void SLPA::GLPA_asyn_pointer(){



 time_t st=time(NULL);

 NODE *v,*nbv;
 int label;
 vector<int> nbWs;
 map<int,NODE *>::iterator mit;


 cout<<"Start iteration:";

 for(int t=1;t<maxT;t++){


  cout<<"*"<<flush;
  srand (time(NULL));
  random_shuffle (net->NODES.begin(), net->NODES.end());




  for(int i=0;i<net->N;i++){
   v=net->NODES[i];


   nbWs.clear();

   for(int j=0;j<v->numNbs;j++){
    nbv=v->nbList_P[j];
    nbWs.push_back(nbv->WQueue[mtrand2.randInt(nbv->WQueue.size()-1)]);
   }


   label=ceateHistogram_selRandMax(nbWs);


   v->WQueue.push_back(label);
  }


 }

 cout<<endl;
 cout<<"Iteration is over (takes "<<difftime(time(NULL),st)<< " seconds)"<<endl;
}

int SLPA::ceateHistogram_selRandMax(const vector<int>& wordsList){
 int label;
 map<int,int> hist;
 map<int,int>::iterator mit;




 createHistogram(hist, wordsList);







 vector<pair<int,int> > pairlist;
 sortMapInt_Int(hist, pairlist);
# 235 "SLPA.cpp"
 int maxv=pairlist[0].second;
 int cn=1;


 for(int i=1;i<pairlist.size();i++){
  if(pairlist[i].second==maxv)
   cn++;
  else
   break;
 }


 if(cn==1)
  label=pairlist[0].first;
 else{


  int wind=mtrand1.randInt(cn-1);



  label=pairlist[wind].first;
 }




 return label;
}


void SLPA::post_createWQHistogram_MapEntryList(){
 NODE *v;
 map<int,int> WQHistgram;

 for(int i=0;i<net->N;i++){
  v=net->NODES[i];


  WQHistgram.clear();
  createHistogram(WQHistgram, v->WQueue);



  v->WQHistMapEntryList.clear();
  sortMapInt_Int(WQHistgram, v->WQHistMapEntryList);


  v->WQueue.clear();
 }


}

void SLPA::dothreshold_createCPM_pointer(int thrc,vector<vector<int>* >& cpm){
 time_t st=time(NULL);


 map<int,int> w_comidTable;
 map<int,int>::iterator mit;

 int comid=-1;

 NODE *v;
 int ov_cn=0;
 for(int i=0;i<net->N;i++){
  v=net->NODES[i];


  vector<int> WS;
  post_thresholding(v->WQHistMapEntryList,thrc,WS);

  if(WS.size()<1) cout<<"ERROR:empty WS"<<endl;
  if(WS.size()>1) ov_cn++;


  for(int j=0;j<WS.size();j++){
   int label=WS[j];


   if(w_comidTable.count(label)==0){
    comid++;
    w_comidTable.insert(pair<int,int>(label, comid));




    vector<int>* avector=new vector<int>();
    cpm.push_back(avector);
   }


   mit=w_comidTable.find(label);
   int v_comid=mit->second;


   cpm[v_comid]->push_back(v->ID);
  }
 }


}


void SLPA::post_threshold_createCPM_pointer(int thrc,string fileName){
 bool isDEBUG=false;
 time_t st;


 vector<vector<int>* > cpm;




 dothreshold_createCPM_pointer(thrc,cpm);
# 372 "SLPA.cpp"
 st=time(NULL);





 if(numThreads==0)
  cpm=post_removeSubset_UorderedHashTable_cpmpointer(cpm);
 else
  cpm=post_removeSubset_UorderedHashTable_cpmpointer_MultiThd(cpm);

 cout<<"removeSubset takes :" <<difftime(time(NULL),st)<< " seconds."<<endl;







 st=time(NULL);
 sort_cpm_pointer(cpm);


 st=time(NULL);
 write2txt_CPM_pointer(fileName,cpm);
# 405 "SLPA.cpp"
 for(int i=0;i<cpm.size();i++)
  delete cpm[i];

}


void SLPA::post_thresholding(vector<pair<int,int> >& pairList, int thrc, vector<int>& WS){





 int label;







 int maxv=pairList[0].second;

 if(maxv<=thrc){

  int cn=1;
  for(int i=1;i<pairList.size();i++){
   if(pairList[i].second==maxv)
    cn++;
   else
    break;
  }


  if(cn==1)
   label=pairList[0].first;
  else{

   int wind=mtrand2.randInt(cn-1);

   label=pairList[wind].first;
  }


  WS.push_back(label);

 }
 else{

  for(int i=0;i<pairList.size();i++){
   if(pairList[i].second>thrc){
    label=pairList[i].first;
    WS.push_back(label);
   }
   else
    break;
  }
 }
}

void SLPA::sort_cpm_pointer(vector<vector<int>* >& cpm){

 for(int i=0;i<cpm.size();i++){

  sort(cpm[i]->begin(),cpm[i]->end(),sort_INT_INC());
 }


 sortVecVec_bySize_pointer<int>(cpm);
}


vector<vector<int>* > SLPA::post_removeSubset_UorderedHashTable_cpmpointer(vector<vector<int>* >& cpm){
 time_t st;



 vector<vector<int>* > newcpm;



 st=time(NULL);
 sort_cpm_pointer(cpm);





 st=time(NULL);


 vector<UOrderedH_INT_INT* > vectHTable;

 for(int i=0;i<cpm.size();i++){
  UOrderedH_INT_INT* H=new UOrderedH_INT_INT;
  for(int j=0;j<cpm[i]->size();j++)

   H->insert(pair<int,int>((*cpm[i])[j],(*cpm[i])[j]));

  vectHTable.push_back(H);
 }





 int indicators[cpm.size()];
 for(int i=0;i<cpm.size();i++)
  indicators[i]=1;


 bool issubset;
 for(int i=cpm.size()-1;i>0;i--){
  for(int j=0;j<i;j++){


   issubset=true;
   for(int k=0;k<cpm[i]->size();k++)
    if(vectHTable[j]->count((*cpm[i])[k])==0){
     issubset=false;
     break;
    }



   if(issubset){
    indicators[i]=0;
    break;
   }
  }
 }


 for(int i=0;i<cpm.size();i++){
  if(indicators[i]==0) continue;

  newcpm.push_back(cpm[i]);
 }


 for(int i=0;i<vectHTable.size();i++)
  delete vectHTable[i];

 return newcpm;
}

void SLPA::write2txt_CPM_pointer(string fileName,vector<vector<int>* >& cpm) {
 vector<string> data;

 for(int i=0;i<cpm.size();i++){

  vector<int>& oneComm=*cpm[i];

  string line;
  for(int j=0;j<oneComm.size();j++){
   line+=int2str(oneComm[j]);
   line+=" ";
  }
  data.push_back(line);
 }


 writeToTxt(fileName, false, data);
}




void SLPA::sort_cpm(vector<vector<int> >& cpm){

 for(int i=0;i<cpm.size();i++){
  sort(cpm[i].begin(),cpm[i].end(),sort_INT_INC());
 }


 sortVecVec_bySize<int>(cpm);
}




void SLPA::decomposeTasks(int numTasks,int numThd,int stInds[],int enInds[]){
 int rem=numTasks%numThd;
 int step=(numTasks-rem)/numThd;

 for(int i=0;i<numThd;i++){
  stInds[i]=i*step;
  enInds[i]=(i+1)*step-1;
 }
 enInds[numThd-1]+=rem;

 if(false){
  cout<<"----------------decomposeTasks---------------"<<endl;
  cout<<"rem="<<rem<<" step="<<step<<endl;
  for(int i=0;i<numThd;i++){
   cout<<stInds[i]<<" "<<enInds[i]<<endl;
  }
 }

}
void *SLPA::removesubset_onethread(void *threadarg){







 struct thread_data *my_data;
 my_data = (struct thread_data *) threadarg;

 if(false) cout<<"startind="<<my_data->startind<<" endind="<<my_data->endind<<endl;



 vector<vector<int>* >& cpm=my_data->cpm;
 vector<UOrderedH_INT_INT* >& vectHTable=my_data->vectHTable;
 int *indicators=my_data->pIndicator;



 bool issubset;


 for(int z=my_data->endind;z>=my_data->startind;z--){
  int i=z;


  for(int j=0;j<i;j++){


   issubset=true;
   for(int k=0;k<cpm[i]->size();k++)
    if(vectHTable[j]->count((*cpm[i])[k])==0){
     issubset=false;
     break;
    }



   if(issubset){
    indicators[i]=0;
    break;
   }
  }
 }





 pthread_exit(NULL);
}



vector<vector<int>* > SLPA::post_removeSubset_UorderedHashTable_cpmpointer_MultiThd(vector<vector<int>* >& cpm){
 time_t st;
 bool isDEBUG=false;
 cout<<"removeSubset (Multiple threads)............."<<endl;

 vector<vector<int>* > newcpm;



 st=time(NULL);
 sort_cpm_pointer(cpm);





 st=time(NULL);


 vector<UOrderedH_INT_INT* > vectHTable;

 for(int i=0;i<cpm.size();i++){
  UOrderedH_INT_INT* H=new UOrderedH_INT_INT;
  for(int j=0;j<cpm[i]->size();j++)

   H->insert(pair<int,int>((*cpm[i])[j],(*cpm[i])[j]));

  vectHTable.push_back(H);
 }


 int indicators[cpm.size()];
 for(int i=0;i<cpm.size();i++)
  indicators[i]=1;

 int numTasks=cpm.size();
 int numThd=numThreads;

 int stInds[numThd];
 int enInds[numThd];

 decomposeTasks(numTasks, numThd, stInds, enInds);

 struct thread_data thread_data_array[numThd];

 pthread_t threads[numThd];
 pthread_attr_t attr;
 void *status;


 pthread_attr_init(&attr);
 pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

 int rc;
 long t;
 for( t=0; t<numThd; t++){
  if(isDEBUG) cout<<"creating thread "<<t<<endl;
  thread_data_array[t].startind=stInds[t];
  thread_data_array[t].endind=enInds[t];
  thread_data_array[t].pIndicator=indicators;
  thread_data_array[t].cpm=cpm;
  thread_data_array[t].vectHTable=vectHTable;

  rc = pthread_create(&threads[t], NULL, removesubset_onethread, (void *) &thread_data_array[t]);
  if (rc){
   printf("ERROR; return code from pthread_create() is %d\n", rc);
   exit(-1);
  }
 }


 pthread_attr_destroy(&attr);


 for(t=numThd-1; t>=0; t--) {
  rc=pthread_join(threads[t], &status);
  if (rc) {
   cout<<"ERROR; return code from pthread_join() is "<<rc<<endl;
   exit(-1);
  }
 }


 if(isDEBUG) for(int i=0;i<cpm.size();i++)
  cout<<"indicator["<<i<<"]="<<indicators[i]<<endl;



 for(int i=0;i<cpm.size();i++){
  if(indicators[i]==0) continue;

  newcpm.push_back(cpm[i]);
 }


 for(int i=0;i<vectHTable.size();i++)
  delete vectHTable[i];

 return newcpm;
}
