#include <conio2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "tadM.h"

void infoEsq(int n, int info, char s_info[])
{
    int i, tam;
    itoa(info,s_info,10);
    tam = strlen(s_info);
    for (i=0; i<n-tam; i++)
        strcat(s_info," ");
}

void infoCenter(int info, char s_info[])
{
    char s[9];
    strcpy(s,"    ");
    sprintf(s_info,"%c",info);
    strcat(s,s_info);
    strcat(s,"    ");
    strcpy(s_info,s);
}

void desenhaTela(int lin, int col)
{
    int i,j;
    char slin[4],scol[9];
    textbackground(0);
    clrscr();
    textbackground(3);
    textcolor(0);
    //desenha a coluna esq 1..20
    for (i=4; i<=23; i++)
    {
        gotoxy(1,i);
        infoEsq(3,lin,slin);
        printf("%s",slin);
        lin=lin+1;
    }
    //desenha a linha sup A..H
    j=4;
    for (i=1; i<=8; i++)
    {
        gotoxy(j,3);
        infoCenter(col,scol);
        printf("%s",scol);
        j=j+9;
        col=col+1;
    }
}

void printConteudo(descM desc, char str[])
{
	//char *str2 = (char *)malloc(50 * sizeof(char));
	char str2[50];
	int i, j;
	strcpy(str2,str);
	
	if(ehNumero(str2))
	{
		for(i=0;i<9-strlen(str2);i++)
			printf(" ");
	}
	else if(str2[0] == '=')
		sprintf(str2,"%*.2f",9,resolverLista(desc, str2));
		
	if(strcmp(str2,"1.#J") == 0)
		strcpy(str2,"!ERRO");
		
	str2[9]='\0';
	printf("%s",str2);
	
}

void irCelula(int lin, int col)
{
	gotoxy(col*9-9+4,lin+3);
}

void exibir(descM desc,int col, int lin)
{
	listaM *aux = NULL;
	int i,j;
	for(i=lin;i<lin+20;i++)
	{
		for(j=col;j<col+8;j++)
		{
			irCelula(i-lin+1,j-col+1);
			
			verificaOcupado(desc.pLinha,i,j,&aux);

			if(aux != NULL)
		    {
		  		printConteudo(desc, aux -> valor);
		    }
		}
	}
}

void retirarEspacos(char str[])
{
	int i=0;
	int j;
	
	while(i < strlen(str) && str[i] == ' ')
		i++;
	if(str[i] == '=')
	{
		j=0;
		while(i < strlen(str))
		{
			if(str[i] != ' ')
				str[j++] = toupper(str[i]);
			i++;
		}
		str[j] = '\0';
	}
}

void arquivo(descM desc,char str[])
{
	lecM *auxL, *auxC;
	listaM *auxLin,*auxCol,reg;
	FILE *ptr = fopen(str,"wb");
	
	rewind(ptr);
	if(ptr != NULL)
	{
		auxL = desc.pLinha;
		while(auxL != NULL)
		{
			auxLin = auxL -> lista;
			while(auxLin != NULL)
			{
				reg.lin = auxLin -> lin;
				reg.col = auxLin -> col;
			
				strcpy(reg.valor,auxLin -> valor);
				
				fwrite(&reg,sizeof(reg),1,ptr);
				
				auxLin = auxLin -> pc;
			}
			auxL = auxL -> prox;
		}	
		fclose(ptr);	
	}
}

char lerArquivo(descM *desc,char str[50])
{
	FILE *ptr = fopen(str,"rb");
	listaM reg;
	if(ptr != NULL)
	{
		limparMatriz(desc);
		fread(&reg,sizeof(reg),1,ptr);
		while(!feof(ptr))
		{
			inserirM(&(*desc),reg.lin,reg.col,reg.valor);
			fread(&reg,sizeof(reg),1,ptr);
		}
		fclose(ptr);
		return 1;	
	}
	return 0;
}

void exibirGrafico(descM desc)
{
	char area[50];
	char aux[50];
	char vet[9][50];
	lecM *auxL;
	listaM *auxM;
	float maior, menor, passo;
	char col1, col2, lin1, lin2, a;
	int i=0, j, TL=0, k, m;
	gotoxy(1,1);
	textbackground(0);
	textcolor(15);
	printf("Gerar grafico: ");
	fflush(stdin);
	gets(area);
	if(strlen(area) > 0)
	{
		textbackground(0);
		textcolor(15);
		clrscr();
		
		
		strcpy(aux,"=");
		strcat(aux, area);
		retirarEspacos(aux);
		
		i++;
		col1 = aux[i++];
		j=0;
		while(aux[i] >= '0' && aux[i] <= '9')
			area[j++] = aux[i++];
		i++;
		area[j] = '\0';
		lin1 = atoi(area);
		
		col2 = aux[i++];
		j=0;
		while(aux[i] >= '0' && aux[i] <= '9')
			area[j++] = aux[i++];
		i++;
		area[j] = '\0';
		lin2 = atoi(area);
		
		if(lin1 > lin2)
		{
			a = lin1;
			lin1 = lin2;
			lin2 = a;
		}
		if(col1 > col2)
		{
			a = col1;
			col1 = col2;
			col2 = a;
		}
		
		
		//teste
		gotoxy(1,1);
		printf("  Grafico: %c%d:%c%d", col1, lin1, col2, lin2);
		
		sprintf(area, "=MAX(%c%d:%c%d)", col1+1, lin1+1, col2, lin2);
		maior = resolverLista(desc,area);
		
		sprintf(area, "=MIN(%c%d:%c%d)", col1+1, lin1+1, col2, lin2);
		menor = resolverLista(desc,area);
		
		if(maior == 0 && menor == 0)
			maior = 100;
		
		if(menor == maior)
		{
			if(menor >= 0)
				menor = 0;
			else
				menor = menor -100;
		}
		
		//printar a linha
		for(i=3;i<25+3;i++)
		{
			gotoxy(10,i);
			printf("|");
		}
		
		//printar os numeros laterais
//		i=5;
//		while(i>0)
//		{
//			gotoxy(1,(5-i)*5+3);
//			printf("%*.2f",9,maior/5*i);
//			i--;
//		}
		i=0;
		passo = (maior-menor) / 5;
		while(i < 6)
		{
			gotoxy(1,28-i*5);
			printf("%*.2f", 9, i*passo+menor);
			i++;
		}
		
		//printar nomes debaixo
		for(i=col1+1;i<=col2;i++)
		{
			verificaOcupado(desc.pLinha, lin1, i, &auxM);
			if(auxM != NULL)
			{
				gotoxy(11+(i-66)*10,28);
				printConteudo(desc, auxM -> valor);
			}
		}
		
		
		//printar a legenda
		j=0;
		for(i = lin1+1;i<=lin2;i++)
		{
			verificaOcupado(desc.pLinha, i, col1, &auxM);
			if(auxM != NULL)
			{
				gotoxy(10+11*j,30);
				j++;
				textbackground(TL+1);
				textcolor(0);
				printf(" ");
				textbackground(0);
				textcolor(15);
				printf(" ");
				printConteudo(desc, auxM -> valor);
				strcpy(vet[TL++], auxM -> valor);
			}
		}
		
		//printar as barras
		for(j=col1+1;j<=col2;j++)
		{
			k=0;
			for(i=lin1+1;i<=lin2;i++)
			{
				verificaOcupado(desc.pLinha, i, j, &auxM);
				if(auxM != NULL)
				{
					textbackground(k+1);
					m = (int)(28-(retornaValorDaCelula(desc, auxM->valor)-menor)/passo*5);
					while(m < 28)
					{
						gotoxy(11+((j-66)*10)+k,m);
						printf(" ");
						m++;
					}
				}
				k++;
			}
		}
		textbackground(0);
		
		getch();
	}
}

char naoCircular(descM desc, char str[50], char linha, char coluna)
{
	int j, i=1, k;
	char str2[50], lin, col, flag = 1,lMin, cMin, c;
	listaM *aux;
	while(i < strlen(str) && flag)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			if(str[i+1] >= '0' && str[i+1] <= '9')
			{
				col = str[i++];
				j=0;
				while(str[i] >= '0' && str[i] <= '9')
					str2[j++] = str[i++];
				str2[j] = '\0';
				
				lin = atoi(str2);
				
				if(lin != linha || col != coluna)
				{
					verificaOcupado(desc.pLinha, lin, col, &aux);
					if(aux != NULL && aux -> valor[0] == '=')
						flag = naoCircular(desc, aux -> valor, linha, coluna);
				}
				else
					flag = 0;
			}
			else
			{
				i += 4;
				cMin = str[i++];
				
				j=0;
				while(str[i] >= '0' && str[i] <= '9')
					str2[j++] = str[i++];
				str2[j] = '\0';
				
				lMin = atoi(str2);
				
				i++;
				
				col = str[i++];
				
				j=0;
				while(str[i] >= '0' && str[i] <= '9')
					str2[j++] = str[i++];
				str2[j] = '\0';
				
				lin = atoi(str2);
				
				if(col < cMin)
				{
					c = col;
					col = cMin;
					cMin = c;
				}
				
				if(lin < lMin)
				{	
					c = lin;
					lin = lMin;
					lMin = c;
				}
				
				for(;lMin <= lin && flag;lMin++)
				{
					for(;cMin <= col && flag; cMin++)
					{
						if(lMin != linha || cMin != coluna)
						{
							verificaOcupado(desc.pLinha, lMin, cMin, &aux);
							if(aux != NULL && aux -> valor[0] == '=')
								flag = naoCircular(desc, aux -> valor, linha, coluna);
						}
						else
							flag = 0;
					}
				}
			}
		}
		else
			i++;
	}
	return flag;
}

void iniciaExcel()
{
    char key, valor[50],nome[50];
    int c,l,col,lin,i;
	descM desc;
	listaM *aux = NULL;
	FILE *ptr = fopen("planilha.dat","ab+");
	init(&desc);
	
    textbackground(0);
    clrscr();
    desenhaTela(1,65);
    //coordenadas de tela
    c=l=1;
    //coordenadas da matriz
    col=65; //letra A
    lin=1;
    gotoxy(c*9-9+4,l+3);
    printf("         ");

    do
    {
        key=getch();
        if (key==-32)
        {
            key=getch();
            switch(key)
            {                       //linhas   linhas-1
                case 80:if (l==20 && lin<100 - 19) //seta p/ baixo
                            lin++;
                        if (l<20)
                            l++;
                        break;
                case 72:if (l==1 && lin>1) //seta p/ cima
                            lin--;
                        if (l>1)
                            l--;
                        break;
                                     //'A' alfabeto  colunas-1
                case 77:if (c==8 && col<65 + 26 - 8) //seta p/ direita
                            col++;
                        if (c<8)
                            c++;
                        break;
                case 75:if (c==1 && col>65) //seta p/ esquerda
                            col--;
                        if (c>1)
                            c--;
                        break;
            }
        }
        else if(key == 0)
        {
        	key = getch();
        	switch(key)
        	{
        		case 59:
        			gotoxy(1,1);
        			textcolor(15);
        			textbackground(0);
        			printf("Certeza que desejas abrir um novo arquivo? (S/N)");
        			if(toupper(getch()) == 'S')
        			{
        				limparMatriz(&desc);
        				//coordenadas de tela
					    c=l=1;
					    //coordenadas da matriz
					    col=65; //letra A
					    lin=1;
        			}
        			break;
        		case 60:
        			textbackground(0);
        			textcolor(15);
        			gotoxy(1,1);
        			printf("                                                  ");
        			gotoxy(1,1);
        			fflush(stdin);
        			gets(valor);
        			retirarEspacos(valor);
        			if(strcmp(valor,"") == 0)
        			{
        				verificaOcupado(desc.pLinha,lin+l-1,col+c-1,&aux);
						if(aux != NULL)
						{
							excluir(&desc,lin+l-1,col+c-1);
							aux=NULL;
						}
        			}
        			else if(valor[0] == '='&&!naoCircular(desc, valor, lin+l-1, col+c-1))
        				strcpy(valor,"ERRO!");
        			inserirM(&desc, lin+l-1, col+c-1,valor);
					break;
				case 61:
					textbackground(0);
        			textcolor(15);
        			gotoxy(1,1);
        			printf("Digite o nome do Arquivo a ser salvo: ");
					gotoxy(39,1);
        			fflush(stdin);
        			gets(valor);
        			if(strlen(valor)>0)
        			{
        				strcat(valor,".dat");
        				arquivo(desc,valor);
        			}
					break;
				case 62:
					textbackground(0);
        			textcolor(15);
        			gotoxy(1,1);
        			printf("Digite o nome do Arquivo a carregar: ");
					gotoxy(38,1);
        			fflush(stdin);
        			gets(valor);
        			if(strlen(valor)>0)
        			{
        				strcat(valor,".dat");
        				if(lerArquivo(&desc,valor))
        				{
	        				//coordenadas de tela
						    c=l=1;
						    //coordenadas da matriz
						    col=65; //letra A
						    lin=1;
						}
        			}
					break;	
				case 63:
					exibirGrafico(desc);
			}
        }
        else if(key == 8)
        {
        	verificaOcupado(desc.pLinha,lin+l-1,col+c-1,&aux);
			if(aux != NULL)
			{
				excluir(&desc,lin+l-1,col+c-1);
				aux=NULL;
			}
        }
        desenhaTela(lin,col);
        textbackground(0);
        textcolor(15);
        //<--------------------exibir todas as celulas na janela atual
        //                     de lin até lin+19, col até col+8
        exibir(desc,col,lin);
        gotoxy(c*9-9+4,l+3);
        textbackground(3);
        textcolor(0);
        printf("         "); //desenhar o conteudo da celula atual a partir da matriz esparsa
        verificaOcupado(desc.pLinha,lin+l-1,col+c-1,&aux);
	    textcolor(15);
	    textbackground(0);
	    gotoxy(1,1);
	    if(aux != NULL)
	    {
			printf("%s",aux -> valor);
	    	gotoxy(c*9-9+4,l+3);
	    	textbackground(3);
	    	textcolor(0);
	    	printConteudo(desc, aux->valor);
	    }
    }while (key!=27);
    fclose(ptr);
}

int main(void)
{
    iniciaExcel();
    return 0;
}
