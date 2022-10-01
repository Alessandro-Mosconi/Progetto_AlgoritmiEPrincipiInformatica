#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#define inf INT_MAX

typedef struct {
	short int indice;
	int somma_cammini;
}grafo;

typedef struct nodo{
	int dist, info;
	struct nodo **succ;
}nodo;

typedef struct minheap{
	int heapsize;
	struct nodo *lista;
}minheap;

int comparator(const void * a, const void * b)
{
  grafo *grafo1 = (grafo *)a;
  grafo *grafo2 = (grafo *)b;
  return ( grafo1->somma_cammini - grafo2->somma_cammini );
}

int getintero();

int dijkstra(int **MAT, int n);

char *getstring(char *str);

int main() 
{
    int index=0, i, j;
    int num_nodi, num_classifica;
    int **A;
    char comando[15];
    grafo graph;
    grafo *classifica;
	
	num_nodi=getintero();
	num_classifica=getintero();
	
	classifica = malloc(num_classifica * sizeof(grafo));

	A= malloc(num_nodi * sizeof(int*));
	for(i=0; i<num_nodi; i++)
		A[i]= malloc(num_nodi * sizeof(int));
	
    do
    {
        getstring(comando);			
    
        if(comando[0]=='A')
        {
			graph.indice=index;
            for(i=0; i<num_nodi; i++)
			for(j=0; j<num_nodi; j++)
	        	A[i][j]=getintero();															//acquisisco la matrice di adiacenza in A
			
            graph.somma_cammini=dijkstra(A, num_nodi);											//chiamata della funzione di dijkstra che ritorna la somma dei cammini minimi
            
            if(index<num_classifica)
	            classifica[index]=graph;														//inizio inserimento classifica + riordinamento con quicksort
            else
			{
				if(graph.somma_cammini < classifica[num_classifica-1].somma_cammini)
					{
						qsort(classifica, num_classifica, sizeof(grafo), comparator);			//chiamata a quick sort
						classifica[num_classifica-1]=graph;
					}
			 } 
			 
            index++;
        }
    }while(comando[0]!='T');


	for(i=0; i<num_classifica; i++)																//stampo la classifica
		printf("%d ", classifica[i].indice);
		
	//for(i=0; i<num_nodi; i++)
	//	free(A[i]);
	free(A);
	free(classifica);

	return 0;
}

int getintero()
{
	char carattere;
	int cifra, i=0;
	int num=0;
	
	carattere=getchar();
		
	while(carattere!=',' && carattere!='\n')													//finisco di acquisire un intero quando leggo , o \n
	{
		if(carattere!=' ')																		//ignoro gli spazi
		{
			cifra=carattere-'0';
			if(i==0)
				{
					num = cifra;
					i++;
				}
			else
				num = cifra + (10 * num);
		}
		carattere=getchar();
	}
	
	return num;
}

char *getstring(char *str)
{
	char carattere;
	int i=0;
			
	carattere=getchar();

  	while(carattere!='\n' && i<15)
  { 
     str[i]=carattere;
     i++;
     carattere=getchar();
  }
  
  return str;
}

void swap(nodo *a, nodo *b)
{
nodo tmp = *a;
*a = *b;
*b = tmp;
}

void minheapify (minheap *A, int n)
{
	int l, r, posmax;
	l= 2*n ;
	r=(2*n) +1;
	
	if(l<=(*A).heapsize && (*A).lista[l].dist < (*A).lista[n].dist)
		posmax=l;
	else posmax=n;
	
	if(r<=(*A).heapsize && (*A).lista[r].dist < (*A).lista[posmax].dist)
		posmax=r;
	
	if( posmax!=n)
	{
		swap (&(*A).lista[n], &(*A).lista[posmax]);
		minheapify(A, posmax);
	}
	
}

void costruisciminheap (minheap *A)
{
	int i;
	for(i = ((*A).heapsize) /2; i>=0; i--)
		minheapify(A, i);
}

void decrementapri(minheap *A, nodo v, int key, int dim)
{
	int i;
	
	for(i=0; i < dim; i++)
	{
		if((*A).lista[i].info==v.info)
			(*A).lista[i].dist=key;			
	}
	costruisciminheap(A);
}

nodo cancellamin(minheap *A)
{
	nodo min;
	
	if ((*A).heapsize<1)
		return min;
		
	min = (*A).lista[0];
	(*A).lista[0]=(*A).lista[(*A).heapsize-1];
	(*A).heapsize=(*A).heapsize-1;
	minheapify(A, 0);
	(*A).lista[(*A).heapsize]=min;
	return min;
}

void inserisci(minheap *A, nodo key)
{
	int i;
	
	(*A).heapsize = (*A).heapsize +1;
	(*A).lista[(*A).heapsize-1] = key;
	i=(*A).heapsize;
	
	while (i>1 && (*A).lista[(i)/2].dist > (*A).lista[i].dist)
	{
		swap(&(*A).lista[(i)/2], &(*A).lista[i]);
		i= (i)/2;
	}
}


int dijkstra(int **MAT, int n)
{
	int i, j, ndis, tot=0;
	nodo u;
	
	minheap Q;
	
	Q.lista= malloc (n*sizeof(struct nodo));
	u.succ = malloc(n*sizeof(struct nodo*));
		
	Q.heapsize=n;
	
	Q.lista[0].dist=0;
	
	for(i=0; i<n; i++)																		//Riempio Q.lista
	{
		Q.lista[i].info=i;
		Q.lista[i].succ = malloc (n*sizeof(struct nodo));
		for(j=0; j<n; j++)
			Q.lista[i].succ[j]=&Q.lista[j];
		
		if(i!=0)
			Q.lista[i].dist=inf;
	}	
	
	while (Q.heapsize!=0)																	//calcolo i cammini minimi
	{
		
		u = cancellamin(&Q);
		
		for(i=0; i<n; i++)
		{			
			ndis= u.dist + MAT[u.info][(*u.succ[i]).info];
			
				if ( u.info!=(*u.succ[i]).info && MAT[u.info][(*u.succ[i]).info]!=0 )
				{
					(*u.succ[i]).dist=ndis;
					decrementapri(&Q, (*u.succ[i]), ndis, n);
				}
		}
	}
		
	Q.heapsize=n;
		
	for(i=0; i<Q.heapsize; i++)																//sommo i cammini minimi
		tot=tot+Q.lista[i].dist;
		
	
	//for(i=0; i<n; i++)
	//	free(Q.lista[i].succ);	
	free(Q.lista);
	free(u.succ);
	
	return tot;
			
}
