/* Autor :  Tomas Tavares
   Numero : 95680   */

#include <stdio.h>
#include <string.h>

#define MAX_DESCRICAO 64
#define MAX_ENCOMENDAS 500
#define MAX_PRODUTOS_ENCOMENDA 200
#define MAX_PRODUTOS 10000

#define ID 0
#define QUANTIDADE 1
 
typedef struct Produto {
    /* E a estrutura de um produto que e constituido por:
            -> um identificador(int [0 - 9999])
            -> uma descricao(str != "" com max_caracteres = 63)
            -> o seu preco(int > 0)
            -> o seu peso(int > 0)
            -> a sua quantidade em stock(int >= 0) */

    int id;
    char descricao[MAX_DESCRICAO];
    int preco, peso, quantidade;
} Produto;

typedef struct Encomenda
{
    /* E a estrucura de uma encomenda que e constituida por:
            -> os produtos(com 2 atributos cada[id e quantidade])
            -> e o peso total da encomenda(0 <= int <= 200)  */

    int produtos[MAX_PRODUTOS_ENCOMENDA][2];
    int peso_total, proximo_id;
} Encomenda;



/* Declaracao de funcoes */
    /* Funcoes dos comandos*/
void obter_comando();
void adicionar_produto();
void adicionar_stock();
void cria_encomenda();
void adicionar_produto_encomenda();
void remove_quantidade();
void remove_produto_encomenda();
void custo_encomenda();
void altera_preco();
void lista_produto_encomanda();
void maior_quantidade_produto();
void lista_produtos();
void lista_produtos_encomanda();


    /* Funcoes Auxiliares*/
void merge_sort();
void merge();
int obter_pos_produto();
int encomenda_naoExiste();
int produto_naoExiste();


/* Declaracao de variaveis */
int a_correr = 1;

int num_produtos = 0, num_encomendas = 0;

/* Todos os produtos existentes */
Produto produtos[MAX_PRODUTOS];
/* Todas as encomandas existentes */
Encomenda encomendas[MAX_ENCOMENDAS];

int main()
{
    while (a_correr)
        obter_comando();

    return 0;
}


void obter_comando() {
    /* Funcao que le o comando e executa a sua funcionalidade*/
    char comando;
    comando = getchar();

    switch (comando) {
        case 'a':
            adicionar_produto();
            break;
        case 'q':
            adicionar_stock();
            break;
        case 'N':
            cria_encomenda();
            break;
        case 'A':
            adicionar_produto_encomenda();
            break;
        case 'r':
            remove_quantidade();
            break;
        case 'R':
            remove_produto_encomenda();
            break;
        case 'C':
            custo_encomenda();
            break;
        case 'p':
            altera_preco();
            break;
        case 'E':
            lista_produto_encomanda();
            break;
        case 'm':
            maior_quantidade_produto();
            break;
        case 'l':
            lista_produtos();
            break;
        case 'L':
            lista_produtos_encomanda();
            break;
        case 'x':
            a_correr = 0;
            break; 
    }
}

void adicionar_produto() 
{
    /* Adicionar um produto ao sistema ,recebendo argumentos:
                - descricao:preco:peso:quantidade    */
    scanf(" %[^:]:%d:%d:%d", produtos[num_produtos].descricao, &produtos[num_produtos].preco,
                            &produtos[num_produtos].peso, &produtos[num_produtos].quantidade);

    produtos[num_produtos].id = num_produtos;
    printf("Novo produto %d.\n", num_produtos);

    num_produtos++;

}

void adicionar_stock()
{
    /* Adiciona stock a um produto ja existente ,recebemdo argumentos:
                        - id:quantidade       
            Erros: - Se o produto nao existir    */ 
    int idp, quantidade;
    /* Obter argumentos */
    scanf(" %d:%d", &idp, &quantidade);

    if (produto_naoExiste(idp))
        printf("Impossivel adicionar produto %d ao stock. Produto inexistente.\n", idp);
    else
        produtos[idp].quantidade += quantidade;

}

void cria_encomenda()
{
    /* Cria uma nova encomenda (nao recebe nenhum argumento) */
    printf("Nova encomenda %d.\n", num_encomendas);
    num_encomendas++;

}


void adicionar_produto_encomenda()
{
    /* Adiciona um produto do sistema a encomenda, recebando como argumentos:
                    - id_ecomenda:id_produto:quantidade
        Erros: - Encomenda ou produto nao existem
               - Quantidade de produto insuficiente
               - O peso da encomenda excede 200      */

    int ide, idp, qnt_aAdicionar, pos_produto, peso_produtos; 
    /* Obter argumentos */
    scanf(" %d:%d:%d", &ide, &idp, &qnt_aAdicionar);

    /* Erros */ 
    if (encomenda_naoExiste(ide))
        printf("Impossivel adicionar produto %d a encomenda %d. Encomenda inexistente.\n", idp, ide);

    else if (produto_naoExiste(idp))
        printf("Impossivel adicionar produto %d a encomenda %d. Produto inexistente.\n", idp, ide);

    else if (produtos[idp].quantidade < qnt_aAdicionar)
        printf("Impossivel adicionar produto %d a encomenda %d. Quantidade em stock insuficiente.\n", idp, ide);

    else if (encomendas[ide].peso_total + (produtos[idp].peso * qnt_aAdicionar) > 200)
        printf("Impossivel adicionar produto %d a encomenda %d. Peso da encomenda excede o maximo de 200.\n", idp, ide);

    else{
        peso_produtos = qnt_aAdicionar * produtos[idp].peso;
        pos_produto = obter_pos_produto(ide, idp);

        /* Remove quantidade de produto a adicionar do stock*/
        produtos[idp].quantidade -= qnt_aAdicionar;

        if (pos_produto != -1) {
            /* Produto exite so e necessario alterar o peso[Encomanda] e a sua quantidade */
            encomendas[ide].produtos[pos_produto][QUANTIDADE] += qnt_aAdicionar;
            encomendas[ide].peso_total += peso_produtos;
        }
        else {
            /* O produto nao existe logo e necessario inicializa-lo [quanidade e id]*/
            pos_produto = encomendas[ide].proximo_id;
            encomendas[ide].produtos[pos_produto][ID] = idp;
            encomendas[ide].produtos[pos_produto][QUANTIDADE] = qnt_aAdicionar;
            
            /* E e necessario alterar o peso da encomenda e o numero de produtos da encomenda
                    visto que passa a existir mais um produto dentro da encomenda */
            encomendas[ide].peso_total += peso_produtos;
            encomendas[ide].proximo_id++;
        }
    }  
}

void remove_quantidade()
{
    /* Remove um produto do stock se for possivel, podendo levantar os erros:
            - O produto ainda nao tiver sido criado
            - A quantidade a remover for maior que o stock existente */

    int idp, qnt_remover;
    /* Obter argumentos */
    scanf(" %d:%d", &idp, &qnt_remover);

    /* Erros */
    if (produto_naoExiste(idp))
        printf("Impossivel remover stock do produto %d. Produto inexistente.\n", idp);
    else if (produtos[idp].quantidade < qnt_remover)
        printf("Impossivel remover %d unidades do produto %d do stock. Quantidade insuficiente.\n", qnt_remover, idp);
    else
        produtos[idp].quantidade -= qnt_remover;

}

void remove_produto_encomenda()
{
    /* Remove um produto de uma encomanda, recebendo como argumentos:
                        - id_encomenda:id_produto

        Erros: - Encomenda ou produto nao existem      */

    int ide, idp, pos_produto, qnt_aRemover;
    /* Obter argumentos */
    scanf(" %d:%d", &ide, &idp);

    /* Erros */
    if (encomenda_naoExiste(ide))
        printf("Impossivel remover produto %d a encomenda %d. Encomenda inexistente.\n", idp, ide);
    else if (produto_naoExiste(idp))
        printf("Impossivel remover produto %d a encomenda %d. Produto inexistente.\n", idp, ide);
    else {
        pos_produto = obter_pos_produto(ide, idp);
        
        if (pos_produto != -1) {
            /* O produto existe na encomenda por isso deve ser removido(quantidade e peso encomenda) */
            qnt_aRemover = encomendas[ide].produtos[pos_produto][QUANTIDADE];
            produtos[idp].quantidade += qnt_aRemover;

            encomendas[ide].peso_total -= (qnt_aRemover * produtos[idp].peso);
            encomendas[ide].produtos[pos_produto][QUANTIDADE] = 0;
        }
    }
}

void custo_encomenda()
{
    /* Calcula o custo da encomenda, recebendo como argumentos:
                        - id_encomenda 

        Erros: - A encomenda nao existe   */

    int ide, idp, i, quantidade, total = 0;
    /* Obter argumentos */
    scanf(" %d", &ide);

    if (encomenda_naoExiste(ide))
        printf("Impossivel calcular custo da encomenda %d. Encomenda inexistente.\n", ide);
    else {
        /* Itera por todos os produtos da encomenda e tendo em conta a 
           sua quantidade e preco, calcula o custo total da encomenda */

        for (i = 0; i < encomendas[ide].proximo_id; i++) {
            idp = encomendas[ide].produtos[i][ID];
            quantidade = encomendas[ide].produtos[i][QUANTIDADE];

            total += (quantidade * produtos[idp].preco);
        } 
        printf("Custo da encomenda %d %d.\n", ide, total);
    }
}


void altera_preco()
{
    /* Altera o preco de um produto em stock, recebando como argumentos:
                        - id_produto:novo_preco   
        Erros: - O produto ainda nao tiver sido criado*/

    int idp, novo_preco;
    /* Obter argumentos */
    scanf(" %d:%d", &idp, &novo_preco);

    /* Erros */
    if (produto_naoExiste(idp))
        printf("Impossivel alterar preco do produto %d. Produto inexistente.\n", idp);
    else
        produtos[idp].preco = novo_preco;
    
}


void lista_produto_encomanda() 
{
    /* Lista a descricao e a quantidade de um certo produto da encomanda, 
                          recebenco como input:
               - id_encomanda:id_produto

        Erros: - A encomanda nao existe
               - O produto nao existe              */

    int ide, idp, pos_produto, quantidade;
    scanf(" %d:%d", &ide, &idp);

    /* Erros */
    if (encomenda_naoExiste(ide))
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ide);
    else if (produto_naoExiste(idp))
        printf("Impossivel listar produto %d. Produto inexistente.\n", idp);
    else {
        pos_produto = obter_pos_produto(ide, idp);

        /* Defenir quantidade = 0 se o produto nao estiver na encomanda*/
        if (pos_produto != -1)
            quantidade = encomendas[ide].produtos[pos_produto][QUANTIDADE];
        else
            quantidade = 0;

        printf("%s %d.\n", produtos[idp].descricao , quantidade);
    }
}


void maior_quantidade_produto()
{
    /* Verifica em todas as encomendas a quantidade de um certo produto
       que procura a encomenda ou encomendas onde esse produto tem maior 
                    quantidade, recebe como argumentos:
                            - id_produto

        Erros: - O produto nao se encontra em nenhuma encomenda */
        int idp, ide_max = -1, qnt_max = 0, ide, pos_produto;
        scanf(" %d", &idp);

        /* Erros */
        if (produto_naoExiste(idp))
            printf("Impossivel listar maximo do produto %d. Produto inexistente.\n", idp);

        /* Itera por todas as encomendas a procura do produto com idp com maior
                        quantidade nas diferentes encomendas  */
        for (ide = 0; ide < num_encomendas; ide++) {
            pos_produto = obter_pos_produto(ide, idp);

            /* Se o produto existir na encomenda e a sua quantidade for maior que 
            todos ate agora vistos esse passa a ser o produto com maior quantidade */
            if (pos_produto != -1 && qnt_max < encomendas[ide].produtos[pos_produto][QUANTIDADE]) {
                qnt_max = encomendas[ide].produtos[pos_produto][QUANTIDADE];
                ide_max = ide;
            }
        }

        /* So produz output se o produto existir em pelo menos uma das encomedas
           onde o output e o produto com maior quantidade em todas as encomendas */
        if (ide_max != -1)
            printf("Maximo produto %d %d %d.\n", idp, ide_max, qnt_max);          
}

void lista_produtos()
{
    /* Lista todos os produtos existentes em stock por
    ordem crescentede preco usando o merge sort [Estavel] */

    int lista_idProdutos[MAX_PRODUTOS];
    int idp, i;

    /* Inicializar vetor com todos os id_produtos existentes*/
    for (idp = 0; idp < num_produtos; idp++)
        lista_idProdutos[idp] = idp;

    /* Sort dos id_produtos segundo o seu preco */
    merge_sort(lista_idProdutos, 0, num_produtos - 1);

    /* Listar Elementos */
    printf("Produtos\n");
    for (i = 0; i < num_produtos; i++) {
        idp = lista_idProdutos[i];
        printf("* %s %d %d\n", produtos[idp].descricao, produtos[idp].preco, produtos[idp].quantidade); 
    }
}


void merge_sort(int array[], int lim_inf, int lim_sup) {
    int meio = (lim_sup + lim_inf) / 2;
    if (lim_sup <= lim_inf)
        return;
    
    merge_sort(array, lim_inf, meio);
    merge_sort(array, meio + 1, lim_sup);
    merge(array, lim_inf, meio, lim_sup);
}

void merge(int array[], int lim_inf, int meio, int lim_sup)
{
    int i, j, tamanho_array = lim_inf; 
    int tamanho_esquerda = meio - lim_inf + 1; 
    int tamanho_direita =  lim_sup - meio; 
  
    /* create temp arrays */
    int esquerda[MAX_PRODUTOS / 2], direita[MAX_PRODUTOS / 2]; 
  
    /* Copia todos os elementos do array para dois arrays temporarios */
    for (i = 0; i < tamanho_esquerda; i++) 
        esquerda[i] = array[lim_inf + i]; 
    for (j = 0; j < tamanho_direita; j++) 
        direita[j] = array[meio + 1+ j]; 
  

    /* Juntar os dois arrays temporarios em 1 so ordenado */
    i = 0;  j = 0;   

    /* Comparar os dois primeiros elementos e juntar o mais pequeno ao array final
                         ate que um desses se acabe*/
    while (i < tamanho_esquerda && j < tamanho_direita) { 
        if (produtos[esquerda[i]].preco <= produtos[direita[j]].preco)
            array[tamanho_array++] = esquerda[i++];   
        else
            array[tamanho_array++] = direita[j++];
    } 
  
    /* Copia o resto dos elementos do array esquerda se 
            existirem alguns para o array final */
    while (i < tamanho_esquerda) 
        array[tamanho_array++] = esquerda[i++];
  
    /* Copia o resto dos elementos do array direita se 
            existirem alguns para o array final */
    while (j < tamanho_direita) 
        array[tamanho_array++] = direita[j++]; 
}


void lista_produtos_encomanda()
{
    /* Lista todos os produtos existentes em uma encomenda por
        ordem alfabetica de descricao, recebe como argumentos:
                        - id_encomenda */

    int ide, idp, quantidade, lista_idProdutos[MAX_PRODUTOS_ENCOMENDA];
    int i, j, pos_atual;
    char decricao_atual[MAX_DESCRICAO], descricao_aComparar[MAX_DESCRICAO];
    /* Obter argumentos */
    scanf(" %d", &ide);

    /* Erros*/
    if (encomenda_naoExiste(ide))
        printf("Impossivel listar encomenda %d. Encomenda inexistente.\n", ide);
    else {
        /* Inicializar array com todos os idp e suas quantidades 
                        da encomenda   */
        for (i = 0; i < encomendas[ide].proximo_id; i++) {
            lista_idProdutos[i] = i;
        }

        /* Insertion Sort */
        for (i = 0; i < encomendas[ide].proximo_id; i++) {
            pos_atual = lista_idProdutos[i];
            j = i - 1;

            strcpy(decricao_atual, produtos[encomendas[ide].produtos[pos_atual][ID]].descricao);
            strcpy(descricao_aComparar, produtos[encomendas[ide].produtos[lista_idProdutos[j]][ID]].descricao);
            
            while (j >= 0 && strcmp(decricao_atual, descricao_aComparar) < 0) {
                lista_idProdutos[j + 1] = lista_idProdutos[j];
                j--;

                strcpy(descricao_aComparar, produtos[encomendas[ide].produtos[lista_idProdutos[j]][ID]].descricao);
            }
            lista_idProdutos[j + 1] = pos_atual;
        }

        /* Listar Elementos */
        printf("Encomenda %d\n", ide);
        for (i = 0; i < encomendas[ide].proximo_id; i++) {
            idp = encomendas[ide].produtos[lista_idProdutos[i]][ID];
            quantidade = encomendas[ide].produtos[lista_idProdutos[i]][QUANTIDADE];
            if (quantidade)
                printf("* %s %d %d\n", produtos[idp].descricao, produtos[idp].preco, quantidade);
        }
    }
}

int encomenda_naoExiste(int ide)
{
    /* Verifica se a encomenda[id_encomenda] ja foi criada 
            retornando:  - 1 : Encomenda ja foi criada
                         - 0 : Encomenda nao foi criada      */

    if (ide >= num_encomendas)
        return 1;
    else 
        return 0;
}

int produto_naoExiste(int idp)
{
    /* Verifica se o produto[id_produto] existe retornando :
                - 1 : Produto existe
                - 0 : Produto nao existe            */
                
    if (idp >= num_produtos)
        return 1;
    else
        return 0;
    
}

int obter_pos_produto(int ide, int idp)
{
    /* Recebe como argumendos o id_encomenda e o id_produto e procura 
              esse produto na encomenda se encontrar devolve 
       a posicao do produto na encomenda caso contrario devolve -1  */

    int pos_produto, qnt_produtos = encomendas[ide].proximo_id;

    for (pos_produto = 0; pos_produto < qnt_produtos; pos_produto++)
        if (encomendas[ide].produtos[pos_produto][ID] == idp)
            return pos_produto;
    
    return -1;
}

