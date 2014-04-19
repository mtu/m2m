#include "SynTreeSQ.h"

using namespace std;
using namespace RelationTree;

void RecurseNode(RTreeNode *node,vector<RTreeNode *>&vec)
{

  if(node->children.empty()){return;}
  for(size_t i(0);i<node->children.size();++i){
    vec.push_back(node->children[i]);
    RecurseNode(node->children[i],vec);
  }
}
void RecurseNodeForEdu(RTreeNode *node,vector<RTreeNode *>&vec)
{

  if(node->children.empty()){vec.push_back(node);return;}
  for(size_t i(0);i<node->children.size();++i){
    //vec.push_back(node->children[i]);
    RecurseNodeForEdu(node->children[i],vec);
  }
}
vector<RTreeNode *> GetAllNode(RTreeNode *root)
{
  vector<RTreeNode *> vec;
  RecurseNode(root,vec);
  //cerr<<vec.size()<<endl;
  return vec;
}
vector<RTreeNode *> GetAllEdus(RTreeNode *root)
{
  vector<RTreeNode *> vec;
  RecurseNodeForEdu(root,vec);
  //cerr<<vec.size()<<endl;
  return vec;
}
void GetWord2EduMap(vector<RelationTree::TreeNode *> &allEdus, map<int,int> &word2Edu)
{
    RelationTree::TreeNode *curNode;
    for(size_t i(0);i<allEdus.size();++i){
      curNode = allEdus[i];
      for(size_t j(curNode->Begin);j<=curNode->Back;++j)
        word2Edu[j] = i;
    }
}

int RelationTree::LeftBranket(const string& s, string & str_flw_lb)
{
	if (s.length() < 2 )
        return -1;
    else if(s.find("[")!=0){str_flw_lb=s;return 0;}
//    else if (s[1] == ')')
//    {
//        str_flw_lb = s;return 0;
//    }
    else if(s.find("[#")==0){str_flw_lb = s.substr(2);return 1;}
    else{ str_flw_lb = s.substr(1);return 1;}

}
int RightBranket(const string& s, string & str_bf_rb,int mode)
{//mode = 0: old format
    //mode=1: new format
    if(mode == 1){
       for(size_t i(0);i<s.length()-1;i += 2){
        if( s[i]=='#' && s[i+1]==']'){continue;}
        else {
	    str_bf_rb=s;
	    return 0;
        }
      }
      return s.length()/2;
    }

    else{
      for(size_t i(0);i<s.length();++i){
        if( s[i]==']')continue;
        else {
	    str_bf_rb=s;
	    return 0;
        }
      }
      return s.length();
    }
}
void SynTreeSQ::test()
{
    string a="d";
    vector<string> vec;
    vec.push_back(a);
}
int SynTreeSQ::ParseTree(const string&s, int mode, int format)
{
    formatMode = format;
using RelationTree::LeftBranket;
using RelationTree::TreeNode;
    stringstream ss(s);
    string word,str_flw_lb,str_bf_rb;
    TreeNode *currentNode(&root);
    TreeNode *tmpNode = new TreeNode("",NULL);

    int emptyLB=0,check,brkNum,eduIndex(0);
    string s_copy(s);
    boost::trim(s_copy);
    if(s_copy.find("[")!=0) return -1;

    while(ss>>word)
    {
       brkNum=0;
    //   cerr<<word<<endl;

       if (word[0]=='['&&word[word.size()-1]!=']'){ //check word starting with (
            check = LeftBranket(word,str_flw_lb);
            if(check==1)
            {
                tmpNode = new TreeNode(str_flw_lb,currentNode);tmpNode->Begin = mode;
                tmpNode->level = currentNode->level+1;
                currentNode->children.push_back(tmpNode);
                currentNode = tmpNode; continue;
            }// else if(word.find("/")!=string::npos)str_bf_rb=word;
	         //else {cerr<<"wrong left brankets\n\n";exit(1);}
            else{str_bf_rb=word;}
        }//else if(word.find("/")!=string::npos) str_bf_rb = word;
	else if(word[word.length()-1]==']')brkNum = RightBranket(word,str_bf_rb,formatMode);
	//else {cerr<<"wrong right brankets\n\n";exit(1);}
	else{str_bf_rb=word;}
    if (str_bf_rb.compare("")>0 && brkNum ==0){
        if(mode!=-1) {

            nodeIdMap[mode++]= currentNode;

            this->sentenceVec.push_back(str_bf_rb);}  //for node id map
            currentNode->Back = mode-1;
    }
        if (brkNum<=0)continue;
        for (;brkNum>0;brkNum--)
        {
           currentNode = currentNode->parent;
           currentNode->Back = mode-1;
        }
    }

    return 0;
}

int SynTreeSQ::ClearTree()
{
    TreeNode * currentNode = &root;
    while(currentNode->parent != NULL || !currentNode->children.empty())
    {
        while(!currentNode->children.empty())
            currentNode = currentNode->children.back();

        currentNode = currentNode->parent;
        for(size_t i(0);i<currentNode->children.size();++i)
            delete currentNode->children[i];
        vector<TreeNode *>().swap(currentNode->children);
        currentNode = currentNode->parent;
        delete currentNode->children.back();
        currentNode->children.pop_back();
    }
    return 0;
}

void SynTreeSQ::PrintTree(TreeNode *node)
{
    TreeNode *currentNode = node;
    for(size_t i(0);i< node->children.size();++i){
        currentNode = node->children[i];
        if (currentNode->children.empty())
            cerr<<' '<<GetSpanWithNode(currentNode);
        else{
                cerr<<" ["<<currentNode->value/*<<"//"<<currentNode->Begin<<"//"<<currentNode->Back*/;
                PrintTree(currentNode);cerr<<" ]";
        }
    }
}
void SynTreeSQ::PrintTreeOri(TreeNode *node)
{
    TreeNode *currentNode = node;
    for(size_t i(0);i< node->children.size();++i){
        currentNode = node->children[i];
        if (currentNode->children.empty())
            cout<<" ["<<currentNode->value<<' '<<GetSpanWithNode(currentNode)<<" ]";
        else{
                cout<<" ["<<currentNode->value/*<<"//"<<currentNode->Begin<<"//"<<currentNode->Back*/;
                PrintTreeOri(currentNode);cout<<" ]";
        }
    }
}
TreeNode *SynTreeSQ::GetNodeWithId(int id)
{
    return nodeIdMap[id];
}


void SynTreeSQ::GetStrVec(int begin, int back,vector<string> &result)
{
    if(back>nodeIdMap.size()-1 || back == -1)back = nodeIdMap.size()-1;
    for(;begin<=back;++begin)
        result.push_back(nodeIdMap[begin]->value);
}
string SynTreeSQ::GetSpanWithNode(TreeNode* node)
{
   string s="";
   for (size_t i(node->Begin);i<=node->Back;++i)
     s += sentenceVec[i]+" ";
   boost::trim(s);
   return s;
}
set<string> keySet;
void InitKeySet()
{
  keySet.insert("BL");  keySet.insert("LG");keySet.insert("DJ");  keySet.insert("XZ");
  keySet.insert("YG");  keySet.insert("MD");keySet.insert("JS");  keySet.insert("TJ");
  keySet.insert("ZE");  keySet.insert("JZ");keySet.insert("LS");  keySet.insert("UKN"); keySet.insert("XX");
}
bool IsKeyWord(const string& s,string &res)
{
//  size_t pos = s.find("-");
//  if (pos==string::npos)return false;
//  res = s.substr(pos+1);
  res=s;
  boost::trim(res);
  if (keySet.find(res)!=keySet.end())return true;
  else return false;
}

void SynTreeSQ::PrintRelTree(TreeNode*node, int mode)
{
 TreeNode *currentNode = node;
 string res="";
    for(size_t i(0);i< node->children.size();++i)
    {
        if (mode==1){cout<<" ["<<i<<":";}
        currentNode = node->children[i];
        if (currentNode->children.empty())
            cout<<' '<<GetSpanWithNode(currentNode);
        else{
	      if(IsKeyWord(currentNode->value,res)){
                cout<<" ["<<res /*<<"//"<<currentNode->Begin<<"//"<<currentNode->Back*/;
                PrintRelTree(currentNode,1);cout<<" ]";
          }
	      else{
	        PrintRelTree(currentNode,0);
	      }
	    }
	    if (mode==1){cout<<" ]";}
    }
}
void SynTreeSQ::PrintSegTree(TreeNode*node, int mode)
{
 TreeNode *currentNode = node;
 string res="";
    for(size_t i(0);i< node->children.size();++i)
    {
        if (mode==1){cout<<" ###";}
        currentNode = node->children[i];
        if (currentNode->children.empty())
            cout<<' '<<GetSpanWithNode(currentNode);
        else
            {
	      if(IsKeyWord(currentNode->value,res)){
                //cout<<" ###"<<res /*<<"//"<<currentNode->Begin<<"//"<<currentNode->Back*/;
                PrintSegTree(currentNode,1);cout<<" ###";
	      }
	      else{
	        PrintSegTree(currentNode,0);
	      }
	    }
	if (mode==1){cout<<" ###";}
    }
}
void SynTreeSQ::PrintRelTree_1(TreeNode*node, int mode)
{
    TreeNode *currentNode = node;
    string res="";
    for(size_t i(0);i< node->children.size();++i)
    {
        currentNode = node->children[i];
        if(mode!=0 && currentNode->children.empty()){cout<<" ["<<i<<":";}
        if (currentNode->children.empty())
            cout<<' '<<GetSpanWithNode(currentNode);
        else if(currentNode->children.size()==1){
            PrintRelTree_1(currentNode, 1);
        }
        else{
            cout<<" ["<<currentNode->value;
	        PrintRelTree_1(currentNode,1);
	        cout<<" ]";
	      }
        if(mode!=0 && currentNode->children.empty())cout<<" ]";
    }
}
void SynTreeSQ::Binarization(TreeNode *node)
{
 // cerr<<node->value<<" "<<node->level<<endl;
  while (node->children.size()>2)
  {
    TreeNode *tmp = new TreeNode("XX",node);tmp->Begin=node->children[0]->Begin;tmp->Back=node->children[1]->Back;
    node->children[0]->parent=tmp;node->children[1]->parent=tmp;
    tmp->children.push_back(node->children[0]);tmp->children.push_back(node->children[1]);
    tmp->children[0]->BinSib = tmp->children[1];tmp->children[1]->BinSib = tmp->children[0];
    node->children.erase(node->children.begin());node->children[0]=tmp;
  }
  if (node->children.empty())return;
  Binarization(node->children[0]);
  if(node->children.size()>1)
    Binarization(node->children[1]);
}
void SynTreeSQ::Convert2Tuple(RelationTree::TreeNode* node,set<string>&vec)
{
    if(node->children.empty())return;
    int beg,mid,back;
    beg = node->children[0]->Begin;
    mid = node->children[0]->Back;
    if(node->children.size()==1){
        back = node->children[0]->Back; Convert2Tuple(node->children[0],vec);
    }
    else{
        back = node->children[1]->Back;
        Convert2Tuple(node->children[0],vec);
        Convert2Tuple(node->children[1],vec);
    }
    vec.insert(boost::lexical_cast<string>(beg)+" "+boost::lexical_cast<string>(mid)+" "+boost::lexical_cast<string>(back));
}
void SynTreeSQ::Convert2TupleComplete(RelationTree::TreeNode* node,set<string>&vec)
{
    if(node->children.empty())return;
    int beg,mid,back;
    beg = node->children[0]->Begin;
    mid = node->children[0]->Back;
    if(node->children.size()==1){
        back = node->children[0]->Back; Convert2TupleComplete(node->children[0],vec);
    }
    else{
        back = node->children[1]->Back;
        Convert2TupleComplete(node->children[0],vec);
        Convert2TupleComplete(node->children[1],vec);
        if(node->value.find(":")!=string::npos)node->value="UKN";
        vec.insert(node->value+" "+boost::lexical_cast<string>(beg)+" "+boost::lexical_cast<string>(mid)+" "+boost::lexical_cast<string>(back));
    }
}
float SynTreeSQ::EvaluationForStr(set<string>&canSet,set<string>&refSet)
{
    cerr<<canSet.size()<<" "<<refSet.size()<<endl;
    set<string>::iterator iter=refSet.begin();
    int match(0);
    while(iter!=refSet.end()){
        if(canSet.find(*iter)!=canSet.end())match++;
        iter++;
    }
    return float(match);
}
void SynTreeSQ::Bin2NonBin(RelationTree::TreeNode *node)
{
    if(node->children.empty())return;
    vector<RelationTree::TreeNode *>childVec;
    for(size_t i(0);i<node->children.size();++i){
        Bin2NonBin(node->children[i]);
        if(node->children[i]->value=="XX"){
            copy(node->children[i]->children.begin(),node->children[i]->children.end(),back_insert_iterator<vector<RelationTree::TreeNode*> >(childVec));

        }
        else{
            childVec.push_back(node->children[i]);
        }
    }
    node->children = childVec;
    vector<RelationTree::TreeNode*>().swap(childVec);

    for(size_t i(0);i<node->children.size();++i){
        node->children[i]->parent = node;
    }
}
void SynTreeSQ::Convert2TupleNonBin(RelationTree::TreeNode* node,set<string>&tuple)
{
    if(node->children.empty())return;

    string tmp="";
//    for(size_t i(0);i<node->children.size();++i)
//    {
//        tmp += boost::lexical_cast<string>(node->children[i]->Begin)+" ";
//        Convert2TupleNonBin(node->children[i],tuple);
//    }
    for(size_t i(0);i<node->children.size();++i)
    {
        Convert2TupleNonBin(node->children[i],tuple);
    }
    tmp = boost::lexical_cast<string>(node->children.front()->Begin)+" "+boost::lexical_cast<string>(node->children.back()->Back);

    if(node->value==""&&node->level==0)return;
    //计算原始结构的时候加上
    //if(node->value.find("UKN")!=string::npos || node->value.find(":")!=string::npos )return;
    tuple.insert(tmp);
}
void SynTreeSQ::Convert2TupleCompleteNonBin(RelationTree::TreeNode* node,set<string>&tuple)
{
    if(node->children.empty())return;

    string tmp="";
    if(node->value.compare("")>0)
    {   if(node->value.find(":")!=string::npos)node->value="UKN";
        tmp = node->value+" ";
    }
//    for(size_t i(0);i<node->children.size();++i)
//    {
//        tmp += boost::lexical_cast<string>(node->children[i]->Begin)+" ";
//        Convert2TupleCompleteNonBin(node->children[i],tuple);
//    }
//    tmp += boost::lexical_cast<string>(node->children.back()->Back);
    for(size_t i(0);i<node->children.size();++i)
    {
        Convert2TupleCompleteNonBin(node->children[i],tuple);
    }
    tmp += boost::lexical_cast<string>(node->children.front()->Begin)+" "+boost::lexical_cast<string>(node->children.back()->Back);
    if(node->value==""&&node->level==0)return;
    //计算原始结构的时候加上
    //if(node->value.find("UKN")!=string::npos || node->value.find(":")!=string::npos )return;
    tuple.insert(tmp);
}
