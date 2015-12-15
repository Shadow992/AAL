// https://de.wikipedia.org/wiki/Rot-Schwarz-Baum

class RecyclingRBTree
{
	private:  
        struct node
		{
		  int key_value;
		  node *left;
		  node *right;
		};
	
    public:
        RecyclingRBTree();
        ~RecyclingRBTree();

        void insert(long long key);
        node *search(long long key);
        void clear();

    private:
        void destroy_tree(node *leaf);
        void insert(int key, node *leaf);
        node *search(int key, node *leaf);
        
        node *root;
};

RecyclingBinaryTree::RecyclingBinaryTree()
{
  root=NULL;
}
