#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
													/*---*****   Contents needs to be in a folder named "files" near the erkam_karaca.c ******---*/
int size = 2;
struct Node{				// Defining node structure.
	
	char fileName [150];
	int degree;
	int count;		 		// Means key.
	struct Node* child;
	struct Node* parent;
	struct Node* sibling;
		       
} node,nodeArray[400];     // Defining array for input files.

struct Heap{				// Defining heap structure.
	
	struct Node* head;

} heap;

struct Node* createNode(int count, char fileName[]){ 	// To initialize a node.
	
	struct Node* node = malloc(sizeof(struct Node));
	node->count = count;
	strcpy(node->fileName,fileName);
	node->degree = 0;
	return node;
	
}

struct Heap* createHeap(){													// To create a heap structure.
	struct Heap* heap =(struct Heap*)malloc(sizeof(struct Heap));
	heap->head = NULL;
	return heap;
}

struct Node* unionHeap( struct Heap* original, struct Heap* uni){					// This function makes Union operation.
	
	if( original->head == NULL ) return uni->head;									// If one of the heap is null then function returns other one.
	if( uni->head == NULL ) return original->head;
	
	struct Node* head;
	struct Node* tHeap1 = original->head;					
	struct Node* tHeap2 = uni->head;
	struct Node* tail;
	
	if( original->head->degree <= uni->head->degree )
	{
		head = original->head;
		tHeap1 = tHeap1->sibling;
		
	}
	else
	{
		head = uni->head;
		tHeap2 = tHeap2->sibling;
	}	
	
	tail = head;
	
	while( tHeap1 != NULL && tHeap2 != NULL )												// This part is arranging the roots with respect to their degrees.
	{
		if( tHeap1->degree <= tHeap2->degree )
		{
			tail->sibling = tHeap1;
			tHeap1 = tHeap1->sibling;
		}
		else
		{
			tail->sibling = tHeap2;
			tHeap2 = tHeap2->sibling;
		}

		tail = tail->sibling;
	}

	if( tHeap1 != NULL ){
		tail->sibling = tHeap1; 	
	}
	else{
		tail->sibling = tHeap2;
	}
	

	struct Node* newHead = head ;

	original->head = NULL;
	uni->head = NULL;
	
	if( newHead == NULL ) return NULL;
	
	struct Node* pre = NULL;
	struct Node* curr = newHead;
	struct Node* next = newHead->sibling;
	
	while( next != NULL ){																											 
		if( curr->degree != next->degree || ( next->sibling != NULL && next->sibling->degree == curr->degree ) ){		
			pre = curr;
			curr = next;
		}
		else{
			if( curr->count > next->count )													// This part is creating max heap structures with respect to their counts (frequency).
			{
				curr->sibling = next->sibling;
				next->parent = curr;
				next->sibling = curr->child;
				curr->child = next;
				curr->degree++;
			}
			else{
				if( pre == NULL )
					newHead = next;
				else
					pre->sibling = next;

				curr->parent = next;
				curr->sibling = next->child;
				next->child = curr;
				next->degree++;

				curr = next;
			}
		}

		next = curr->sibling;
	}

	return newHead;
}

void insertHeap( struct Heap* heap, int count, char fileName[] ){
	struct Node* node = createNode( count , fileName);								// This part is for inserting nodes to heap.
	struct Heap* temp = createHeap();
	temp->head = node;
	heap->head = unionHeap( heap, temp );
	free( temp );
}

struct Node* extractMax( struct Heap* heap ){
	
	if( heap->head == NULL ) return NULL;									//This function finds the max of roots and remove it then re union all other childs and siblings.

	struct Node* max = heap->head;
	struct Node* premax = NULL;
	struct Node* next = max->sibling;
	struct Node* next_prev = max;

	while( next != NULL ){
		if( next->count > max->count )
		{
			max = next;
			premax = next_prev;
		}

		next_prev = next;
		next = next->sibling;
	}
	
	
	if( max == heap->head ){
		heap->head = max->sibling;
	}
		
	else{
		premax->sibling = max->sibling;
	}
		

	struct Node* new_head = NULL;
	struct Node* child = max->child;

	while( child != NULL ){
		
		struct Node *next = child->sibling;
		child->sibling = new_head;
		child->parent = NULL;	
		new_head = child;
		child = next;
	}

	struct Heap* temp = createHeap();
	temp->head = new_head;
	heap->head = unionHeap( heap, temp );
	free( temp );
	
	
	
	return max;
}


char *toLower(char* tempWord){																	//lowercase func. to compare strings correctly.
	int i;
  
    for (i = 0; tempWord[i]!='\0'; i++) {
      if(tempWord[i] >= 'A' && tempWord[i] <= 'Z') {
         tempWord[i] = tempWord[i] + 32;
      }
   }
	return tempWord;
}

void setFrequencies(char word[],struct Node nodeArray[]){
	int i = 2;

	while(strcmp(nodeArray[size].fileName,"\0") != 0){
		size += 1;
	}
	

	while( i < size ){
		FILE *fp; 											// File pointer.
   		char buff[255];
		
		int frequency = 0;
		
		char directoryName[150];
		strcpy(directoryName,"files/");							// Contents must be in a folder named "files" 
		strcat(directoryName,nodeArray[i].fileName);
		fp = fopen(directoryName, "r");
		
		int wordSize = 0;
		while(word[wordSize] != '\0'){
       		wordSize += 1;       	
   	    }
		 
		char word_dot[250] ;									// This part is combining the given word with dot,colon,quotation etc. to get  correct results.
		strcpy(word_dot,word);
		word_dot[wordSize] = '.';
		
		
		char word_exc[250] ;
		strcpy(word_exc,word);
		word_exc[wordSize] = '!';
		
		
		char word_quo[250] ;
		strcpy(word_quo,word);
		word_quo[wordSize] = '\'';
		
	
		char word_col[250] ;
		strcpy(word_col,word);
		word_col[size] = ',';
		
		char word_twoDot[250] ;
		strcpy(word_twoDot,word);
		word_twoDot[size] = ':';
		
		 
		while(fscanf(fp,"%s",buff) != EOF){									// This part is setting frequencies.
			if(strcmp(toLower(word),toLower(buff)) == 0 
			|| strcmp(buff,toLower(word_dot)) == 0 
			|| strcmp(buff,toLower(word_col)) == 0 
			|| strcmp(buff,toLower(word_exc)) == 0
			|| strcmp(buff,toLower(word_twoDot)) == 0
			|| strcmp(buff,toLower(word_quo)) == 0){      					
				frequency += 1;
			}
		}
		
		nodeArray[i].count = frequency;
		
	
		i += 1;
	}
	
	
	
}



void readFiles(){

	DIR *folder;										// This part is reaching to directory that has a folder named files.
    struct dirent *entry;								// Contents must be in a folder named "files" near the c file to reach contents corretly. 
    int files = 0;

    folder = opendir("files");
    if(folder == NULL)
    {
    	
        perror("Unable to read directory");
        
    }
	
	int i = 0;
    while( (entry=readdir(folder)))
    {
		struct Node newNode;
		strcpy(newNode.fileName, entry->d_name);           // Copying names to nodes.		
		newNode.count = 0;								   // Assigning frequencies
		nodeArray[i] = newNode;							   // Assigning files to array.
	 		
		i += 1;
    
    	
    }

    closedir(folder);
}

void NSC_traversal(struct Node* node){								//This function is for debug. Node-Sibling-Child traversal.
	
	if(node == NULL){
		return;
	}
	printf("%s = %d times - Degree = %d\n",node->fileName,node->count,node->degree);
	
	NSC_traversal(node->sibling);
	NSC_traversal(node->child);
}


int main()
{
	char word[250];
    
	readFiles();
	
	
	printf("Please enter a word to search = ");                                  
	scanf("%s",&word);												// Ask a word to user and taking input.
	
	setFrequencies(word,nodeArray);									// Setting frequencies of documents with respect to the input.
	

	int i = 2;
	struct Heap* heap = createHeap();							// Creating the Heap structure.
	
	i = 2;
	while(i < 44){
		insertHeap(heap,nodeArray[i].count,nodeArray[i].fileName);			// Inserting the nodes to the heap structure.
		i += 1;
	}
	

    printf("------------------------------------------\n");
    
    struct Node* lastArray[5];
    int p = 0;
    while(p < 5){																				// This part is extracting the most relevant documents from Binomial Heap structure
    	struct Node* extract = extractMax(heap);													// and adding their pointers to a list to print their contents.
    	printf("Extracted -> %s -> Count = %d\n",extract->fileName,extract->count);
    	lastArray[p] = extract;
    	p++;
	}
	printf("------------------------------------------\n");
	
	printf("The relevance order is : ");

	p = 0;
    while(p < 5){
    	if(lastArray[p]->count != 0 ){														// This part is printing the relevance order of documents.
    		if(p == 4){
    			printf("%s (%d).\n",lastArray[p]->fileName,lastArray[p]->count);
    			break;
			}
    		printf("%s (%d) ",lastArray[p]->fileName,lastArray[p]->count);
			if(lastArray[p+1]->count == 0){
				printf(".");
			}
			else{
				printf(", ");
			}
		}
		p++;
	}
	
	printf("\n\n\n");
	
	p = 0;
	while(p < 5){
		if(lastArray[p]->count == 0){
			printf("There is no relevant document.\n");
			p++;
			continue;
		}
		printf("%s (%d):   ",lastArray[p]->fileName,lastArray[p]->count);
		
		FILE *fp;
	    char buff[255];
	   
	    char directoryName[150];
	    strcpy(directoryName,"files/");
	    strcat(directoryName,lastArray[p]->fileName);
	    fp = fopen(directoryName, "r");
	   
	    while(fscanf(fp,"%s",buff) != EOF){							// This part is printing the relevant documents content.
	    	printf("%s ", buff);
	    }
	    printf("\n\n\n");
	 	
		p++;
	}
	
	
   
   
    return(0);
}
