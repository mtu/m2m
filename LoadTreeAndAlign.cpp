#include "LoadTreeAndAlign.h"


int TCTTreeAndAlign::LoadResource(const string &tct, const string&align)
{
    //align: 带句子长度

    this->mChsTree = new SynTreeSQ;
    if(this->mChsTree->ParseTree(tct,0)==-1)return -1; // tct分析出错

    //读取对齐数据
    using namespace boost;
    stringstream ss(align);
    int key(0),value(0);
    size_t pos;string word;
    ss>>word;
    sizeC = lexical_cast<int>(word);
    ss>>word;
    sizeE = lexical_cast<int>(word);
    while(ss>>word)
    {
        if((pos=word.find("-"))!=string::npos)
        {
            key=lexical_cast<int>(word.substr(0,pos));
            value=lexical_cast<int>(word.substr(pos+1));
        }
        C2EMap.insert(make_pair(key,value));
        E2CMap.insert(make_pair(value,key));
    }

    //记录wordIndex 2 edu
    allChnEdus = GetAllEdus(&mChsTree->root);
    if (allChnEdus.size()<2)return -1;
    GetWord2EduMap(allChnEdus, word2EduChn);
    //记录所有关系点
    allChsNodes = GetAllNode(&mChsTree->root);
    return 0;
}
void TCTTreeAndAlign::PushBackBoundaryPair(int i,int enBegin,int enBack)
{
    this->alignCh2EnMap[i]= new pair<int,int>(enBegin,enBack);
}
pair<int,int> * TCTTreeAndAlign::GetEnSpanIndex(int i)
{
    map<int,pair<int,int>* >::iterator it = this->alignCh2EnMap.find(i);
    if (it != alignCh2EnMap.end() && it->second != NULL)return it->second;
    else return NULL;
}
pair<int,int> * TCTTreeAndAlign::GetPrjSpanIndex(int i,bool chn)
{
    map<int,pair<int,int>* >::iterator it = chn?this->alignCh2EnMap.find(i):this->alignEn2ChMap.find(i);
    if ((it != alignCh2EnMap.end()|| it!= alignEn2ChMap.end()) && it->second != NULL)return it->second;
    else return NULL;
}
int TCTTreeAndAlign::GetOneRelationAndEdus(int index,RelEdu &res)
{
    RTreeNode *node = allChsNodes[index];
    vector<int>().swap(res.eduVec);
    if (node->value == "BL")res.relIndex=1;
    else if(node->value == "LG")res.relIndex=2;else if(node->value == "DJ")res.relIndex=3;
    else if(node->value == "XZ")res.relIndex=4;else if(node->value == "MD")res.relIndex=5;
    else if(node->value == "JS")res.relIndex=6;else if(node->value == "TJ")res.relIndex=7;
    else if(node->value == "ZE")res.relIndex=8;else if(node->value == "JZ")res.relIndex=9;
    else if(node->value == "LS")res.relIndex=10;else if(node->value == "UKN")res.relIndex=11;
    else if(node->value == "XX")res.relIndex=0;else if(node->value == "YG")res.relIndex=12;
    else res.relIndex=13;
    if (node->children.size()<2) return -1;//只有一个孩子,不是关系节点
    for(size_t i(0);i<node->children.size();++i){
        map<int,int>::iterator it = word2EduChn.find(node->children[i]->Begin);
        if (it!=word2EduChn.end())res.eduVec.push_back(it->second);
        else {cerr<<"wrong in word2EduChn\n";
            exit(0);
        }
    }
    return 0;
}


void TCTTreeAndAlign::GetMinAndMaxBoundary(int Begin, int Back, pair<int,int> &boundary)
{
    int a(sizeE),b(-1); map<int,int>::iterator it;
    for(int i(Begin);i<=Back;++i){
      it = C2EMap.find(i);
      if (it!=C2EMap.end()){
        if (it->second < a)a=it->second;
        if (it->second > b)b=it->second;
      }
    }
    boundary.first=a;boundary.second=b;
}
void TCTTreeAndAlign::Grow()
{
    pair<int,int> bd;
    for(size_t i(0);i<allChnEdus.size();++i)
    {
        if(allChnEdus[i]->Back == allChnEdus[i]->Begin){alignCh2EnMap[i] = NULL;continue;} //逗号跳过
        GetMinAndMaxBoundary(allChnEdus[i]->Begin,allChnEdus[i]->Back,bd);
        if (bd.first == sizeE || bd.second == -1){//未对齐跳过
            alignCh2EnMap[i] = NULL;continue;
        }
        int curPos,left,right;
        //向左延伸
        for(curPos=(bd.first-1);curPos>=0;curPos--){
          if (E2CMap.find(curPos)!=E2CMap.end())break;
        }
        left = curPos+1;
        //向右延伸
        right = bd.second > bd.first+2 ? bd.first+2:bd.second;
        alignCh2EnMap[i]= new pair<int,int>(left,right);
    }
}
int TCTTreeAndAlign::PrintWord2RelPosfile(const string &tct)
{
    this->mChsTree = new SynTreeSQ;
    if(this->mChsTree->ParseTree(tct,0)==-1)return -1;
    //记录所有关系点
    allChsNodes = GetAllNode(&mChsTree->root);

    RTreeNode *node;
    for(size_t i(0);i<allChsNodes.size();++i){
        node = allChsNodes[i];
        if(node->children.size()<2)continue;
        for(size_t j(1);j<node->children.size();++j){
            if (node->children[j]->Begin == node->children[j]->Back)continue;
            //cerr<<"<"<<node->children[j]->Begin<<" "<<node->children[j]->Back<<"> ";
            cout << node->children[j]->Begin+1 <<" "<<node->value;
            if (j==node->children.size()-1)
                cout<<"2"<<" ";
            else
                cout<<"1"<<" ";
        }
    }
    cout<<endl;
}
int TCTTreeAndAlign::PrintRelStartEnd(const string & tct)
{
    this->mChsTree = new SynTreeSQ;
    if(this->mChsTree->ParseTree(tct,0)==-1)return -1;
    //记录所有关系点
    allChsNodes = GetAllNode(&mChsTree->root);

    RTreeNode *node;
    for(size_t i(0);i<allChsNodes.size();++i){
        node = allChsNodes[i];
        if(node->value == "0" && node->children.size() > 1)cout<<"NULL ";
        else if(node->children.size()<2)cout<<"EDU ";
        else cout<<node->value<<" ";
        cout<<node->Begin<<" "<<node->Back<<" ";
    }
    cout<<endl;
}

int GetLowerBound(int wordId, bool chn)
{
    multimap<int,int>* mp = chn? &C2EMap : &E2CMap;
    multimap<int,int>::iterator it = mp->lower_bound(wordId);
    if(it==mp->end())return -1; //align to null
    return it->second;
}
int GetUpperBound(int wordId, bool Chn=true)
{
    multimap<int,int>* mp = chn? &C2EMap : &E2CMap;
    multimap<int,int>::iterator it = mp->upper_bound(wordId);
    if(it==mp->end())return -1; //align to null
    return it->second;
}
int TCTTreeAndAlign::GetEDUIndex(int wordId, bool chn)
{
  map<int,int>::iterator it;
  if(chn){
    it = word2EduChn.find(wordId);
  }
  else it = word2EduEng.find(wordId);
  if (it==word2EduChn.end() || it == word2EduEng.end())return -1;// wrong to find wordid
  else return it->second;
}

int TCTTreeAndAlign::ComputeEDUDice(int eduId,bool chn, vector<eSpanScorePair>&diceRes)
{
    //return -1: wrong
    //return 0: NULL return
    //return 1: normal
    RTreeNode *node;
    if(chn && eduId<allChnEdus.size())node = allChnEdus[eduId];
    else if(!chn && eduId < allEngEdus.size())node = allEngEdus[eduId];
    else{cerr<<"eduId "<<eduId<<" is greater than the edu\n Is chinese ? "<<chn<<endl; return -1;} //wrong

    int prjBeg, prjBack;pair<int,int> *p = GetPrjSpanIndex(eduId,chn);
    if(p == NULL)return 0;

    prjBeg = GetEDUIndex(p->first,chn);
    prjBack = GetEDUIndex(p->second,chn);
    if(prjBeg == -1 || prjBack == -1) {cerr<<"chnedu: "<<p->first<<" "<<p->second<<endl;return -1;}

    int inter = 0; ;

    int lower, upper, countAlign(0), countInter(0);
    map<int,int> *mp = chn? &this->word2EduChn : &this->word2EduEng;
    vector<RTreeNode *> *vp =chn? &this->allEngEdus : &this->allChnEdus;
    map<int,int>::iterator it=mp->begin();
    for(int i(prjBeg);i<=prjBack;++i){
      node = vp->at(i);
      for(int j(node->Begin);j<=node->Back;++j){
        lower = GetLowerBound(j,!chn);upper = GetUpperBound(j,!chn);
        if (lower == -1 || upper == -1) continue;

      }



    }


}
