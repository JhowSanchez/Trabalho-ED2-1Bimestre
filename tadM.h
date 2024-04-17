#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//matEsp
struct listaM
{
	char lin,col,valor[50];
	struct listaM *pc, *pl;
};
typedef struct listaM listaM;

struct lec
{
	char indc;
	struct lec *prox;
	struct listaM *lista;
};
typedef struct lec lecM;

struct descritor
{
	struct lec *pLinha, *pColuna;	
};
typedef struct descritor descM;


//listaGen
union noLista{
	float valor;
	char op;
	char func[50];
};
typedef union noLista noLista;

struct listaGen
{
	struct listaGen *cab,*cauda;
	noLista no;
	char terminal;
};
typedef struct listaGen listaGen;

void resolverReta(listaGen **l,float *result);
float resolverExpressao(listaGen **lista);
float resolverLista(descM desc,char str[]);

char Nula(listaGen *L)
{
	return L==NULL;
}

void Cons(listaGen **nc,noLista elem, char term,listaGen *cab, listaGen *cauda)
{
	*nc = (listaGen*)malloc(sizeof(listaGen));
	
	(*nc) -> cab = cab;
	(*nc) -> cauda = cauda;
	(*nc) -> terminal = term;
	(*nc) -> no = elem;
}

// MatrizEsp

void init(descM *desc)
{
	(*desc).pLinha = NULL;
	(*desc).pColuna = NULL;
}

void inserirLec(lecM **lista, char ind,lecM **aux,lecM **ant)
{
	lecM *novo;
	
	*ant = NULL;
	(*aux) = (*lista);
	
	while( *aux != NULL && (*aux) -> indc < ind)
	{
		*ant = *aux; 
		*aux = (*aux) -> prox;	
	}
	
	if(*aux == NULL || (*aux) -> indc != ind)
	{
		novo = (lecM*)malloc(sizeof(lecM));
		novo -> lista = NULL;
		novo -> indc = ind;
		novo -> prox = *aux;
		
		if(*ant == NULL)
			*lista = novo;
		else
			(*ant) -> prox = novo;
			
		*aux = novo;
	}
}

void verificaOcupado(lecM *lista,char lin, char col, listaM **aux)
{
	listaM *auxM;
	lecM *auxD;
	*aux = NULL;
	
	auxD = lista;
	while(auxD != NULL && auxD -> indc < lin)
	{
		auxD = auxD -> prox;
	}
	
	if(auxD != NULL && auxD -> indc == lin)
	{
		auxM = auxD -> lista;
		while(auxM != NULL && auxM -> col < col)
			auxM = auxM -> pc;
			
		if(auxM != NULL && auxM -> col == col)
		{
			*aux = auxM;
		}
	}
}

void inserirM(descM *desc,char lin, char col, char valor[50])
{
	lecM *auxL, *auxC,*antCol, *antLin;
	listaM *aux,*nova,*antL,*antC,*atualL,*atualC;
	
	verificaOcupado((*desc).pLinha,lin,col,&aux);
	
	if(aux != NULL)
	{
		strcpy(aux-> valor,valor);
	}
	else
	{
		inserirLec(&(*desc).pLinha,lin,&auxL,&antLin);
		inserirLec(&(*desc).pColuna,col,&auxC,&antCol);
		
		nova = (listaM*) malloc(sizeof(listaM));
		nova -> lin = lin;
		nova -> col = col;
		nova -> pc = NULL;
		nova -> pl = NULL;
		strcpy(nova-> valor,valor);
		
		if(auxL -> lista == NULL)
		{
			auxL -> lista = nova;
		}
		else if(auxL -> lista -> col > col)
		{
			nova -> pc = auxL -> lista;
			auxL -> lista = nova;
		}
		else{
			antL = auxL -> lista;
			atualL = antL -> pc;
			
			while(atualL != NULL && atualL -> col < col)
			{
				antL = atualL;
				atualL = atualL -> pc;
			}
		
			nova -> pc = atualL;
			antL -> pc = nova;
		}
		
		if(auxC -> lista == NULL)
		{
			auxC -> lista = nova;
		}
		else if(auxC -> lista -> lin > lin)
		{
			nova -> pl = auxC -> lista;
			auxC -> lista = nova;
		}
		else{
			antC = auxC -> lista;
			atualC = antC -> pl;
			
			while(atualC != NULL && atualC -> lin < lin)
			{
				antC = atualC;
				atualC = atualC -> pl;
			}
		
			nova -> pl = atualC;
			antC -> pl = nova;
		}		
	}
}

void excluir(descM *desc,char lin, char col)
{
	lecM *auxL,*antL,*auxC,*antC;
	listaM *antCo,*auxCo,*antLi,*auxLi;
	
	inserirLec(&(*desc).pLinha,lin,&auxL,&antL);
	inserirLec(&(*desc).pColuna,col,&auxC,&antC);
	
	antCo = NULL;
	auxCo = auxC -> lista;
	while(auxCo -> lin < lin)
	{
		antCo = auxCo;
		auxCo = auxCo -> pl;
	}

	antLi = NULL; 
	auxLi = auxL -> lista;
	while(auxLi -> col < col)
	{
		antLi = auxLi;
		auxLi = auxLi -> pc;
	}
	

	if(antLi == NULL)
	{
		auxL -> lista = auxLi -> pc;
	}
	else
	{
		antLi -> pc = auxLi -> pc;
	}

	if(antCo == NULL)
	{
		auxC -> lista = auxCo -> pl;
	}
	else
	{
		antCo -> pl = auxCo -> pl;
	}	
	free(auxLi);
	
		
	if(auxL -> lista == NULL)
	{
		if(antL == NULL)
		{
			(*desc).pLinha = auxL -> prox;
		}
		else
		{
			antL -> prox = auxL -> prox;
		}
		free(auxL);
	}
	
	if(auxC -> lista == NULL)
	{
		if(antC == NULL)
		{
			(*desc).pColuna = auxC -> prox;
		}
		else
		{
			antC -> prox = auxC -> prox;
		}
		free(auxC);
	}
}

void limparCaixas(listaM *aux)
{
	if(aux -> pc != NULL)
		limparCaixas(aux -> pc);
	free(aux);
}

void limparMatriz(descM *desc)
{
	lecM *auxL = (*desc).pLinha;
	lecM *auxC;
	listaM *aux;
	
	while(auxL != NULL)
	{
		auxC = auxL;
		limparCaixas(auxL -> lista);
		auxL = auxL -> prox;
		free(auxC);
	}
	auxC = (*desc).pColuna;
	while(auxC != NULL)
	{
		auxL = auxC;
		auxC = auxC -> prox;
		free(auxL);
	}
	
	(*desc).pLinha = NULL;
	(*desc).pColuna = NULL;
}

void buscaNaMatriz(descM desc,lecM *L, char col,char lin, char col2,char lin2,char op,float *result)
{
	char maiorCol,menorCol,maiorLin,menorLin, flagMenor = 1, flagMaior = 1;
	int cont=0;
	float maior =0, menor = 0,soma,aux;
	lecM *auxCol;
	listaM *auxL, *auxC;
	
	if(col < col2)//A < B
	{
		maiorCol = col2;
		menorCol = col;	
	}
	else
	{
		maiorCol = col;
		menorCol = col2;
	}
	
	if(lin < lin2)//A 1 < 2 B
	{	
		menorLin = lin;
		maiorLin = lin2;
	}
	else
	{
		menorLin = lin2;
		maiorLin = lin;
	}	
		
	auxCol = L;
	while(auxCol != NULL && auxCol -> indc <= maiorCol)
	{
		if(auxCol -> indc >= menorCol)
		{
			auxL = auxCol -> lista;
			while(auxL != NULL && auxL -> lin <= maiorLin)
			{
				if(auxL -> lin >= menorLin)
				{
					if(auxL -> valor[0] == '=')
					{
						aux = resolverLista(desc,auxL -> valor);
						soma += aux;	
						cont++;
						if(flagMaior || aux > maior)
						{
							flagMaior = 0;
							maior = aux;
						}
							
						if(flagMenor || aux < menor)
						{
							flagMenor = 0;
							menor = aux;
						}
					}
					else
					{
						soma += atof(auxL -> valor);	
						cont++;
						if(flagMaior || atof(auxL -> valor) > maior)
						{
							flagMaior = 0;
							maior = atof(auxL -> valor);
						}	
							
						if(flagMenor || atof(auxL -> valor) < menor)
						{
							flagMenor = 0;
							menor = atof(auxL -> valor);
						}	
					}
				}
				auxL = auxL -> pl;
			}
		}
		auxCol = auxCol -> prox;
	}

	if(op == '<')
		*result = menor;
	else if(op == '>')
		*result = maior;
	else if(op == '/')
		*result = (soma/cont);
	else
		*result = soma;
}

char ehNumero(char str[50])
{
	int i=0;
	if(str[i] == '-')
		i++;
	while(str[i] >= '0' && str[i] <= '9')
		i++;
	if(str[i] == '.' && str[i+1] >= '0' && str[i+1] <= '9')
	{
		i++;
		while(str[i] >= '0' && str[i] <= '9')
			i++;
	}
	return i == strlen(str);
}

float retornaValorDaCelula(descM desc, char valor[50])
{
	if(valor[0] == '=')
	{
		return resolverLista(desc, valor);
	}
	if(ehNumero(valor))
	{
		return atof(valor);
	}
	return 0;
}

void criarLista(descM desc, listaGen **lista, char str[], int *i)
{
	char str2[50], lin, col, lMin, cMin;
	float result;
	listaM *aux = NULL;
	noLista elemento;
	listaGen *nova, *l, *zero;
	int j,k;
	while((*i) < strlen(str) && str[*i] != ')')
	{
		if(str[*i] >= '0' && str[*i] <= '9' || str[*i] == '.')
		{
			j=0;
			while(str[*i] >= '0' && str[*i] <= '9' || str[*i] == '.')
				str2[j++] = str[(*i)++];
			str2[j] = '\0';
			
			elemento.valor = atof(str2);
			Cons(&nova, elemento, 'v', NULL, NULL);
		}
		else if(str[*i] == '+' || str[*i] == '-' || str[*i] == '*' || str[*i] == '/')
		{
			if(str[*i] == '-' && ((*i) == 1 || str[(*i)-1] == '('))
			{
				elemento.valor = 0;
				Cons(&zero, elemento, 'v', NULL, NULL);
				elemento.op = '-';
				Cons(&nova, elemento, 'o', NULL, zero);
				(*i)++;
			}
			else
			{
				elemento.op = str[(*i)++];
				Cons(&nova, elemento, 'o', NULL, NULL);
			}
		}
		else if(str[*i] == '(')
		{
			(*i)++;
			elemento.valor = 0;
			Cons(&nova, elemento, 'l', NULL, NULL);
			criarLista(desc, &(*nova).cab, str, i);
			(*i)++;
		}
		else if(str[*i] >= 'A' && str[*i] <= 'Z')
		{
			if(str[(*i)+1] >= '0' && str[(*i)+1] <= '9')
			{
				col = str[(*i)++];
				
				j=0;
				while(str[*i] >= '0' && str[*i] <= '9')
					str2[j++] = str[(*i)++];
				str2[j] = '\0';
				
				lin = atoi(str2);
				verificaOcupado(desc.pLinha, lin, col, &aux);
				
				if(aux != NULL)
				{
					if(aux -> valor[0] == '=')
					{
						elemento.valor = resolverLista(desc, aux -> valor);
					}
					else
					{
						if(ehNumero(aux -> valor))
							elemento.valor = atof(aux -> valor);
						else
							elemento.valor = 0;
					}
				}
				else
					elemento.valor = 0;
					
				Cons(&nova, elemento, 'v', NULL, NULL);
			}
			else
			{
				k = (*i)+4;
				cMin = str[k++];
				
				j=0;
				while(str[k] >= '0' && str[k] <= '9')
					str2[j++] = str[k++];
				str2[j] = '\0';
				
				lMin = atoi(str2);
				
				k++;
				
				col = str[k++];
				
				j=0;
				while(str[k] >= '0' && str[k] <= '9')
					str2[j++] = str[k++];
				str2[j] = '\0';
				
				lin = atoi(str2);
				
				if(str[*i] == 'S' && str[(*i)+1] == 'U' && str[(*i)+2] == 'M')
				{
					buscaNaMatriz(desc,desc.pColuna,col, lin, cMin, lMin,'+',&result);
				}
				else if(str[*i] == 'A' && str[(*i)+1] == 'V' && str[(*i)+2] == 'G')
				{
					buscaNaMatriz(desc,desc.pColuna,col, lin, cMin, lMin,'/',&result);
				}
				else if(str[*i] == 'M' && str[(*i)+1] == 'I' && str[(*i)+2] == 'N')
				{
					buscaNaMatriz(desc,desc.pColuna,col, lin, cMin, lMin,'<',&result);
				}
				else if(str[*i] == 'M' && str[(*i)+1] == 'A' && str[(*i)+2] == 'X')
				{
					buscaNaMatriz(desc,desc.pColuna,col, lin, cMin, lMin,'>',&result);
				}
				*i = k+1;
				elemento.valor = result;
				Cons(&nova, elemento, 'v', NULL, NULL);
			}
		}
		
		if(*lista == NULL)
			(*lista) = nova;
		else
			l -> cauda = nova;
		
		if(nova -> cauda == NULL)
			l = nova;
		else
			l = nova -> cauda;
	}
}

void resolverReta(listaGen **l,float *result)
{
	listaGen *aux=*l,*ant=*l;
	float soma;
	
	
	while(aux -> cauda != NULL)
	{
		if(aux ->cauda -> terminal == 'o' && ( aux -> cauda -> no.op == '*' || aux -> cauda -> no.op == '/'))
		{
			if(aux -> cauda -> no.op == '*')
			{
				aux -> no.valor = aux -> no.valor * aux -> cauda -> cauda -> no.valor;
			}
			else
			{
				aux -> no.valor = aux -> no.valor / aux -> cauda -> cauda -> no.valor;
			}
			
			ant = aux -> cauda;
			aux -> cauda = aux -> cauda -> cauda -> cauda;
			free(ant -> cauda);
			free(ant);
		}
		else
			aux = aux -> cauda;
	}
	
	aux=*l;
	ant=*l;
	while(aux -> cauda != NULL)
	{
		if(aux -> cauda -> no.op == '+')
		{
			aux -> no.valor = aux -> no.valor + aux -> cauda -> cauda -> no.valor;
		}
		else
		{
			aux -> no.valor = aux -> no.valor - aux -> cauda -> cauda -> no.valor;
		}
		
		ant = aux -> cauda;
		aux -> cauda = aux -> cauda -> cauda -> cauda;
		free(ant -> cauda);
		free(ant);
	}
	*result = aux -> no.valor;
	free(aux);
	*l = NULL;
}

float resolverExpressao(listaGen **lista)
{
	listaGen *aux = *lista;
	float result = 0;
	while(aux != NULL)
	{
		if(aux -> cab != NULL)
		{
			aux -> no.valor = resolverExpressao(&aux->cab);
		}
		
		aux = aux -> cauda;
	}
	resolverReta(lista,&result);
	
	return result;
}

float resolverLista(descM desc,char str[])
{
	int i=1;
	listaGen *lista = NULL,*aux;
	criarLista(desc,&lista,str,&i);
	return resolverExpressao(&lista);
}


