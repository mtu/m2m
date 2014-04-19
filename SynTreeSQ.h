#ifndef SYNTREESQ_H_INCLUDED
#define SYNTREESQ_H_INCLUDED

#include "Common.h"

using namespace std;
namespace RelationTree{
struct TreeNode
{
  string value; //篇章关系，不是真正的value
  TreeNode *parent;
  int id;
  int level;
  int LeftOrRight; //0:left, 1:right -1:s
  int Begin;
  int Back;
  TreeNode *BinSib;
  vector<TreeNode*> children;
  TreeNode(const string&m_v, TreeNode* m_parent):value(m_v),parent(m_parent),id(0),LeftOrRight(-1),level(0){};
};
int LeftBranket(const string& s, string & str_flw_lb);
}
typedef RelationTree::TreeNode RTreeNode;
class SynTreeSQ
{

public:
    RelationTree::TreeNode root;
    SynTreeSQ():root("",NULL){};
    int ParseTree(const string& s, int mode=-1, int format=0);
    int ClearTree();
    void PrintTree(RelationTree::TreeNode *node);
    void PrintTreeOri(RelationTree::TreeNode *node);
    RelationTree::TreeNode* GetNodeWithId(int id);
    void GetStrVec(int begin, int back,vector<string>&result);
    int GetSize(){return nodeIdMap.size();}
    string GetSpanWithNode(RelationTree::TreeNode*);
    void PrintRelTree(RelationTree::TreeNode *node,int mode);
    void PrintRelTree_1(RelationTree::TreeNode *node,int mode);
    void PrintSegTree(RelationTree::TreeNode *node,int mode);
    float EvaluationForStr(set<string> &canSet,set<string>&refSet);
    void Convert2Tuple(RelationTree::TreeNode* node,set<string>&tuple);
    void Convert2TupleComplete(RelationTree::TreeNode* node,set<string>&tuple);
    void Convert2TupleNonBin(RelationTree::TreeNode* node,set<string>&tuple);
    void Convert2TupleCompleteNonBin(RelationTree::TreeNode* node,set<string>&tuple);

    void Binarization(RelationTree::TreeNode *);
    void Bin2NonBin(RelationTree::TreeNode *node);


    void test();

public:
    std::vector<std::string> sentenceVec;
    RelationTree::TreeNode* CompleRoot;
    map<int, RelationTree::TreeNode*> nodeIdMap;


private:
    int formatMode; //0: old, 1:new format


};

vector<RelationTree::TreeNode *> GetAllNode(RelationTree::TreeNode *root);
vector<RelationTree::TreeNode *> GetAllEdus(RelationTree::TreeNode *root);
void GetWord2EduMap(vector<RelationTree::TreeNode *> &allEdus, map<int,int> &word2Edu);
void InitKeySet();
#endif // SYNTREESQ_H_INCLUDED
