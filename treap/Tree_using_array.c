#include<stdio.h>
#include<stdlib.h>

typedef struct BST{
	int left,data,right;
}BST;

/*We have created memory of given size using malloc once*/
BST* createBSTArray(int n){	
	BST *bst = (BST*)malloc(sizeof(BST)*n);
	if(bst==NULL){
		return NULL;
	}
	
	for(int i=0;i<n;i++){
		bst[i].left  = -1;
		bst[i].data  = -1;
		bst[i].right =  i+1;
	}
	bst[n-1].right =  -1;
	return bst;
}

void showBSTArray(BST *bst,int n){
	printf("\nBST Array\n");
	for(int i=0;i<n;i++){
		printf("%2d %4d %5d\n",bst[i].left,bst[i].data,bst[i].right);	
	}
	printf("\n");
}

int insert(BST *bst,int* root,int *top,int data){
	if(*top==-1){
		printf("\nBST Array is full\n");
		return -1;
	}
	int t = *top;
	*top = bst[*top].right;
	
	bst[t].data = data;
	bst[t].left = bst[t].right = -1;
		
	if(*root==-1){
		*root = t;
	}else{
		
		int temp = *root;
		int prev = *root;
		while(temp!=-1){
			prev = temp;
			if(bst[temp].data > data){
				temp = bst[temp].left;
			}else{
				temp = bst[temp].right;
			}		
		}
		if(bst[prev].data > data){
			bst[prev].left = t;
		}else{
			bst[prev].right = t;
		}
	}
	return 0;
}

void inorder(BST *bst,int root){
	if(root!=-1){
		printf("%d ",bst[root].data);
		inorder(bst,bst[root].left);
		inorder(bst,bst[root].right);
	}
}

void delete(BST *bst,int *root,int *top,int data){
	if(*root==-1){
		printf("\nTree is Empty..\n");
		return;
	}
	
	int temp = *root;
	int prev = -1;
	while(temp!=-1&&bst[temp].data!=data){
		prev = temp;
		if(bst[temp].data > data){
			temp = bst[temp].left;
		}else{
			temp = bst[temp].right;
		}		
	}
	if(temp==-1){
		printf("\nElement in not present..\n");
		return;
	}else{
	
		int tprev = -1;
		int ttemp = bst[temp].right;
		
		while(ttemp!=-1 && bst[ttemp].left!=-1){
			tprev = ttemp;
			ttemp = bst[ttemp].left;
		}
		
		if(ttemp!=-1){
 			if(tprev!=-1){
 				bst[tprev].left = bst[ttemp].right;
 				bst[ttemp].right = bst[temp].right;
 			}	
 			bst[ttemp].left = bst[temp].left;
 			
 			if(prev!=-1){
				if(bst[prev].data>data){
				 	bst[prev].left = ttemp;
				}else{
				 	bst[prev].right = ttemp;
				}
			}else{	
				*root = ttemp;
			}	
 					
		}else{
			if(prev!=-1){
				if(bst[prev].data>data){
				 	bst[prev].left = bst[temp].left;
				}else{
				 	bst[prev].right = bst[temp].left;
				}
			}else{	
				*root = bst[temp].left;
			}
		}						
		bst[temp].left = -1;
		bst[temp].data = 0;
		bst[temp].right = *top;
		*top = temp;
	}
}

int main(){	
	
	int n = 0;
	printf("\nEnter No of Elements:\n");
	scanf("%d",&n);
	
	BST *bst = createBSTArray(n);
	if(bst==NULL){
		printf("\nmain():Can't create Bst Array\n");
		return -1;
	}
	int top = 0;
	int root = -1;
	int status = -1;
	
	/*Initial Configuration*/
	showBSTArray(bst,n);
	printf("\nTop:%d ROOT : %d \n",top,root);

	//case1 :
	insert(bst,&root,&top,5);
	insert(bst,&root,&top,15);
	insert(bst,&root,&top,3);
	insert(bst,&root,&top,1);
	insert(bst,&root,&top,23);
	insert(bst,&root,&top,7);
	
	/*Configuration, After inserting some elements into BST Array*/
	showBSTArray(bst,n);
	printf("\nTop:%d ROOT : %d \n",top,root);
	printf("\nTree:\n");
	inorder(bst,root);
	printf("\n");
	
	delete(bst,&root,&top,7);
	
	/*
	showBSTArray(bst,n);
	printf("\nTop:%d ROOT : %d \n",top,root);
	
	printf("\nTree:\n");
	inorder(bst,root);
	printf("\n");
	*/

	delete(bst,&root,&top,5);
	
	showBSTArray(bst,n);
	printf("\nTop:%d ROOT : %d \n",top,root);
	printf("\nTree:\n");
	inorder(bst,root);
	printf("\n");
	
	delete(bst,&root,&top,15);
	
	/*
	showBSTArray(bst,n);
	printf("\nTop:%d ROOT : %d \n",top,root);
	printf("\nTree:\n");
	inorder(bst,root);
	printf("\n");
	*/

	delete(bst,&root,&top,145);
	
	delete(bst,&root,&top,1);
	
	delete(bst,&root,&top,3);
	
	delete(bst,&root,&top,23);
	
	delete(bst,&root,&top,23);
	
	showBSTArray(bst,n);	
	printf("\nTop:%d ROOT : %d \n",top,root);
	printf("\nTree:\n");
	inorder(bst,root);
	printf("\n");
	
	return 0;
}




