#ifndef LOADTREEANDALIGN_H_INCLUDED
#define LOADTREEANDALIGN_H_INCLUDED

#include"SynTreeSQ.h"

struct RelEdu
{
   int relIndex;
   vector<int> eduVec;
};

typedef pair<pair<int,int>, float> eSpanScorePair;
class TCTTreeAndAlign{

public:
    TCTTreeAndAlign(){};
    ~TCTTreeAndAlign(){};


    vector<RTreeNode *> allChsNodes;
    vector<RelationTree::TreeNode *> allChnEdus; //保存所有的EDU for chn
    vector<RTreeNode *> allEngEdus; // 保存所有的EDU for eng

public:
    SynTreeSQ *GetChsTree(){return mChsTree;}
    int LoadResource(const string &tct, const string&align);
    void PushBackBoundaryPair(int i,int enBegin,int enEnd); //插入eduIndex以及其对应的projected span的开始和结束
    pair<int,int> * GetEnSpanIndex(int i);
    pair<int,int> * GetPrjSpanIndex(int i,bool chn=true); //Get the projected span [begin, back], or return null
    int GetOneRelationAndEdus(int index,RelEdu &res);
    void GetMinAndMaxBoundary(int Begin, int Back, pair<int,int> &boudary);
    void Clear(){C2EMap.clear();E2CMap.clear();alignCh2EnMap.clear();
        vector<RTreeNode *>().swap(allChsNodes);
        vector<RTreeNode *>().swap(allChnEdus);
        word2EduChn.clear();}
    void Grow(); //增长式的插入EDUINDEX以及其对应的左边界projected span，窗口为[-X,2]，即向左延伸到前一个EDU的右边界PS，向右延伸最多2个词，
                //记录其开始和结束位置
    void PrintMap(){
      map<int,pair<int,int>*>::iterator it = alignCh2EnMap.begin();
      while(it!=alignCh2EnMap.end()){
          if (it->second == NULL) {it++;continue;}
          cerr<<it->first<<":"<<it->second->first<<"-"<<it->second->second<<endl;
      it++;}
    };

    int GetEnSize(){return sizeE;}
    int GetChSize(){return sizeC;}

    //to print the word id to relation and pos file
    int PrintWord2RelPosfile(const string & tctstr);

    //print rel-[start,end] as rel start end
    int PrintRelStartEnd(const string & tctstr);


    //get the edu index for word id
    int GetEDUIndex(int wordId, bool Chn=true);
    int GetLowerBound(int wordId, bool Chn=true);
    int GetUpperBound(int wordId, bool Chn=true);
    int ComputeEDUDice(int eduId,bool Chn, vector<eSpanScorePair> &diceRes);


private:
    map<int,pair<int,int>* > alignCh2EnMap; //记录eduIndex以及其对应的projected span
    map<int,pair<int,int>* > alignEn2ChMap;

    multimap<int,int> C2EMap;
    multimap<int,int> E2CMap;
    SynTreeSQ *mChsTree;//保存tct树
    SynTreeSQ *mEngTree;//保存eng 树




    map<int,int> word2EduChn;
    map<int,int> word2EduEng;


    int sizeC;
    int sizeE;

};

#endif // LOADTREEANDALIGN_H_INCLUDED
